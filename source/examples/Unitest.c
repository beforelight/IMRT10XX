//
// Created by 17616 on 2020/7/22.
//

#include <sc_gpio.h>
#include <board/pin_mux.h>
#include <drv_keypad.h>
#include <fsl_debug_console.h>
#include <FreeRTOS.h>
#include <task.h>
#include <drv_lcd.h>
#include <drv_oled.h>
#include <sc_sd.h>
#include <drv_ov7725.h>
#include <drv_zzf.h>
#include "Unitest.h"
#include <stdio.h>
#include <sc_adc.h>
#include <sc_enc.h>
#include <sc_flash.h>
#include <sc_pit.h>
#include <sc_pwm.h>
#include <status.h>
TaskHandle_t LED_task_handle;
void LED_task(void* pvData) {
	gpio_t led1 = { XSNVS_PMIC_STBY_GPIO, XSNVS_PMIC_STBY_PIN,0 };
	GPIO_Init(&led1);
	while (1) {
        GPIO_Toggle(&led1);
		vTaskDelay(1 * configTICK_RATE_HZ);//等待1秒
	}
}

UnitestItem_t item_list[] = {
        {U_keypad,"keypad",NULL},
        {NULL,NULL,NULL},//确定结尾有多少个
};

void Unitest(void) {
    int num=0;
    TaskHandle_t task_handle;
    TaskStatus_t xTaskDetails;
    vTaskGetInfo(NULL, &xTaskDetails, pdTRUE, eInvalid);//获取自己任务优先级
    while(item_list[num].pxTaskCode !=NULL){
        num++;
    }
    PRINTF("Unitest has %d items\r\n", num);
    uint8_t* item_done = pvPortMalloc(sizeof(uint8_t) * num);
    memset(item_done, 0, sizeof(uint8_t) * num);
    while (1)
    {
        PRINTF("\r\nUnitest>>>\r\n");
        for (int i = 0; i < num; i++)
        {
            if (item_done[i] == 0) {
                PRINTF("%d:\t%s\r\n", i, item_list[i].pcName);
            }
            else{
                PRINTF("%d:\t%s\t√\r\n", i, item_list[i].pcName);
            }
        }
        PRINTF("Enter the number and -1 to exit:");
        int ans = -1;
        SCANF("%d", &ans);
        if (ans == -1) {
            PRINTF("Unitest exit!\r\n");
            break;
        }
        else if (0<=ans&&ans<num)//选中任务啦
        {
            PRINTF("run %s......", item_list[ans].pcName);
            xTaskCreate(item_list[ans].pxTaskCode,
                item_list[ans].pcName, 2048,
                item_list[ans].pvParameters,
                xTaskDetails.uxCurrentPriority,
                &task_handle);
            eTaskState e;
            while (1) {//等待测试任务退出
                e = eTaskGetState(task_handle);
                if (e == eDeleted || e == eInvalid){
                    break;
                }
                else{
                    vTaskDelay(100);
                }
            }
            item_done[ans] = 1;
            PRINTF("Done!\r\n");
        }
    }
    vPortFree(item_done);
}

void U_keypad(void *pv) {
    gpio_t row_list[] = {
            {LCD_D0_GPIO, LCD_D0_PIN, 0},
            {LCD_D1_GPIO, LCD_D1_PIN, 0},
            {LCD_D2_GPIO, LCD_D2_PIN, 0},
            {LCD_D3_GPIO, LCD_D3_PIN, 0},
            {LCD_D4_GPIO, LCD_D4_PIN, 0},
            {LCD_D5_GPIO, LCD_D5_PIN, 0},
            {LCD_D6_GPIO, LCD_D6_PIN, 0},
            {LCD_D7_GPIO, LCD_D7_PIN, 0},
            {0, 0,                    0}
    };

    gpio_t col_list[] = {
            {LCD_C0_GPIO, LCD_C0_PIN, 0},
            {LCD_C1_GPIO, LCD_C1_PIN, 0},
            {0, 0,                    0}
    };
    char ch;
    KEYPAD_Init(&g_keypad, col_list, row_list);
    while (1) {
        PRINTF("\r\np->print status of keypad and e->exit\r\n");
        ch = GETCHAR();
        if (ch == 'p' || ch == 'P') {
            PRINTF("\tc0\tc1\r\n");
            for (int i = 0; i < 8; i++) {
                PRINTF("r%d\t", i);
                for (int j = 0; j < 2; j++) {
                    PRINTF("%d\t", (int) KEYPAD_Get(&g_keypad, i, j));
                }
                PRINTF("\r\n");
            }
        } else if (ch == 'e' || ch == 'E') {
            break;
        }
    }
    vTaskDelete(NULL);
}

