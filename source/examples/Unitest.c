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
#include <sc_i2c.h>
#include <sc_i2cs.h>
#include <sc_pit.h>
#include <sc_pwm.h>
#include <status.h>
#include <source/smartcar/svbmp.h>
#include <stdio.h>

TaskHandle_t LED_task_handle;

void LED_task(void *pvData) {
    gpio_t led1 = {XSNVS_PMIC_STBY_GPIO, XSNVS_PMIC_STBY_PIN, 0};
    GPIO_Init(&led1);
    while (1) {
        GPIO_Toggle(&led1);
        vTaskDelay(1 * configTICK_RATE_HZ);//等待1秒
    }
}

UnitestItem_t default_item_list[] = {
        {U_status, "status", NULL},//
        {U_keypad, "keypad", NULL},//
        {U_lcd, "lcd", NULL},//
        {U_oled, "oled", NULL},//
        {U_zzf, "zzf&oled", "oled"},//
        {U_zzf, "zzf&lcd", "lcd"},//
        {U_zzf, "zzf&sd", "sd"},//
        {U_adc, "adc", NULL},//
        {U_flash, "flash", NULL},//
        {U_flash_with_lfs, "flash_with_lfs", NULL},//
#ifdef TEST_PIT
        {U_pit, "pit", NULL},//
#endif//TEST_PIT
        {U_msc, "msc", NULL},//
        {U_sd, "sd", NULL},//
        {U_file_dump, "file_dump", NULL},//
        {U_pwm, "pwm", NULL},//
        {U_enc, "enc", NULL},//
        {U_i2c_soft, "iic_soft", NULL},//
        {U_sccb_soft, "sccb_soft", NULL},//
        {NULL, NULL, NULL},//结尾为NULL以确定有多少项
};

void Unitest(UnitestItem_t item_list[]) {
    int num = 0;
    TaskHandle_t task_handle;
    TaskStatus_t xTaskDetails;
    vTaskGetInfo(NULL, &xTaskDetails, pdTRUE, eInvalid);//获取自己任务优先级
    xTaskCreate(LED_task, "led task", 64, NULL, xTaskDetails.uxCurrentPriority + 1, &LED_task_handle);
    while (item_list[num].pxTaskCode != NULL) {
        num++;
    }
    PRINTF("Unitest has %d items\r\n", num);
    uint8_t *item_done = pvPortMalloc(sizeof(uint8_t) * num);
    memset(item_done, 0, sizeof(uint8_t) * num);
    while (1) {
        PRINTF("\r\nUnitest>>>\r\n");
        for (int i = 0; i < num; i++) {
            if (item_done[i] == 0) {
                PRINTF("%d:\t%s\r\n", i, item_list[i].pcName);
            } else {
                PRINTF("%d:\t%s\t√\r\n", i, item_list[i].pcName);
            }
        }
        PRINTF("Enter the number and -1 to exit:");
        int ans = -1;
        SCANF("%d", &ans);
        PRINTF("\r\n");
        if (ans == -1) {
            PRINTF("Unitest exit!\r\n");
            break;
        } else if (0 <= ans && ans < num)//选中任务啦
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
                if (e == eDeleted || e == eInvalid) {
                    break;
                } else {
                    vTaskDelay(100);
                }
            }
            item_done[ans] = 1;
            PRINTF("Done!\r\n");
        }
    }
    vPortFree(item_done);
    vTaskDelete(LED_task_handle);
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
    KEYPAD_Init(&g_keypad, row_list, col_list);
    PRINTF("Enter 'q' to quit and others to continue\r\n");
    while (1) {
        ch = GETCHAR();
        if (ch == 'q' || ch == 'Q') {
            break;
        }
        PRINTF("\r\n\tc0\tc1\r\n");
        for (int i = 0; i < g_keypad.rowNum; i++) {
            PRINTF("r%d\t", i);
            for (int j = 0; j < g_keypad.colNum; j++) {
                PRINTF("%d\t", (int) KEYPAD_Get(&g_keypad, i, j));
            }
            PRINTF("\r\n");
        }
    }
    KEYPAD_Deinit(&g_keypad);
    vTaskDelete(NULL);
}

