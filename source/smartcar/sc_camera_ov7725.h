//
// Created by 17616 on 2020/3/17.
//

#ifndef IMXRT_CAMERA_OV7725_H
#define IMXRT_CAMERA_OV7725_H

#include"sc_camera_common.h"
#define OV_FRAME_SZIE(HEIGHT,WIDTH) (uint32_t)((HEIGHT<<16)|WIDTH)
#define OV_FRAME_WIDTH(size) ((uint32_t)size&0xffff)
#define OV_FRAME_HEIGHT(size) (((uint32_t)size&(0xffff<<16))>>16)

typedef enum _ov7725_frame_size
{
    OV7725_FrameSizeVGA480x640 = OV_FRAME_SZIE(480, 640),
    OV7725_FrameSizeQVGA240x320 = OV_FRAME_SZIE(240, 320),
}ov7725_frame_size_t;

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

    //sccb接口，记得修改成适合自己板子的
    uint8_t OV7725_SCCB_WR_Reg(uint8_t reg, uint8_t data);
    uint8_t OV7725_SCCB_RD_Reg(uint8_t reg);
    void    OV7725_SCCB_Init(void);

    status_t   OV7725_Init(ov7725_frame_size_t size);
    void OV7725_Light_Mode(uint8_t mode);
    void OV7725_Color_Saturation(int8_t sat);
    void OV7725_Brightness(int8_t bright);
    void OV7725_Contrast(int8_t contrast);
    void OV7725_Special_Effects(uint8_t eft);


    /**
     * @brief
     * @param  {void} undefined : 
     * @return {status_t}       : 
     */
	status_t CAMERA_ReceiverDeinit(void);

    /**
     * @brief	提交一帧图像的空缓存，如果没有缓存，传输会自动停止，最大4帧缓存
     * @brief	注意缓存所在的内存区域不能启用cache
     * @brief	注意缓存必须64字节对齐
     * @param  {void*} buff : 缓存指针
     * @return {status_t}   : kStatus_Success为成功提交
     */
	status_t CAMERA_SubmitBuff(void* buff);

    /**
     * @brief	获取采集好图像的缓存指针
     * @param  {void*} buff : 有图像的缓存指针
     * @return {status_t}   : kStatus_Success为成功获取一帧图像缓存
     */
	status_t CAMERA_FullBufferGet(void* buff);

    /**
     * @brief	开始传输图像，开始传输之前确保已经提交足够的空缓存
     * @param  {void} undefined : 
     * @return {status_t}       : kStatus_Success为成功开始
     */
	status_t CAMERA_ReceiverStart(void);

    /**
     * @brief	停止传输
     * @param  {void} undefined : 
     * @return {status_t}       : 
     */
	status_t CAMERA_ReceiverStop(void);

    /**
     * @brief	返回目前摄像头的帧率，原理是统计最近60张图像所用的时间
     * @brief	会阻塞大概1-2秒
     * @param  {void} undefined : 
     * @return {float}          :fps 
     */
	float CAMERA_FpsGet(void);


    /**
     * @brief	传输完成回调函数
     * @param  {CSI_Type*} base       : 
     * @param  {csi_handle_t*} handle : 
     * @param  {status_t} status      : 
     * @param  {void*} userData       : 
     */
	void CAMERA_Callback(CSI_Type* base, csi_handle_t* handle, status_t status, void* userData);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif //IMXRT_CAMERA_OV7725_H