void U_lcd(void *pv) {
    Lcd_Init();            //初始化OLED
    LCD_Clear(WHITE);
    BACK_COLOR = WHITE;
    LCD_ShowString(10, 35, "2.4 TFT SPI 240*320", RED);
    LCD_ShowString(10, 55, "LCD_W:", RED);
    LCD_ShowNum(70, 55, LCD_W, 3, RED);
    LCD_ShowString(110, 55, "LCD_H:", RED);
    LCD_ShowNum(160, 55, LCD_H, 3, RED);
    LCD_ShowNum1(80, 95, 3.14159f, 5, RED);
    vTaskDelete(NULL);
}

void U_oled(void *pv) {
    OLED_Init();
    OLED_Fill(0xff);
    vTaskDelay(100);
    OLED_Logo();
    vTaskDelay(100);
    vTaskDelete(NULL);
}

BSS_SDRAM_NOCACHE uint8_t buff1[1024 * 1024] ALIGN(64);//最多4缓存，这里声明为1mb的缓存
BSS_SDRAM_NOCACHE uint8_t buff2[1024 * 1024] ALIGN(64);//是因为这俩摄像头都用这个缓存
BSS_SDRAM_NOCACHE uint8_t buff3[1024 * 1024] ALIGN(64);//实际图片多大，缓存就多大
BSS_SDRAM_NOCACHE uint8_t buff4[1024 * 1024] ALIGN(64);//
void U_ov7725(void *pv) {
    FIL png;
    int error;
    char str[100];
    img_t img;
    if (SD_Mount() == kStatus_Success) {
        char ch;
        Lcd_Init();
        while (1) {
            PRINTF("which one?o->ov7725 z->zzf\r\n");
            ch = GETCHAR();
            if (ch == 'o' || ch == 'O' || ch == 'z' || ch == 'Z') { break; }
        }
        if (ch == 'o' || ch == 'O') //初始化ov7725摄像头
        {
            PRINTF("select ov7725\r\n");
            CAMERA_MclkSet(24 * 1000 * 1000);
            OV7725_Init(OV7725_FrameSizeVGA480x640);
            OV7725_Light_Mode(0);
            OV7725_Color_Saturation(0);
            OV7725_Brightness(0);
            OV7725_Contrast(0);
            OV7725_Special_Effects(0);
            img.format = PixelFormatRGB565;
            img.width = 640;
            img.height = 480;
        } else if (ch == 'z' || ch == 'Z')//初始化总钻风摄像头
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
        for (int i = 0; i < 100; i++) {
            if (kStatus_Success == CAMERA_FullBufferGet(&img.pImg)) {
                snprintf(str, 100, "/picture/%d.png", i);
                error = f_open(&png, str, (FA_WRITE | FA_CREATE_ALWAYS));
                if (error) {
                    if (error == FR_EXIST) {
                        PRINTF("%s exists.\r\n", str);
                    } else {
                        PRINTF("Open %s failed.\r\n", str);
                        vTaskDelete(NULL);
                        return;
                    }
                }
                CAMERA_Save2PngFile(&img, &png);//保存到sd卡中
                LCD_PrintPicture(&img);//在屏幕上显示
                if (FR_OK == f_close(&png)) {
                    PRINTF("Save %s success.\r\n", str);
                } else {
                    PRINTF("close %s failed.\r\n", str);
                    vTaskDelete(NULL);
                    return;
                }
                CAMERA_SubmitBuff(img.pImg);//将空缓存提交
            } else {
                i--;
                vTaskDelay(1);
            }
        }
        //100张图片采集达成，统计一下fps
        PRINTF("fps=%d\r\n", (int) CAMERA_FpsGet());
        CAMERA_ReceiverStop();//停止传输
    } else {
        PRINTF("Please insert SD card\r\n");
    }
    vTaskDelete(NULL);
}

