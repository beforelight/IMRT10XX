/*
 * test.h
 *
 *  Created on: 2020年4月15日
 *      Author: 17616
 */
 //#define EXAMPLES_TEST_H_

#ifndef EXAMPLES_TEST_H_
#define EXAMPLES_TEST_H_
#include "FreeRTOS.h"
#include "task.h"
#include "fsl_debug_console.h"
#include "smartcar/status.h"
#include "board.h"

#define TEST_ADC
#define TEST_CAMERA
#define TEST_ENC
#define TEST_FLASH
//#define TEST_GPIO//这个就不用写示例了
//#define TEST_IIC//没法单独测试
#define TEST_KEYPAD

void test(TaskFunction_t pxTaskCode, const char* const pcName, uint32_t TimeOutMs, int runDirect) {
	if (TimeOutMs == 0) { TimeOutMs = ~0; }
	TaskStatus_t xTaskDetails;
	vTaskGetInfo(NULL, &xTaskDetails, pdTRUE, eInvalid);
	//    xTaskDetails.uxCurrentPriority;//调用Test函数任务的优先级
	if (runDirect == 0)
	{
		PRINTF(">>run task %s? y/n \r\n", pcName);
		char ch = GETCHAR();
		if (ch == 'y' || ch == 'Y') {}
		else
		{
			return;
		}
	}
	PRINTF("run task %s>>\r\n", pcName);
	uint32_t msp = TimerUsGet();
	TaskHandle_t task_handle;
	xTaskCreate(pxTaskCode, pcName, 2048, NULL, xTaskDetails.uxCurrentPriority, &task_handle);
	vTaskDelay(100);
	eTaskState e;
	while (1)
	{
		e = eTaskGetState(task_handle);
		if (e == eDeleted || e == eInvalid)
		{
			PRINTF("task %s exit!\r\n", pcName);
			return;
		}
		else if (TimerUsGet() - msp >= TimeOutMs)
		{
			vTaskDelete(task_handle);
			PRINTF("task %s time out!\r\n", pcName);
			return;
		}
		vTaskDelay(100);
	}
}

#ifdef TEST_ADC
#include "smartcar/sc_adc.h"
void adc(void* pv)
{
	ADC_Init2();
	PRINTF("adc\tchannel\tvalue\r\n");
	for (int i = 3; i < 9; i++)
	{
		float val = ADC_Read(ADC1, i) / 4096.0;
		PRINTF("adc1\%d\t%d.%d%d%dv\r\n", i, (int)val,
			((int)(val * 10)) % 10,
			((int)(val * 100)) % 10,
			((int)(val * 1000)) % 10
		);
	}
	PRINTF("\r\n");
	vTaskDelete(NULL);
}
#endif // TEST_ADC

#ifdef TEST_CAMERA 
#include <stdio.h>
#include "smartcar/sc_sd.h"
#include "smartcar/sc_camera_ov7725.h"
#include "smartcar/sc_camera_zzf.h"
BSS_SDRAM_NOCACHE uint8_t buff1[1024 * 1024] ALIGN(64);//最多4缓存，这里声明为1mb的缓存
BSS_SDRAM_NOCACHE uint8_t buff2[1024 * 1024] ALIGN(64);//是因为这俩摄像头都用这个缓存
BSS_SDRAM_NOCACHE uint8_t buff3[1024 * 1024] ALIGN(64);//实际图片多大，缓存就多大
BSS_SDRAM_NOCACHE uint8_t buff4[1024 * 1024] ALIGN(64);//
void camera(void* pv)//采集图像并且保存到sd卡中
{
	FIL png;
	int error;
	char str[100];
	img_t img;
	if (SD_Mount() == kStatus_Success)
	{
		char ch;
		while (1)
		{
			PRINTF("which one?o->ov7725 z->zzf\r\n");
			ch = GETCHAR();
			if (ch == 'o' || ch == 'O' || ch == 'z' || ch == 'Z') { break; }
		}
		if (ch == 'o' || ch == 'O') //初始化ov7725摄像头
		{
			PRINTF("select ov7725\r\n");
			OV7725_Init(OV7725_FrameSizeVGA480x640);
			OV7725_Light_Mode(0);
			OV7725_Color_Saturation(0);
			OV7725_Brightness(0);
			OV7725_Contrast(0);
			OV7725_Special_Effects(0);
			img.format = PixelFormatRGB565;
			img.width = 640;
			img.height = 480;
		}
		else if (ch == 'z' || ch == 'Z')//初始化总钻风摄像头
		{
			PRINTF("select zzf\r\n");
			UART_Init(LPUART4, 9600, 80 * 1000 * 1000);
			ZZF_Init(ZZF_FrameSize480x752, LPUART4);
			img.format = PixelFormatGray;
			img.width = 752;
			img.height = 480;
		}
		//摄像头初始化完毕，开始接收图像
		CAMERA_SubmitBuff(buff1);
		CAMERA_SubmitBuff(buff2);
		CAMERA_SubmitBuff(buff3);
		CAMERA_SubmitBuff(buff4);
		assert(kStatus_Success == CAMERA_ReceiverStart());
		for (int i = 0; i < 100; i++)
		{
			if (kStatus_Success == CAMERA_FullBufferGet(img.pImg)) {
				snprintf(str, 100, "/picture/%d.png", i);
				error = f_open(&png, str, (FA_WRITE | FA_CREATE_ALWAYS));
				if (error)
				{
					if (error == FR_EXIST)
					{
						PRINTF("%s exists.\r\n", str);
					}
					else
					{
						PRINTF("Open %s failed.\r\n", str);
						vTaskDelete(NULL);
						return;
					}
				}
				CAMERA_Save2PngFile(&img, &png);//保存到sd卡中
				if (FR_OK == f_close(&png))
				{
					PRINTF("Save %s success.\r\n", str);
				}
				else
				{
					PRINTF("close %s failed.\r\n", str);
					vTaskDelete(NULL);
					return;
				}
				CAMERA_SubmitBuff(img.pImg);//将空缓存提交
			}
			else
			{
				i--;
			}
		}
		//100张图片采集达成，统计一下fps
		PRINTF("fps=%d\r\n", (int)CAMERA_FpsGet());
		CAMERA_ReceiverStop();//停止传输
	}
	else
	{
		PRINTF("Please insert SD card\r\n");
	}
	vTaskDelete(NULL);

}
#endif // TEST_CAMERA


