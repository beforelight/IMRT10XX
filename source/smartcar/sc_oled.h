/*********************************************************************************************************************
Smartcar OLED显示底层
制作人：王学嘉
联系方式：18846446962
qq：512924543
备注：哈尔滨工业大学智能车创新俱乐部专用，请勿泄露，感谢逐飞科技
	  OLED接线有多种方式，想要自己开发的同学请自行查询芯片手册
					OLED接线定义：
					------------------------------------
					OLED液晶      单片机
						PTD0          A15
						PTD1          A16
						RES         A14
						DC          B16
					------------------------------------
 ********************************************************************************************************************/

#ifndef _OELD_H
#define _OELD_H
#include"fsl_gpio.h"
#include"pin_mux.h"
#include"sc_camera_common.h"

#ifndef OLED_DC_GPIO
#define OLED_DC_GPIO GPIO2 //数据命令选择线
#define OLED_DC_PIN 13U
#endif // !OLED_DC_GPIO

#ifndef OLED_D0_GPIO
#define OLED_D0_GPIO GPIO2 //时钟线
#define OLED_D0_PIN 12U
#endif // !OLED_D0_GPIO

#ifndef OLED_D1_GPIO
#define OLED_D1_GPIO GPIO2 //数据线
#define OLED_D1_PIN 0U
#endif // !OLED_D1_GPIO

#if defined(OLED_RES_GPIO)&&defined(OLED_RES_PIN) //复位线
#define OLED_RES_SET() GPIO_PortSet(OLED_RES_GPIO,1<<OLED_RES_PIN)
#define OLED_RES_CLR() GPIO_PortClear(OLED_RES_GPIO,1<<OLED_RES_PIN)
#endif // OLED_RES_GPIO

#if defined(OLED_SC_GPIO)&&defined(OLED_SC_PIN) //片选线
#define OLED_SC_SET() GPIO_PortSet(OLED_SC_GPIO,1<<OLED_SC_PIN)
#define OLED_SC_CLR() GPIO_PortClear(OLED_SC_GPIO,1<<OLED_SC_PIN)
#endif // OLED_SC_GPIO


#define OLED_DC_SET() GPIO_PortSet(OLED_DC_GPIO,1<<OLED_DC_PIN)
#define OLED_DC_CLR() GPIO_PortClear(OLED_DC_GPIO,1<<OLED_DC_PIN)
#define OLED_D0_SET() GPIO_PortSet(OLED_D0_GPIO,1<<OLED_D0_PIN)
#define OLED_D0_CLR() GPIO_PortClear(OLED_D0_GPIO,1<<OLED_D0_PIN)
#define OLED_D1_SET() GPIO_PortSet(OLED_D1_GPIO,1<<OLED_D1_PIN)
#define OLED_D1_CLR() GPIO_PortClear(OLED_D1_GPIO,1<<OLED_D1_PIN)

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