BSS_SDRAM uint8_t b_bmp_buf[240 * 320 * 2] ALIGN(64);
BSS_SDRAM uint8_t a_bmp_buf[120 * 184] ALIGN(64);

void U_lcd(void *pv) {
    Lcd_Init();            //初始化LCD
    LCD_Clear(WHITE);
    BACK_COLOR = WHITE;
    GETCHAR();
    LCD_Clear(RGB565(255, 0, 0));
    GETCHAR();
    LCD_Clear(RGB565(0, 255, 0));
    GETCHAR();
    LCD_Clear(RGB565(0, 0, 255));
    GETCHAR();
    LCD_Clear(WHITE);
    LCD_ShowString(10, 35, "2.4 TFT SPI 240*320", RED);
    LCD_ShowString(10, 55, "LCD_W:", RED);
    LCD_ShowNum(70, 55, LCD_W, 3, RED);
    LCD_ShowString(110, 55, "LCD_H:", RED);
    LCD_ShowNum(160, 55, LCD_H, 3, RED);
    LCD_ShowNum1(80, 95, 3.14159f, 5, RED);
    GETCHAR();//测试显示彩色图像
    LCD_Clear(WHITE);
    img_t b;
    b.format = PixelFormatRGB565;
    b.height = 240;
    b.width = 320;
    b.pImg = b_bmp_buf;
    for (size_t i = 0; i < b.width; i++) {
        ((uint16_t *) b.pImg)[28 * b.width + i] = RGB565(255, 0, 0);
        ((uint16_t *) b.pImg)[29 * b.width + i] = RGB565(255, 0, 0);
        ((uint16_t *) b.pImg)[30 * b.width + i] = RGB565(255, 0, 0);
        ((uint16_t *) b.pImg)[31 * b.width + i] = RGB565(255, 0, 0);
        ((uint16_t *) b.pImg)[39 * b.width + i] = RGB565(0, 255, 0);
        ((uint16_t *) b.pImg)[40 * b.width + i] = RGB565(0, 255, 0);
        ((uint16_t *) b.pImg)[41 * b.width + i] = RGB565(0, 255, 0);
        ((uint16_t *) b.pImg)[42 * b.width + i] = RGB565(0, 255, 0);
        ((uint16_t *) b.pImg)[49 * b.width + i] = RGB565(0, 0, 255);
        ((uint16_t *) b.pImg)[50 * b.width + i] = RGB565(0, 0, 255);
        ((uint16_t *) b.pImg)[51 * b.width + i] = RGB565(0, 0, 255);
        ((uint16_t *) b.pImg)[52 * b.width + i] = RGB565(0, 0, 255);
    }
    LCD_PrintPicture(&b);
    GETCHAR();//测试显示灰度图像
    LCD_Clear(WHITE);
    img_t a;
    a.format = PixelFormatGray;
    a.height = 120;
    a.width = 184;
    a.pImg = a_bmp_buf;
    for (size_t i = 0; i < a.width; i++) {
        ((uint8_t *) a.pImg)[41 * a.width + i] = 255;
        ((uint8_t *) a.pImg)[42 * a.width + i] = 255;
        ((uint8_t *) a.pImg)[43 * a.width + i] = 255;
        ((uint8_t *) a.pImg)[44 * a.width + i] = 255;
        ((uint8_t *) a.pImg)[31 * a.width + i] = 128;
        ((uint8_t *) a.pImg)[32 * a.width + i] = 128;
        ((uint8_t *) a.pImg)[33 * a.width + i] = 128;
        ((uint8_t *) a.pImg)[34 * a.width + i] = 128;
    }
    LCD_PrintPicture(&a);
    GETCHAR();
    LCD_Clear(WHITE);
    vTaskDelete(NULL);
}

