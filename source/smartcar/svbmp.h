#ifndef __SVBMP_H__
#define __SVBMP_H__
#include <stdint.h>
#ifdef ON_WINDOWS
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
#include <stdio.h>
#define FIL FILE
#else
#include "sc_camera.h"
#include <fatfs/source/ff.h>
#endif // ON_WINDOWS

int BMP_FileWrite(FIL* fp, const void* buff, uint32_t btw);
int BMP_Save(FIL* fp, img_t* img);
#endif //__SVBMP_H__