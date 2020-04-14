//
// Created by 17616 on 2020/3/17.
//

#ifndef IMXRT_CAMERA_COMMON_H
#define IMXRT_CAMERA_COMMON_H

#include "fsl_csi.h"

typedef enum _camera_pixel_format
{
	PixelFormatGray = 1,//灰度
	PixelFormatRGB565 = 2,
}camera_pixel_format_t ;

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
    /**
     * @brief	设置输出像素时钟，默认24m
     * @param  {uint32_t} clk : 时钟
     * @return {status_t}     : kStatus_Success为成功设置
     */
	status_t CAMERA_MclkSet(uint32_t clk);

    /**
     * @brief	初始化CSI
     * @brief	这是写摄像头驱动的接口
     * @param  {csi_config_t*} csi_config : 
     * @return {status_t}                 : 
     */
	status_t CAMERA_ReceiverInit(const csi_config_t* csi_config);

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

#endif //IMXRT_CAMERA_COMMON_H