void U_oled(void *pv) {
    OLED_Init();
    OLED_Fill(0);
    GETCHAR();
    OLED_Fill(0xff);
    GETCHAR();
    OLED_Logo();
    GETCHAR();
    OLED_Fill(0);
    OLED_P6x8Str(0, 0, (uint8_t *) "Nobody knows oled then me!");
    OLED_P6x8Rst(6 * 5, 4, (uint8_t *) "FAKE NEWS!");
    GETCHAR();//测试显示彩色图像
    OLED_Fill(0xff);
    img_t b;
    b.format = PixelFormatRGB565;
    b.height = 240;
    b.width = 320;
    b.pImg = b_bmp_buf;
    for (size_t i = 0; i < b.width; i++) {
        ((uint16_t *) b.pImg)[28 * b.width + i] = RGB565(255, 0, 0);
        ((uint16_t *) b.pImg)[29 * b.width + i] = RGB565(255, 0, 0);
        ((uint16_t *) b.pImg)[30 * b.width + i] = RGB565(255, 0, 0);
        ((uint16_t *) b.pImg)[31 * b.width + i] = RGB565(255, 0, 0);
        ((uint16_t *) b.pImg)[39 * b.width + i] = RGB565(0, 255, 0);
        ((uint16_t *) b.pImg)[40 * b.width + i] = RGB565(0, 255, 0);
        ((uint16_t *) b.pImg)[41 * b.width + i] = RGB565(0, 255, 0);
        ((uint16_t *) b.pImg)[42 * b.width + i] = RGB565(0, 255, 0);
        ((uint16_t *) b.pImg)[49 * b.width + i] = RGB565(0, 0, 255);
        ((uint16_t *) b.pImg)[50 * b.width + i] = RGB565(0, 0, 255);
        ((uint16_t *) b.pImg)[51 * b.width + i] = RGB565(0, 0, 255);
        ((uint16_t *) b.pImg)[52 * b.width + i] = RGB565(0, 0, 255);
    }
    OLED_PrintPicture(&b, 100);
    GETCHAR();//测试显示灰度图像
    OLED_Fill(0xff);
    img_t a;
    a.format = PixelFormatGray;
    a.height = 120;
    a.width = 184;
    a.pImg = a_bmp_buf;
    for (size_t i = 0; i < a.width; i++) {
        ((uint8_t *) a.pImg)[41 * a.width + i] = 255;
        ((uint8_t *) a.pImg)[42 * a.width + i] = 255;
        ((uint8_t *) a.pImg)[43 * a.width + i] = 255;
        ((uint8_t *) a.pImg)[44 * a.width + i] = 255;
        ((uint8_t *) a.pImg)[31 * a.width + i] = 128;
        ((uint8_t *) a.pImg)[32 * a.width + i] = 128;
        ((uint8_t *) a.pImg)[33 * a.width + i] = 128;
        ((uint8_t *) a.pImg)[34 * a.width + i] = 128;
    }
    OLED_PrintPicture(&a, 100);
    GETCHAR();
    OLED_Fill(0xff);
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
                //CAMERA_Save2BmpFile(&img, &png);//保存到sd卡中
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
    PRINTF("Enter 'q' to quit and others to continue\r\n");
    while (1) {
        PRINTF("\r\nadc\tchannel\tvalue\r\n");
        for (int i = 3; i < 9; i++) {
            float val = 3.3f * (float) ADC_Read(ADC1, i) / 4096.0;
            PRINTF("adc1\t%d\t%.3fv\r\n", i, val);
        }
        char quit;
        quit = GETCHAR();
        if (quit == 'q' || quit == 'Q') { break; }
    }
    PRINTF("\r\n");
    vTaskDelete(NULL);
}

