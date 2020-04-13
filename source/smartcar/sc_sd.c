//
// Created by 17616 on 2020/3/17.
//
#include "fsl_debug_console.h"
#include "sc_sd.h"
#include "board.h"
#include "clock_config.h"
#if !defined(BOARD_SD_SUPPORT_180V) || BOARD_SD_SUPPORT_180V
#error 在board.h中定义“ #define BOARD_SD_SUPPORT_180V 0U#define BOARD_SD_SUPPORT_180V 0U”飞凌核心板不支持1.8v模式
#endif
static FATFS g_fileSystem; /* File system object */
status_t SD_CardDetect(void) {
    BOARD_USDHC_CD_GPIO_INIT();
    if (0 == BOARD_USDHC_CD_STATUS()) {
        return kStatus_Success;
    } else {
        return kStatus_Fail;
    }
}


static const sdmmchost_detect_card_t s_sdCardDetect = {
#ifndef BOARD_SD_DETECT_TYPE
        .cdType = kSDMMCHOST_DetectCardByGpioCD,
#else
        .cdType = BOARD_SD_DETECT_TYPE,
#endif
        .cdTimeOut_ms = 0,
};

void voidfunc(void) {
    __asm volatile ("nop");
    return;
}

static const sdmmchost_pwr_card_t s_sdCardPwrCtrl = {
        .powerOn = voidfunc,
        .powerOnDelay_ms = 0U,
        .powerOff = voidfunc,
        .powerOffDelay_ms = 0U,
};

uint32_t USDHC1_ClkFreqGet(void) {
    uint32_t freq;
    int div = CLOCK_GetDiv(kCLOCK_Usdhc1Div);
    int mux = CLOCK_GetMux(kCLOCK_Usdhc1Mux);
    if (mux == 1) {
        freq = CLOCK_GetSysPfdFreq(kCLOCK_Pfd0);
        freq /= (div + 1);
    } else if (mux == 0) {
        freq = CLOCK_GetSysPfdFreq(kCLOCK_Pfd2);
        freq /= (div + 1);
    }
    assert(freq);
    return freq;
}

status_t SD_Init2(void) {
    status_t status;
    status = SD_CardDetect();
    if (status != kStatus_Success) { return status; }

    /* Save host information. */
    if (g_sd.flags) { return kStatus_Success; }
    g_sd.host.base = USDHC1;
    g_sd.host.sourceClock_Hz = USDHC1_ClkFreqGet();
    /* card detect type */
    g_sd.usrParam.cd = &s_sdCardDetect;
    /*! @brief SDMMC card power control configuration */
    g_sd.usrParam.pwr = &s_sdCardPwrCtrl;
    /* SD host init function */
    if (SD_HostInit(&g_sd) != kStatus_Success) {
        PRINTF("\r\nSD host init fail\r\n");
        return kStatus_Fail;
    }
    /* wait card insert */
    if (SD_WaitCardDetectStatus(SD_HOST_BASEADDR, &s_sdCardDetect, true) == kStatus_Success) {
        PRINTF("\r\nCard inserted.\r\n");
    } else {
        PRINTF("\r\nCard detect fail.\r\n");
        return kStatus_Fail;
    }
    return SD_Init(&g_sd);
}

static const TCHAR driverNumberBuffer[3U] = {SDDISK + '0', ':', '/'};

status_t SD_Mount(void) {
    status_t status;
    status = SD_Init2();
    if (status != kStatus_Success) { return status; }

    if (f_mount(&g_fileSystem, driverNumberBuffer, 0U)) {
        PRINTF("Mount volume failed.\r\n");
        return -1;
    }
#if (FF_FS_RPATH >= 2U)
    int error = f_chdrive((char const *) &driverNumberBuffer[0U]);
    if (error) {
        PRINTF("Change drive failed.\r\n");
        return -1;
    }
#endif
    return kStatus_Success;
}

extern void USB_DeviceApplicationInit(void);

status_t SD_MscInit(void) {
    PRINTF("Please insert SD card\r\n");
    status_t status;
    status = SD_Init2();
    if (status != kStatus_Success) { return status; }

    USB_DeviceApplicationInit();
    return kStatus_Success;
}
