#ifndef __DRV_LCD_H__
#define __DRV_LCD_H__
#ifndef __LCD_H
#define __LCD_H			  	 

#include "fsl_gpio.h"
#include "sc_camera.h"
#include "pin_mux.h"
#include "board.h"

#define USE_HORIZONTAL 0  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏

#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 240
#else
#define LCD_W 240
#define LCD_H 240
#endif

#if defined(LCD_WR_GPIO)&&defined(LCD_WR_PIN)
#define LCD_WR_CLR GPIO_PortClear(LCD_WR_GPIO,1<<LCD_WR_PIN)
#define LCD_WR_SET GPIO_PortSet(LCD_WR_GPIO,1<<LCD_WR_PIN)
#else
#define LCD_WR_CLR
#define LCD_WR_SET
#endif // defined(LCD_WR_GPIO)&&defined(LCD_WR_PIN)

#if defined(LCD_DC_GPIO)&&defined(LCD_DC_PIN)
#define LCD_DC_SET GPIO_PortSet(LCD_DC_GPIO,1<<LCD_DC_PIN)
#define LCD_DC_CLR GPIO_PortClear(LCD_DC_GPIO,1<<LCD_DC_PIN) 
#else
#define LCD_DC_SET
#define LCD_DC_CLR
#endif // defined(LCD_DC_GPIO)&&defined(LCD_DC_PIN)


#if defined(LCD_D0_GPIO)&&defined(LCD_D0_PIN)
/******************************************************************************
	  函数说明：LCD并行数据写入函数
	  入口数据：dat  要写入的并行数据
	  返回值：  无
******************************************************************************/
static inline void LCD_Writ_Bus(uint8_t dat){
	LCD_D0_GPIO->DR_SET = dat << LCD_D0_PIN;
	LCD_WR_CLR;
	LCD_D0_GPIO->DR_CLEAR = ((~dat) & 0xff) << LCD_D0_PIN;
	LCD_WR_SET;//上升沿采样
}
#else
static inline void LCD_Writ_Bus(uint8_t dat) {
}
#endif // defined(LCD_D0_GPIO)&&defined(LCD_D0_PIN)

extern  uint16_t BACK_COLOR;   //背景色

void LCD_WR_DATA8(uint8_t dat);
void LCD_WR_DATA(uint16_t dat);
void LCD_WR_REG(uint8_t dat);
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void Lcd_Init(void);
void LCD_Clear(uint16_t Color);
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color);
void LCD_DrawPoint_big(uint16_t x, uint16_t y, uint16_t colory);
void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint8_t mode, uint16_t color);
void LCD_ShowString(uint16_t x, uint16_t y, const char* p, uint16_t color);
uint32_t mypow(uint8_t m, uint8_t n);
void LCD_ShowNum(uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t color);
void LCD_ShowNum1(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t color);
void LCD_PrintPicture(img_t* src);

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE           	 0x001F  
#define BRED             0XF81F
#define GRED 			       0XFFE0
#define GBLUE			       0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 //棕色
#define BRRED 			     0XFC07 //棕红色
#define GRAY  			     0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 

#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			     0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)




#endif  /*__LCD_H*/
#endif /* __DRV_LCD_H__ */