void U_enc(void *pv) {
    ENC_InitDecoder(ENC1);
    ENC_InitDecoder(ENC2);
    ENC_InitDecoder(ENC3);
    ENC_InitDecoder(ENC4);
    PRINTF("Enter 'q' to quit and others to continue\r\n");
    while (1) {
		PRINTF("ENC\tPosition\tDate\tRevolution\r\n");
		PRINTF("1\t%d\t%d\t%d\r\n", (int)ENC_PositionGet(ENC1), (int)ENC_Dateget(ENC1), (int)ENC_Revolutionget(ENC1));
        PRINTF("2\t%d\t%d\t%d\r\n", (int)ENC_PositionGet(ENC2), (int)ENC_Dateget(ENC2), (int)ENC_Revolutionget(ENC2));
        PRINTF("3\t%d\t%d\t%d\r\n", (int)ENC_PositionGet(ENC3), (int)ENC_Dateget(ENC3), (int)ENC_Revolutionget(ENC3));
        PRINTF("4\t%d\t%d\t%d\r\n", (int)ENC_PositionGet(ENC4), (int)ENC_Dateget(ENC4), (int)ENC_Revolutionget(ENC4));
		char quit;
		quit = GETCHAR();
		if (quit == 'q' || quit == 'Q') { break; }
    }
    ENC_Deinit(ENC1);
    ENC_Deinit(ENC2);
    ENC_Deinit(ENC3);
    ENC_Deinit(ENC4);
    PRINTF("\r\n");
    vTaskDelete(NULL);
}

void U_flash(void *pv) {
    PRINTF("flash测试\r\n");
    if (0 != FLASH_Init()) {
        PRINTF("flash初始化失败\r\n");
        vTaskDelete(NULL);
    }
    int addr_start = 3 * 1024 * 1024;//flash前3m存程序，后1m用来存储数据
    static uint8_t buff_r[FLASH_SECTOR_SIZE];
    static uint8_t buff_w[FLASH_SECTOR_SIZE];
    FLASH_Erase(addr_start);
    FLASH_Read(addr_start, buff_r, FLASH_SECTOR_SIZE);
    for (size_t i = 0; i < FLASH_SECTOR_SIZE; i++) {
        if (0xff != buff_r[i]) {
            PRINTF("flash擦除失败\r\n");
            vTaskDelete(NULL);
        }
    }
    for (size_t i = 0; i < FLASH_PAGE_SIZE; i++) {
        buff_w[i] = i;
    }
    for (size_t i = 0; i < FLASH_SECTOR_SIZE / FLASH_PAGE_SIZE; i++) {
        //必须先擦除扇区之后才能写入
        FLASH_Prog(addr_start + i * FLASH_PAGE_SIZE, buff_w);
    }
    FLASH_Read(addr_start, buff_r, FLASH_SECTOR_SIZE);
    for (size_t i = 0; i < FLASH_SECTOR_SIZE / FLASH_PAGE_SIZE; i++) {
        for (size_t j = 0; j < FLASH_PAGE_SIZE; j++) {
            if (buff_r[j] != j) {
                PRINTF("flash写失败\r\n");
                vTaskDelete(NULL);
            }
        }
    }
    PRINTF("flash读写正常\r\n");
    vTaskDelete(NULL);
}

