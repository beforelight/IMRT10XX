//
// Created by 17616 on 2020/3/17.
//

#ifndef IMXRT_CAMERA_ZZF_H
#define IMXRT_CAMERA_ZZF_H
#include"sc_camera_common.h"
#include"sc_uart.h"

#define ZZF_FRAME_SZIE(HEIGHT,WIDTH) (uint32_t)((HEIGHT<<16)|WIDTH)
#define ZZF_FRAME_WIDTH(size) ((uint32_t)size&0xffff)
#define ZZF_FRAME_HEIGHT(size) (((uint32_t)size&(0xffff<<16))>>16)

typedef enum _zzf_frame_size
{
	//ZZF_FrameSize120x188 = ZZF_FRAME_SZIE(120,188),//1052不支持这个分辨率，因为WIDTH要整除8
	ZZF_FrameSize120x184 = ZZF_FRAME_SZIE(120, 184),
	ZZF_FrameSize240x376 = ZZF_FRAME_SZIE(240, 376),
	ZZF_FrameSize480x752 = ZZF_FRAME_SZIE(240, 376),
}zzf_frame_size_t;

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
	/**
	 * @brief	初始化总钻风摄像头，注意初始化之后请先提交空缓存再开始传输
	 * @param  {zzf_frame_size_t} frameSize : 图像大小，在zzf_frame_size_t预定义了多个图像分辨率
	 * @param  {LPUART_Type*} LPUARTx       : 中转风使用的串口，注意，必须在外部完成初始化，波特率9600
	 * @return {status_t}                   : 返回kStatus_Success成功初始化
	 */
	status_t ZZF_Init(zzf_frame_size_t frameSize, LPUART_Type* LPUARTx);

	/**
	 * @brief	内部使用，读取摄像头现在的配置到内存
	 * @param  {void} undefined : 
	 * @return {status_t}       : 
	 */
	status_t ZZF_ConfigGet(void);

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

#endif //IMXRT_CAMERA_ZZF_H
