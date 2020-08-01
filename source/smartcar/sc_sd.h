//
// Created by 17616 on 2020/3/17.
//

#ifndef IMXRT_SC_SD_H
#define IMXRT_SC_SD_H

#include "fsl_sd_disk.h"
#include "core_cm7.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
    extern volatile int _SD_ICacheIsDisable;
    extern volatile int _SD_DCacheIsDisable;
    extern volatile int _SD_CriticalNesting;
    __STATIC_FORCEINLINE void SD_EnterCritical(void) {
        if (_SD_CriticalNesting <= 0) {
            //PRINTF("SD_EnterCritical\r\n");
            if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR)) {
                SCB_DisableICache();
                _SD_ICacheIsDisable = 1;
            }
            if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR)) {
                SCB_DisableDCache();
                _SD_DCacheIsDisable = 1;
            }
            ARM_MPU_Disable();
        }
        _SD_CriticalNesting++;
    }
    __STATIC_FORCEINLINE void SD_ExitCritical(void) {
        _SD_CriticalNesting--;
        if (_SD_CriticalNesting <= 0) {
            //PRINTF("SD_ExitCritical\r\n");
            ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);
            if (_SD_ICacheIsDisable) {
                SCB_EnableDCache();
                _SD_ICacheIsDisable = 0;
            }
            if (_SD_DCacheIsDisable) {
                SCB_EnableICache();
                _SD_DCacheIsDisable = 0;
            }
        }
    }

/**
 * @brief   SD卡检测
 * @param  {void} undefined : 
 * @return {status_t}       : kStatus_Success为检测到
 */
status_t SD_CardDetect(void);

/**
 * @brief   初始化sd卡
 * @param  {void} undefined : 
 * @return {status_t}       : kStatus_Success为成功初始化
 */
status_t SD_Init2(void);

/**
 * @brief   初始化sd卡并挂载fatfs，sd卡将会挂载到2:/路径下
 * @param  {void} undefined : 
 * @return {status_t}       : kStatus_Success表示成功
 */
status_t SD_Mount(void);

/**
 * @brief   初始化sd卡挂载为usb大容量存储设备
 * @param  {void} undefined : 
 * @return {status_t}       : kStatus_Success表示成功
 */
status_t SD_MscInit(void);
#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif //IMXRT_SC_SD_H