void U_flash_with_lfs(void *pv) {
    PRINTF("flash rw with lfs\r\n");
    if (0 != FLASH_Init()) {
        PRINTF("flash初始化失败\r\n");
        vTaskDelete(NULL);
    }
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
    //创建文件夹
    lfs_mkdir(&lfs, "boot_count");

    // read current count
    uint32_t boot_count = 0;

    lfs_file_open(&lfs, &file, "boot_count/boot_count.bin", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_read(&lfs, &file, &boot_count, sizeof(boot_count));

    // update boot count
    boot_count += 1;
    // print the boot count
    PRINTF("%s boot_count: %d\r\n", BOARD_NAME, boot_count);

    lfs_file_rewind(&lfs, &file);
    lfs_file_write(&lfs, &file, &boot_count, sizeof(boot_count));

    // remember the storage is not updated until the file is closed successfully
    lfs_file_close(&lfs, &file);

    // release any resources we were using
    lfs_unmount(&lfs);



    vTaskDelete(NULL);
}

#ifdef TEST_PIT
volatile int pit0_stop = 0;
volatile int pit1_stop = 0;
volatile int pit2_stop = 0;
volatile int pit3_stop = 0;

void U_pit(void *pv) {
    PIT_Init2(kPIT_Chnl_0, 500);//500us
    PIT_Init2(kPIT_Chnl_1, 5 * 1000);//5ms
    PIT_Init2(kPIT_Chnl_2, 10 * 1000);//10ms
    PIT_Init2(kPIT_Chnl_3, 20 * 1000);//20ms
    NVIC_SetPriority(PIT_IRQn, 6);//设置pit中断优先级为6，优先级为6的中断可以调用RTOS中断级API
    PIT_StartTimer(PIT, kPIT_Chnl_0);
    PIT_StartTimer(PIT, kPIT_Chnl_1);
    PIT_StartTimer(PIT, kPIT_Chnl_2);
    PIT_StartTimer(PIT, kPIT_Chnl_3);
    while (1) {
        vTaskDelay(10);
        if (pit0_stop && pit1_stop && pit2_stop && pit3_stop) {
            break;
        }
    }
    vTaskDelete(NULL);
}
//中断服务函数（不准直接改名字可以用define改名字）
//注意四个pit通道共用一个中断服务函数
/*RAMFUNC_ITC*/ void PIT_IRQHandler(void) {
    /*清除中断标志位 （要用的时候解注释，通道可换）*/
    if (PIT_GetStatusFlags(PIT, kPIT_Chnl_0) == kPIT_TimerFlag) {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
        static int count = 0;
        count++;
        if (count >= 3) {
            PIT_StopTimer(PIT, kPIT_Chnl_0);
            pit0_stop = 1;
        }
    }
    if (PIT_GetStatusFlags(PIT, kPIT_Chnl_1) == kPIT_TimerFlag) {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_1, kPIT_TimerFlag);
        static int count = 0;
        count++;
        if (count >= 3) {
            PIT_StopTimer(PIT, kPIT_Chnl_1);
            pit1_stop = 1;
        }
    }
    if (PIT_GetStatusFlags(PIT, kPIT_Chnl_2) == kPIT_TimerFlag) {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_2, kPIT_TimerFlag);
        static int count = 0;
        count++;
        if (count >= 3) {
            PIT_StopTimer(PIT, kPIT_Chnl_2);
            pit2_stop = 1;
        }
    }
    if (PIT_GetStatusFlags(PIT, kPIT_Chnl_3) == kPIT_TimerFlag) {
        PIT_ClearStatusFlags(PIT, kPIT_Chnl_3, kPIT_TimerFlag);
        static int count = 0;
        count++;
        if (count >= 3) {
            PIT_StopTimer(PIT, kPIT_Chnl_3);
            pit3_stop = 1;
        }
    }
    /*中断服务函数内容*/
    __DSB();
    /*DSB--数据同步屏障
     * 作用：因为CPU时钟的主频大于IP总线时钟故会出现中断标志位还未被清掉时中断服务函数中的内容会执行完，而使得程序一遍一遍的进入中断服务函数，
     * 而DSB则是为了预防这一情况的发生。*/
}