#ifdef TEST_ENC
#include "smartcar/sc_enc.h"
void enc(void* pv)
{
	ENC_Init_t(ENC1);
	ENC_Init_t(ENC2);
	ENC_Init_t(ENC3);
	ENC_Init_t(ENC4);
	char ch;
	while (1)
	{
		PRINTF("\r\np->print status of enc and e->exit\r\n");
		ch = GETCHAR();
		if (ch == 'p' || ch == 'P')
		{
			PRINTF("ENC\tDate\tPosition\tRevolution\r\n");
			PRINTF("1\t%d\t%d\t%d\r\n", (int)ENC_Dateget(ENC1), (int)ENC_Positionget(ENC1), (int)ENC_Revolutionget(ENC1));
			PRINTF("2\t%d\t%d\t%d\r\n", (int)ENC_Dateget(ENC2), (int)ENC_Positionget(ENC2), (int)ENC_Revolutionget(ENC2));
			PRINTF("3\t%d\t%d\t%d\r\n", (int)ENC_Dateget(ENC3), (int)ENC_Positionget(ENC3), (int)ENC_Revolutionget(ENC3));
			PRINTF("4\t%d\t%d\t%d\r\n", (int)ENC_Dateget(ENC4), (int)ENC_Positionget(ENC4), (int)ENC_Revolutionget(ENC4));
		}
		else if (ch == 'e' || ch == 'E')
		{
			break;
		}
	}
	ENC_Dateclear(ENC1);
	ENC_Dateclear(ENC2);
	ENC_Dateclear(ENC3);
	ENC_Dateclear(ENC4);
	vTaskDelete(NULL);
}
#endif // TEST_ENC

#ifdef TEST_FLASH
#include "smartcar/sc_flash.h"
void flash(void* pv) {
	PRINTF("flash test\r\n");
	assert(0 == FLASH_Init());
	int addr_start = 3 * 1024 * 1024;
	static uint8_t buff_r[FLASH_SECTOR_SIZE];
	static uint8_t buff_w[FLASH_SECTOR_SIZE];
	assert(0 == FLASH_Erase(addr_start));
	assert(0 == FLASH_Read(addr_start, buff_r, FLASH_SECTOR_SIZE));
	for (size_t i = 0; i < FLASH_SECTOR_SIZE; i++) {
		assert(0xff == buff_r[i]);
	}
	for (size_t i = 0; i < FLASH_PAGE_SIZE; i++) {
		buff_w[i] = i;
	}
	for (size_t i = 0; i < FLASH_SECTOR_SIZE / FLASH_PAGE_SIZE; i++) {
		assert(0 == FLASH_Prog(addr_start + i * FLASH_PAGE_SIZE, buff_w));
	}
	assert(0 == FLASH_Read(addr_start, buff_r, FLASH_SECTOR_SIZE));
	for (size_t i = 0; i < FLASH_SECTOR_SIZE / FLASH_PAGE_SIZE; i++) {
		for (size_t j = 0; j < FLASH_PAGE_SIZE; j++) {
			assert(buff_r[j] == j);
		}
	}
	PRINTF("flash test success\r\n");
	vTaskDelete(NULL);
}


void flash_lfs(void* pv) {
	PRINTF("flash rw with lfs\r\n");
	assert(0 == FLASH_Init());
	static lfs_t lfs;
	static struct lfs_config cfg;
	static lfs_file_t file;
	FLASH_LfsGetDefaultConfig(&cfg);
	int err = lfs_mount(&lfs, &cfg);

	// reformat if we can't mount the filesystem
	// this should only happen on the first boot
	if (err) {
		lfs_format(&lfs, &cfg);
		lfs_mount(&lfs, &cfg);
		PRINTF("reformat!\r\n");
	}
	// read current count
	uint32_t boot_count = 0;
	lfs_file_open(&lfs, &file, "boot_count", LFS_O_RDWR | LFS_O_CREAT);
	lfs_file_read(&lfs, &file, &boot_count, sizeof(boot_count));

	// update boot count
	boot_count += 1;
	lfs_file_rewind(&lfs, &file);
	lfs_file_write(&lfs, &file, &boot_count, sizeof(boot_count));

	// remember the storage is not updated until the file is closed successfully
	lfs_file_close(&lfs, &file);

	// release any resources we were using
	lfs_unmount(&lfs);

	// print the boot count
	PRINTF("%s boot_count: %d\r\n", BOARD_NAME, boot_count);

	//    while (1){vTaskDelay(1000);TaskStatusPrint();}
	vTaskDelete(NULL);
}
#endif // TEST_FLASH



#endif /* EXAMPLES_TEST_H_ */
