//
// Created by 17616 on 2020/3/17.
// 更新于2020/8/3

#ifndef IMXRT_CAMERA_OV7725_H
#define IMXRT_CAMERA_OV7725_H

#include"sc_camera.h"
#include"sc_i2cs.h"
#include"video/fsl_camera.h"
#include"video/fsl_camera_device.h"
#include"LQ_OV7725M.h"
typedef enum _ov7725_frame_size {
    OV7725_FrameSizeVGA480x640 = CAMERA_FRAME_SZIE(480, 640),
    OV7725_FrameSizeQVGA240x320 = CAMERA_FRAME_SZIE(240, 320),
} ov7725_frame_size_t;




#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
    status_t OV7725_Init2(ov7725_frame_size_t size, I2CS_Type* base);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif //IMXRT_CAMERA_OV7725_H