#endif //TEST_PIT
//一个pwm_t能输出俩路pwm
pwm_t my1 = {PWM1, kPWM_Module_3, 25 * 1000, 0, 0, kPWM_HighTrue};//一个pwm_t能输出俩路pwm
pwm_t my2 = {PWM2, kPWM_Module_3, 30 * 1000, 0, 0, kPWM_HighTrue};//一个pwm_t能输出俩路pwm
pwm_t my3 = {PWM2, kPWM_Module_2, 35 * 1000, 0, 0, kPWM_HighTrue};//一个pwm_t能输出俩路pwm
pwm_t my4 = {PWM2, kPWM_Module_1, 40 * 1000, 0, 0, kPWM_HighTrue};//一个pwm_t能输出俩路pwm
gpio_t OE_B = {PWM_OE_B_GPIO, PWM_OE_B_PIN, 0};//一个pwm_t能输出俩路pwm
//一个pwm_t能输出俩路pwm
void U_pwm(void *pv) {//一个pwm_t能输出俩路pwm
    pwm_t *list[] = {//一个pwm_t能输出俩路pwm
            &my1,//一个pwm_t能输出俩路pwm
            &my2,//一个pwm_t能输出俩路pwm
            &my3,//一个pwm_t能输出俩路pwm
            &my4,//一个pwm_t能输出俩路pwm
            NULL//一个pwm_t能输出俩路pwm
    };//一个pwm_t能输出俩路pwm//一个pwm_t能输出俩路pwm
    PWM_Init2(list);//一个pwm_t能输出俩路pwm
    GPIO_Init(&OE_B);//一个pwm_t能输出俩路pwm
    my1.dutyA = 10.0;//一个pwm_t能输出俩路pwm
    my1.dutyB = 20.0;//一个pwm_t能输出俩路pwm
    my2.dutyA = 30.0;//一个pwm_t能输出俩路pwm
    my2.dutyB = 40.0;//一个pwm_t能输出俩路pwm
    my3.dutyA = 50.0;//一个pwm_t能输出俩路pwm
    my3.dutyB = 60.0;//一个pwm_t能输出俩路pwm
    my4.dutyA = 70.0;//一个pwm_t能输出俩路pwm
    my4.dutyB = 80.0;//一个pwm_t能输出俩路pwm
    PWM_Change(&my1);//一个pwm_t能输出俩路pwm
    PWM_Change(&my2);//一个pwm_t能输出俩路pwm
    PWM_Change(&my3);//一个pwm_t能输出俩路pwm
    PWM_Change(&my4);//一个pwm_t能输出俩路pwm
    GPIO_Write(&OE_B, 0);//使能74LVC245输出//一个pwm_t能输出俩路pwm
    vTaskDelete(NULL);//一个pwm_t能输出俩路pwm
}//一个pwm_t能输出俩路pwm

void U_status(void *pv) {
    TaskStatusPrint();
    APP_PrintRunFrequency(0);
    PRINTF("%dms\r\n", TimerMsGet());
    vTaskDelete(NULL);
}

void U_i2c_soft(void* pv)
{
    int addr = 0x68;
    int whoami = 0x75;
    int pwr = 0x6b;
    uint8_t val=0;
    status_t status;
	I2CS_Type iics;
	iics.delay = 100;
	iics.SDA.base = IIC_SDA_GPIO;
	iics.SDA.pin = IIC_SDA_PIN;
	iics.SCL.base = IIC_SCL_GPIO;
	iics.SCL.pin = IIC_SCL_PIN;
	I2CS_Init(&iics);
	status = I2CS_Read(&iics, addr, whoami, &val, 1);
	if (status != kStatus_Success) { PRINTF("kStatus_LPI2C_Nak\r\n"); }
	else {
		PRINTF("Addr0x%x,a Read from R0x%x; Returned Value 0x%x\r\n", addr, whoami, (int)val);
	}
	val = 0x80;
	status = I2CS_Write(&iics, addr, pwr, &val, 1);
	if (status != kStatus_Success) { PRINTF("kStatus_LPI2C_Nak\r\n"); }
	else {
		PRINTF("Addr0x%x, a Write to R0x%x with the Value 0x%x\r\n", addr, pwr, (int)val);
	}
    vTaskDelete(NULL);
}

