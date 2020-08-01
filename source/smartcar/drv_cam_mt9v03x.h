#ifndef _DRV_CAM_MT9V03X_H_
#define _DRV_CAM_MT9V03X_H_
#include "fsl_common.h"
#include "sc_camera.h"
#include "sc_i2cs.h"
//#include "drv_cam_mt9v03x_port.h"
#define CAM_MT9V034_I2CADDR (0x48)

typedef enum _mt9v03x_frame_size {
	MT9V03X_FrameSize120x184 = CAMERA_FRAME_SZIE(120, 184),
	//MT9V03X_FrameSize240x376 = CAMERA_FRAME_SZIE(240, 376),
	//MT9V03X_FrameSize480x752 = CAMERA_FRAME_SZIE(480, 752),
} mt9v03x_frame_size_t;

static I2CS_Type* _mt9v03x_i2cs;

static inline status_t CAM_MT9V03X_I2C_RxWord(uint8_t _addr, uint16_t* _data)
{
	return I2CS_Read(_mt9v03x_i2cs, CAM_MT9V034_I2CADDR, _addr, (uint8_t*)_data, 2);
}
static inline status_t CAM_MT9V03X_I2C_TxWord(uint8_t _addr, uint16_t* _data)
{
	return I2CS_Write(_mt9v03x_i2cs, CAM_MT9V034_I2CADDR, _addr, (uint8_t*)_data, 2);
}

status_t MT9V034_DataInit(I2CS_Type* base);





#endif // ! _DRV_CAM_MT9V03X_H_