void U_adc(void *pv) {
    ADC_Init2();
    PRINTF("adc\tchannel\tvalue\r\n");
    for (int i = 3; i < 9; i++) {
        float val = (float) ADC_Read(ADC1, i) / 4096.0;
        PRINTF("adc1\%d\t%d.%d%d%dv\r\n", i, (int) val,
               ((int) (val * 10)) % 10,
               ((int) (val * 100)) % 10,
               ((int) (val * 1000)) % 10
        );
    }
    PRINTF("\r\n");
    vTaskDelete(NULL);
}

void U_enc(void *pv) {
    ENC_Init_t(ENC1);
    ENC_Init_t(ENC2);
    ENC_Init_t(ENC3);
    ENC_Init_t(ENC4);
    char ch;
    while (1) {
        PRINTF("\r\np->print status of enc and e->exit\r\n");
        ch = GETCHAR();
        if (ch == 'p' || ch == 'P') {
            PRINTF("ENC\tDate\tPosition\tRevolution\r\n");
            PRINTF("1\t%d\t%d\t%d\r\n", (int) ENC_Dateget(ENC1), (int) ENC_Positionget(ENC1),
                   (int) ENC_Revolutionget(ENC1));
            PRINTF("2\t%d\t%d\t%d\r\n", (int) ENC_Dateget(ENC2), (int) ENC_Positionget(ENC2),
                   (int) ENC_Revolutionget(ENC2));
            PRINTF("3\t%d\t%d\t%d\r\n", (int) ENC_Dateget(ENC3), (int) ENC_Positionget(ENC3),
                   (int) ENC_Revolutionget(ENC3));
            PRINTF("4\t%d\t%d\t%d\r\n", (int) ENC_Dateget(ENC4), (int) ENC_Positionget(ENC4),
                   (int) ENC_Revolutionget(ENC4));
        } else if (ch == 'e' || ch == 'E') {
            break;
        }
    }
    ENC_Dateclear(ENC1);
    ENC_Dateclear(ENC2);
    ENC_Dateclear(ENC3);
    ENC_Dateclear(ENC4);
    vTaskDelete(NULL);
}

