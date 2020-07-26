#ifndef _OELD_H
#define _OELD_H
#include"fsl_gpio.h"
#include"pin_mux.h"
#include"board.h"
#include"sc_camera.h"

//数据命令选择线，必须接
#if defined(OLED_DC_GPIO)&&defined(OLED_DC_PIN)
#define OLED_DC_SET() GPIO_PortSet(OLED_DC_GPIO,1<<OLED_DC_PIN)
#define OLED_DC_CLR() GPIO_PortClear(OLED_DC_GPIO,1<<OLED_DC_PIN)
#else
#define OLED_DC_SET()
#define OLED_DC_CLR()
#endif // defined(OLED_DC_GPIO)&&defined(OLED_DC_PIN)

//时钟线，必须接
#if defined(OLED_D0_GPIO)&&defined(OLED_D0_PIN)
#define OLED_D0_SET() GPIO_PortSet(OLED_D0_GPIO,1<<OLED_D0_PIN)
#define OLED_D0_CLR() GPIO_PortClear(OLED_D0_GPIO,1<<OLED_D0_PIN)
#else
#define OLED_D0_SET() 
#define OLED_D0_CLR() 
#endif // defined(OLED_D0_GPIO)&&defined(OLED_D0_PIN)

//数据线，必须接
#if defined(OLED_D1_GPIO)&&defined(OLED_D1_PIN)
#define OLED_D1_SET() GPIO_PortSet(OLED_D1_GPIO,1<<OLED_D1_PIN)
#define OLED_D1_CLR() GPIO_PortClear(OLED_D1_GPIO,1<<OLED_D1_PIN)
#else
#define OLED_D1_SET() 
#define OLED_D1_CLR() 
#endif // defined(OLED_D1_GPIO)&&defined(OLED_D1_PIN)

//复位线，必须接
#if defined(OLED_RES_GPIO)&&defined(OLED_RES_PIN) 
#define OLED_RES_SET() GPIO_PortSet(OLED_RES_GPIO,1<<OLED_RES_PIN)
#define OLED_RES_CLR() GPIO_PortClear(OLED_RES_GPIO,1<<OLED_RES_PIN)
#else
#define OLED_RES_SET()
#define OLED_RES_CLR()
#endif // defined(OLED_RES_GPIO)&&defined(OLED_RES_PIN) 

//片选线，可选，不接时直接拉低
#if defined(OLED_CS_GPIO)&&defined(OLED_CS_PIN)
#define OLED_CS_SET() GPIO_PortSet(OLED_CS_GPIO,1<<OLED_CS_PIN)
#define OLED_CS_CLR() GPIO_PortClear(OLED_CS_GPIO,1<<OLED_CS_PIN)
#else
#define OLED_CS_SET()
#define OLED_CS_CLR()
#endif // defined(OLED_SC_GPIO)&&defined(OLED_SC_PIN) 

void OLED_Init(void);
void OLED_Fill(uint8_t dat);//填充颜色选择0x00 or 0xff
void OLED_WrDat(uint8_t data);
void OLED_WrCmd(uint8_t cmd);
void OLED_WrByte(uint8_t OneByte);
void OLED_PutPixel(uint8_t x, uint8_t y);
void OLED_ClrPixel(uint8_t x, uint8_t y);
void OLED_Set_Pos(uint8_t x, uint8_t y);
void OLED_P6x8Str(uint8_t x, uint8_t y, uint8_t ch[]);
void OLED_P8x16Str(uint8_t x, uint8_t y, uint8_t ch[]);
void OLED_HEXACSII(uint16_t hex, uint8_t* Print);
void OLED_Print_Num(uint8_t x, uint8_t y, uint16_t num);
void OLED_Print_Num1(uint8_t x, uint8_t y, int16_t num);
void dis_bmp(uint16_t high, uint16_t width, uint8_t* p, uint8_t value);
void image_size_half_forOLED(uint8_t* src, uint8_t* dst, const int row, const int col);
void Str_Clr(uint32_t x, uint32_t y, uint32_t num);
void OLED_Logo(void);
void OLED_P6x8Rst(uint8_t x, uint8_t y, uint8_t ch[]);
void OLED_PrintPicture(img_t* src, uint8_t threshold);

/********************************************************************/

#endif
