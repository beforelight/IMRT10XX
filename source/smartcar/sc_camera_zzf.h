//
// Created by 17616 on 2020/3/17.
//

#ifndef IMXRT_CAMERA_ZZF_H
#define IMXRT_CAMERA_ZZF_H

#include"sc_camera_common.h"
#include"sc_uart.h"

#define ZZF_FRAME_SZIE(HEIGHT, WIDTH) (uint32_t)((HEIGHT<<16)|WIDTH)
#define ZZF_FRAME_WIDTH(size) ((uint32_t)size&0xffff)
#define ZZF_FRAME_HEIGHT(size) (((uint32_t)size&(0xffff<<16))>>16)

typedef enum _zzf_frame_size {
    //ZZF_FrameSize120x188 = ZZF_FRAME_SZIE(120,188),//1052不支持这个分辨率，因为WIDTH要整除8
    ZZF_FrameSize120x184 = ZZF_FRAME_SZIE(120, 184),
    ZZF_FrameSize240x376 = ZZF_FRAME_SZIE(240, 376),
    ZZF_FrameSize480x752 = ZZF_FRAME_SZIE(480, 752),
} zzf_frame_size_t;

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/**
 * @brief	初始化总钻风摄像头，注意初始化之后请先提交空缓存再开始传输
 * @param  {zzf_frame_size_t} frameSize : 图像大小，在zzf_frame_size_t预定义了多个图像分辨率
 * @param  {LPUART_Type*} LPUARTx       : 中转风使用的串口，注意，必须在外部完成初始化，波特率9600
 * @return {status_t}                   : 返回kStatus_Success成功初始化
 */
status_t ZZF_Init(zzf_frame_size_t frameSize, LPUART_Type *LPUARTx);

/**
 * @brief	内部使用，读取摄像头现在的配置到内存
 * @param  {void} undefined :
 * @return {status_t}       :
 */
status_t ZZF_ConfigGet(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif //IMXRT_CAMERA_ZZF_H