void U_sccb_soft(void* pv)
{
    int addr = 0x21;
	int MIDH = 0X1c;
	int MIDL = 0X1d;
    int PIDH = 0X0a;
    int PIDL = 0X0b;
	int pwr = 0x12;
	uint16_t val;
	I2CS_Type iics;
	iics.delay = 200;
	iics.SDA.base = IIC_SDA_GPIO;
	iics.SDA.pin =  IIC_SDA_PIN;
	iics.SCL.base = IIC_SCL_GPIO;
	iics.SCL.pin =  IIC_SCL_PIN;
	I2CS_Init(&iics);
	val = 0x80;
	I2CS_WriteSCCB(&iics, addr, pwr, (uint8_t*)&val, 1);//复位
	vTaskDelay(100);
	val = 0;
	I2CS_ReadSCCB(&iics, addr, MIDH, (uint8_t*)&val, 1);
	val <<= 8;
	I2CS_ReadSCCB(&iics, addr, MIDL, (uint8_t*)&val, 1);
	PRINTF("ov7725MID(0X7FA2): 0x%x\r\n", (int)val);

    val = 0;
    I2CS_ReadSCCB(&iics, addr, PIDH, (uint8_t*)&val, 1);
    val <<= 8;
    I2CS_ReadSCCB(&iics, addr, PIDL, (uint8_t*)&val, 1);
    PRINTF("ov7725PID(0X7721): 0x%x\r\n", (int)val);
    vTaskDelete(NULL);
}

BSS_SDRAM_NOCACHE uint8_t zzf_buf1[752 * 480] ALIGN(64);//最大可以使用4缓存
BSS_SDRAM_NOCACHE uint8_t zzf_buf2[752 * 480] ALIGN(64);//缓存需64字节对齐，并且放在noccche区域
BSS_SDRAM_NOCACHE uint8_t zzf_buf3[752 * 480] ALIGN(64);//
BSS_SDRAM_NOCACHE uint8_t zzf_buf4[752 * 480] ALIGN(64);//
void U_zzf(void *pv) {
    PRINTF("总钻风摄像头测试\r\n");
    //先准备其他资源
    if (0 == strcmp(pv, "oled")) {
        OLED_Init();
    } else if (0 == strcmp(pv, "lcd")) {
        Lcd_Init();
    } else if (0 == strcmp(pv, "sd")) {
        if (kStatus_Success != SD_Mount()) {
            vTaskDelete(NULL);
        }
        f_mkdir("zzf");//在根目录下创建名为zzf的文件夹
    } else {
        vTaskDelete(NULL);
    }
    img_t img;
    //初始化摄像头
    UART_Init(LPUART4, 9600, 80 * 1000 * 1000);
    if (kStatus_Success != ZZF_Init(ZZF_FrameSize480x752, LPUART4)) {
        PRINTF("zzf init fail!\r\n");
        vTaskDelete(NULL);
    }
    img.format = PixelFormatGray;
    img.width = 752;
    img.height = 480;
    CAMERA_SubmitBuff(zzf_buf1);
    CAMERA_SubmitBuff(zzf_buf2);
    CAMERA_SubmitBuff(zzf_buf3);
    CAMERA_SubmitBuff(zzf_buf4);
    if (kStatus_Success != CAMERA_ReceiverStart())//开始接收摄像头传来的图像
    {
        PRINTF("zzf init fail!\r\n");
        vTaskDelete(NULL);
    }
    for (int i = 0; i < 100; i++) {
        if (kStatus_Success == CAMERA_FullBufferGet(&img.pImg)) {
            //如果成功接收一帧图像，则按情况输出到Oled,lcd，Sd卡
            if (0 == strcmp(pv, "oled")) {
                OLED_PrintPicture(&img, 100);
            } else if (0 == strcmp(pv, "lcd")) {
                LCD_PrintPicture(&img);
            } else if (0 == strcmp(pv, "sd")) {
                FIL fil;
                char line[64];
                snprintf(line, 64, "zzf/%d.bmp", i);
                f_open(&fil, line, FA_CREATE_ALWAYS | FA_WRITE);
                BMP_Save(&fil, &img);
                f_close(&fil);
                i+=10;
            }
            PRINTF("fps=%f\r\n", CAMERA_FpsGet());
            CAMERA_SubmitBuff(img.pImg);//将空缓存提交

        } else {
            i--;
            vTaskDelay(1);
        }
    }
    CAMERA_ReceiverStop();//停止传输
    CAMERA_ReceiverDeinit();//De-initialize
    vTaskDelete(NULL);
}

