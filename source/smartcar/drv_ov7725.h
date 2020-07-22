//
// Created by 17616 on 2020/3/17.
//

#ifndef IMXRT_CAMERA_OV7725_H
#define IMXRT_CAMERA_OV7725_H

#include"sc_camera.h"

typedef enum _ov7725_frame_size {
    OV7725_FrameSizeVGA480x640 = CAMERA_FRAME_SZIE(480, 640),
    OV7725_FrameSizeQVGA240x320 = CAMERA_FRAME_SZIE(240, 320),
} ov7725_frame_size_t;

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

//sccb接口，记得修改成适合自己板子的
uint8_t OV7725_SCCB_WR_Reg(uint8_t reg, uint8_t data);

uint8_t OV7725_SCCB_RD_Reg(uint8_t reg);

void OV7725_SCCB_Init(void);

status_t OV7725_Init(ov7725_frame_size_t size);

void OV7725_Light_Mode(uint8_t mode);

void OV7725_Color_Saturation(int8_t sat);

void OV7725_Brightness(int8_t bright);

void OV7725_Contrast(int8_t contrast);

void OV7725_Special_Effects(uint8_t eft);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif //IMXRT_CAMERA_OV7725_H
