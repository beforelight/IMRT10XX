//
// Created by 17616 on 2020/3/17.
//

#ifndef IMXRT_SC_SD_H
#define IMXRT_SC_SD_H

#include "fsl_sd_disk.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

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
 * @brief   初始化sd卡并挂载fatfs
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