void U_msc(void *pv) {
    status_t status;
    status = SD_MscInit();
    if (status != kStatus_Success) { PRINTF("msc init fail\r\n"); }
    vTaskDelete(NULL);
}

void U_sd(void *pv) {
    status_t status;
    status = SD_Mount();
    if (status != kStatus_Success) { PRINTF("sd init fail\r\n"); }
    f_mkdir("txt");//创建文件夹
    char *str = "何夜无月？何处无竹柏？但少闲人如吾两人者耳。";
    FIL fil;
    f_open(&fil, "txt/txt.txt", FA_CREATE_ALWAYS | FA_WRITE);
    UINT bw;
    f_write(&fil, str, strlen(str), &bw);
    if (bw != strlen(str)) { PRINTF("sd卡容量满了or文件名不能是中文"); }
    f_close(&fil);//只有关闭的时候才是真正
    vTaskDelete(NULL);
}

void U_file_dump(void *pv) {
    status_t status;
    //初始化fatfs
    status = SD_Mount();
    if (status != kStatus_Success) {
        PRINTF("sd init fail\r\n");
        vTaskDelete(NULL);
    }
    PRINTF("flash rw with lfs\r\n");
    if (0 != FLASH_Init()) {
        PRINTF("flash初始化失败\r\n");
        vTaskDelete(NULL);
    }

    //初始化littlefs
    static lfs_t lfs;
    static struct lfs_config cfg;
    FLASH_LfsGetDefaultConfig(&cfg);
    int err = lfs_mount(&lfs, &cfg);
    // reformat if we can't mount the filesystem
    // this should only happen on the first boot
    if (err) {
        PRINTF("no littlefs\r\n");
        vTaskDelete(NULL);
    }

    static lfs_file_t lfil1;//boot_count/boot_count.bin
    static lfs_file_t lfil2;
    static FIL fil1;
    static FIL fil2;//txt/记承天寺夜游.txt
    f_mkdir("boot");
    int state_fil2 = f_open(&fil2, "txt/txt.txt", FA_READ);
    int state_fil1 = f_open(&fil1, "boot/count.bin", FA_CREATE_ALWAYS | FA_WRITE);
    lfs_mkdir(&lfs, "txt");
    int state_lfil1 = lfs_file_open(&lfs, &lfil1, "boot_count/boot_count.bin", LFS_O_RDONLY);
    int state_lfil2 = lfs_file_open(&lfs, &lfil2, "txt/txt.txt", LFS_O_RDWR | LFS_O_CREAT);
    if (0 == (state_fil2 | state_fil1 | state_lfil1 | state_lfil2)) {
        size_t s_txt = f_size(&fil2);
        size_t s_bin = lfs_file_size(&lfs, &lfil1);
        void *buf_txt = pvPortMalloc(s_txt);
        void *buf_bin = pvPortMalloc(s_bin);
        UINT bw;
        f_read(&fil2, buf_txt, s_txt, &bw);
        assert(bw == s_txt);
        lfs_file_write(&lfs, &lfil2, buf_txt, s_txt);

        lfs_file_read(&lfs, &lfil1, buf_bin, s_bin);
        f_write(&fil1, buf_bin, s_bin, &bw);
        assert(bw == s_bin);
        vPortFree(buf_txt);
        vPortFree(buf_bin);
    }
    f_close(&fil1);
    f_close(&fil2);
    lfs_file_close(&lfs, &lfil1);
    lfs_file_close(&lfs, &lfil2);
    lfs_unmount(&lfs);
    vTaskDelete(NULL);
}


