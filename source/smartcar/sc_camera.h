//
// Created by 17616 on 2020/3/17.
//

#ifndef IMXRT_CAMERA_COMMON_H
#define IMXRT_CAMERA_COMMON_H

#include "fsl_csi.h"

#define RGB565_R(x) ((uint8_t)((x&0xF800U)>>8) )
#define RGB565_G(x) ((uint8_t)((x&0x7E0U)>>3)  )
#define RGB565_B(x) ((uint8_t)((x&0x1FU)<<3)   )

#define RGB565(r,g,b) (uint16_t)(((r<<8)&0xF800U)|((g<<3)&0x7E0U)|((b>>3)&0x1FU))

#define CAMERA_FRAME_SZIE(HEIGHT, WIDTH) (uint32_t)((HEIGHT<<16)|WIDTH)
#define CAMERA_FRAME_WIDTH(size) ((uint32_t)size&0xffff)
#define CAMERA_FRAME_HEIGHT(size) (((uint32_t)size&(0xffff<<16))>>16)


typedef enum _camera_pixel_format
{
	PixelFormatGray = 1,//灰度
	PixelFormatRGB565 = 2,
}camera_pixel_format_t;


typedef struct __img {
	camera_pixel_format_t format;
	uint16_t height;
	uint16_t width;
	void* pImg;
}img_t;

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
	
	
	status_t CAMERA_EnableClock(void);
	
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
	 * @brief	返回目前摄像头最近几帧图片的帧率
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