void U_flash(void *pv) {
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

void U_flash_with_lfs(void *pv) {
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

int c0count = 0;
int c1count = 0;
int c2count = 0;
int c3count = 0;
uint32_t c0time_us = 0;
uint32_t c1time_us = 0;
uint32_t c2time_us = 0;
uint32_t c3time_us = 0;
uint32_t c0pit_us = 0;
uint32_t c1pit_us = 0;
uint32_t c2pit_us = 0;
uint32_t c3pit_us = 0;

void U_pit(void *pv) {
    PIT_Init2(kPIT_Chnl_0, 500);//500us
    PIT_Init2(kPIT_Chnl_1, 5 * 1000);//5ms
    PIT_Init2(kPIT_Chnl_2, 10 * 1000);//10ms
    PIT_Init2(kPIT_Chnl_3, 20 * 1000);//20ms
    NVIC_SetPriority(PIT_IRQn, 6);//设置pit中断优先级为6
    PIT_StartTimer(PIT, kPIT_Chnl_0);
    PIT_StartTimer(PIT, kPIT_Chnl_1);
    PIT_StartTimer(PIT, kPIT_Chnl_2);
    PIT_StartTimer(PIT, kPIT_Chnl_3);
    while (1) {
        vTaskDelay(10);
        if (c0count >= 3 &&
            c1count >= 3 &&
            c2count >= 3 &&
            c3count >= 3) {
            PRINTF("Period us-->ch0:%d,ch1:%d,ch2:%d,ch3:%d\r\n", c0pit_us, c1pit_us, c2pit_us, c3pit_us);
            break;
        }
    }
    vTaskDelete(NULL);
}

pwm_t my1 = {PWM1, kPWM_Module_3, 25 * 1000, 0, 0, kPWM_HighTrue};
pwm_t my2 = {PWM2, kPWM_Module_3, 25 * 1000, 0, 0, kPWM_HighTrue};
pwm_t my3 = {PWM2, kPWM_Module_2, 25 * 1000, 0, 0, kPWM_HighTrue};
pwm_t my4 = {PWM2, kPWM_Module_1, 25 * 1000, 0, 0, kPWM_HighTrue};
gpio_t OE_B = {PWM_OE_B_GPIO, PWM_OE_B_PIN, 0};

void U_pwm(void *pv) {
    pwm_t *list[] ={
                    &my1,
                    &my2,
                    &my3,
                    &my4,
                    NULL
            };
    PWM_Init2(list);
    GPIO_Init(&OE_B);
    my1.dutyA = 10.0;
    my1.dutyB = 20.0;
    my2.dutyA = 30.0;
    my2.dutyB = 40.0;
    my3.dutyA = 50.0;
    my3.dutyB = 60.0;
    my4.dutyA = 70.0;
    my4.dutyB = 80.0;
    PWM_Change(&my1);
    PWM_Change(&my2);
    PWM_Change(&my3);
    PWM_Change(&my4);
    GPIO_Write(&OE_B, 0);//使能缓冲芯片输出
    vTaskDelete(NULL);
}

void U_status(void *pv) {
    TaskStatusPrint();
    APP_PrintRunFrequency(0);
    PRINTF("%dms\r\n", TimerMsGet());
    vTaskDelete(NULL);
}
//中断服务函数（不准直接改名字可以用define改名字）
//注意四个pit通道共用一个中断服务函数
/*RAMFUNC_ITC*/ void PIT_IRQHandler(void) {
    /*清除中断标志位 （要用的时候解注释，通道可换）*/
    if (PIT_GetStatusFlags(PIT, kPIT_Chnl_0) == kPIT_TimerFlag) {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
        c0count++;
        c0pit_us = TimerUsGet() - c0time_us;
        c0time_us = TimerUsGet();
        if (c0count >= 3) {
            PIT_StopTimer(PIT, kPIT_Chnl_0);
        }
    }
    if (PIT_GetStatusFlags(PIT, kPIT_Chnl_1) == kPIT_TimerFlag) {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, kPIT_TimerFlag);
        c1count++;
        c1pit_us = TimerUsGet() - c1time_us;
        c1time_us = TimerUsGet();
        if (c1count >= 3) {
            PIT_StopTimer(PIT, kPIT_Chnl_1);
        }
    }
    if (PIT_GetStatusFlags(PIT, kPIT_Chnl_2) == kPIT_TimerFlag) {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, kPIT_TimerFlag);
        c2count++;
        c2pit_us = TimerUsGet() - c2time_us;
        c2time_us = TimerUsGet();
        if (c2count >= 3) {
            PIT_StopTimer(PIT, kPIT_Chnl_2);
        }
    }
    if (PIT_GetStatusFlags(PIT, kPIT_Chnl_3) == kPIT_TimerFlag) {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_3, kPIT_TimerFlag);
        c3count++;
        c3pit_us = TimerUsGet() - c3time_us;
        c3time_us = TimerUsGet();
        if (c3count >= 3) {
            PIT_StopTimer(PIT, kPIT_Chnl_3);
        }
    }
    /*中断服务函数内容*/
    __DSB();
    /*DSB--数据同步屏障
     * 作用：因为CPU时钟的主频大于IP总线时钟故会出现中断标志位还未被清掉时中断服务函数中的内容会执行完，而使得程序一遍一遍的进入中断服务函数，
     * 而DSB则是为了预防这一情况的发生。*/
}
