/*
 * sc_lcd.c
 *
 *  Created on: 2020年1月21日
 *      Author: 17616
 */
#include "drv_lcd.h"
#include "stdint.h"
#include "stdlib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "board.h"
uint16_t BACK_COLOR;   //背景色
/******************************************************************************
	  函数说明：LCD并行数据写入函数
	  入口数据：dat  要写入的并行数据
	  返回值：  无
******************************************************************************/
inline void LCD_Writ_Bus(uint8_t dat)
{
	LCD_D0_GPIO->DR_SET = dat << LCD_D0_PIN;
	//如果LCD_DATA_LINE_GPIO==LCD_WR_GPIO
//	LCD_DATA_LINE_GPIO->DR_CLEAR = (((~dat) & 0xff )<< LCD_DATA_LINE_PIN_START )| (1 << IO8080_WR_PIN);
	//否则
	LCD_WR_CLR;
	LCD_D0_GPIO->DR_CLEAR = ((~dat) & 0xff) << LCD_D0_PIN;
	LCD_WR_SET;//上升沿采样
}




/******************************************************************************
	  函数说明：LCD写入数据
	  入口数据：dat 写入的数据
	  返回值：  无
******************************************************************************/
inline void LCD_WR_DATA8(uint8_t dat)
{
	LCD_Writ_Bus(dat);
}


/******************************************************************************
	  函数说明：LCD写入数据
	  入口数据：dat 写入的数据
	  返回值：  无
******************************************************************************/
inline void LCD_WR_DATA(uint16_t dat)
{
	LCD_Writ_Bus(dat >> 8);
	LCD_Writ_Bus(dat);
}


/******************************************************************************
	  函数说明：LCD写入命令
	  入口数据：dat 写入的命令
	  返回值：  无
******************************************************************************/
inline void LCD_WR_REG(uint8_t dat)
{
	LCD_DC_CLR;//写命令
	LCD_Writ_Bus(dat);
	LCD_DC_SET;//写数据
}


/******************************************************************************
	  函数说明：设置起始和结束地址
	  入口数据：x1,x2 设置列的起始和结束地址
				y1,y2 设置行的起始和结束地址
	  返回值：  无
******************************************************************************/
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
#if (USE_HORIZONTAL == 0)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
#elif (USE_HORIZONTAL == 1)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1 + 80);
		LCD_WR_DATA(y2 + 80);
		LCD_WR_REG(0x2c);//储存器写
	}
#elif (USE_HORIZONTAL == 2)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
#else
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1 + 80);
		LCD_WR_DATA(x2 + 80);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
#endif
}

/******************************************************************************
	  函数说明：LCD初始化函数
	  入口数据：无
	  返回值：  无
******************************************************************************/
void Lcd_Init(void)
{
	vTaskDelay(10); //等待上电复位
	//************* Start Initial Sequence **********// 
	LCD_WR_REG(0x36);
	if (USE_HORIZONTAL == 0)LCD_WR_DATA8(0x00);
	else if (USE_HORIZONTAL == 1)LCD_WR_DATA8(0xC0);
	else if (USE_HORIZONTAL == 2)LCD_WR_DATA8(0x70);
	else LCD_WR_DATA8(0xA0);

	LCD_WR_REG(0x3A);
	LCD_WR_DATA8(0x05);

	LCD_WR_REG(0xB2);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x33);

	LCD_WR_REG(0xB7);
	LCD_WR_DATA8(0x35);

	LCD_WR_REG(0xBB);
	LCD_WR_DATA8(0x19);

	LCD_WR_REG(0xC0);
	LCD_WR_DATA8(0x2C);

	LCD_WR_REG(0xC2);
	LCD_WR_DATA8(0x01);

	LCD_WR_REG(0xC3);
	LCD_WR_DATA8(0x12);

	LCD_WR_REG(0xC4);
	LCD_WR_DATA8(0x20);

	LCD_WR_REG(0xC6);
	LCD_WR_DATA8(0x0F);

	LCD_WR_REG(0xD0);
	LCD_WR_DATA8(0xA4);
	LCD_WR_DATA8(0xA1);

	LCD_WR_REG(0xE0);
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x2B);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x54);
	LCD_WR_DATA8(0x4C);
	LCD_WR_DATA8(0x18);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x0B);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x23);

	LCD_WR_REG(0xE1);
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x11);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x2C);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x44);
	LCD_WR_DATA8(0x51);
	LCD_WR_DATA8(0x2F);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x20);
	LCD_WR_DATA8(0x23);

	LCD_WR_REG(0x21);

	LCD_WR_REG(0x11);
	//vTaskDelay(10); 

	LCD_WR_REG(0x29);
}


/******************************************************************************
	  函数说明：LCD清屏函数
	  入口数据：无
	  返回值：  无
******************************************************************************/
void LCD_Clear(uint16_t Color)
{
	uint16_t i, j;
	LCD_Address_Set(0, 0, LCD_W - 1, LCD_H - 1);
	for (i = 0; i < LCD_W; i++)
	{
		for (j = 0; j < LCD_H; j++)
		{
			LCD_WR_DATA(Color);
		}

	}
}


///******************************************************************************
//	  函数说明：LCD显示汉字
//	  入口数据：x,y   起始坐标
//				index 汉字的序号
//				size  字号
//	  返回值：  无
//******************************************************************************/
//void LCD_ShowChinese(uint16_t x, uint16_t y, uint8_t index, uint8_t size, uint16_t color)
//{
//	uint8_t i, j;
//	const uint8_t* temp;
//	uint8_t size1;
//	if (size == 16) { temp = Hzk16; }//选择字号
//	if (size == 32) { temp = Hzk32; }
//	LCD_Address_Set(x, y, x + size - 1, y + size - 1); //设置一个汉字的区域
//	size1 = size * size / 8;//一个汉字所占的字节
//	temp += index * size1;//写入的起始位置
//	for (j = 0; j < size1; j++)
//	{
//		for (i = 0; i < 8; i++)
//		{
//			if ((*temp & (1 << i)) != 0)//从数据的低位开始读
//			{
//				LCD_WR_DATA(color);//点亮
//			}
//			else
//			{
//				LCD_WR_DATA(BACK_COLOR);//不点亮
//			}
//		}
//		temp++;
//	}
//}


/******************************************************************************
	  函数说明：LCD显示汉字
	  入口数据：x,y   起始坐标
	  返回值：  无
******************************************************************************/
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color)
{
	LCD_Address_Set(x, y, x, y);//设置光标位置 
	LCD_WR_DATA(color);
}


/******************************************************************************
	  函数说明：LCD画一个大的点
	  入口数据：x,y   起始坐标
	  返回值：  无
******************************************************************************/
void LCD_DrawPoint_big(uint16_t x, uint16_t y, uint16_t color)
{
	LCD_Fill(x - 1, y - 1, x + 1, y + 1, color);
}


/******************************************************************************
	  函数说明：在指定区域填充颜色
	  入口数据：xsta,ysta   起始坐标
				xend,yend   终止坐标
	  返回值：  无
******************************************************************************/
void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
	uint16_t i, j;
	LCD_Address_Set(xsta, ysta, xend, yend);      //设置光标位置 
	for (i = ysta; i <= yend; i++)
	{
		for (j = xsta; j <= xend; j++)LCD_WR_DATA(color);//设置光标位置 	    
	}
}


/******************************************************************************
	  函数说明：画线
	  入口数据：x1,y1   起始坐标
				x2,y2   终止坐标
	  返回值：  无
******************************************************************************/
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	uint16_t t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	delta_x = x2 - x1; //计算坐标增量 
	delta_y = y2 - y1;
	uRow = x1;//画线起点坐标
	uCol = y1;
	if (delta_x > 0)incx = 1; //设置单步方向 
	else if (delta_x == 0)incx = 0;//垂直线 
	else { incx = -1; delta_x = -delta_x; }
	if (delta_y > 0)incy = 1;
	else if (delta_y == 0)incy = 0;//水平线 
	else { incy = -1; delta_y = -delta_x; }
	if (delta_x > delta_y)distance = delta_x; //选取基本增量坐标轴 
	else distance = delta_y;
	for (t = 0; t < distance + 1; t++)
	{
		LCD_DrawPoint(uRow, uCol, color);//画点
		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance)
		{
			xerr -= distance;
			uRow += incx;
		}
		if (yerr > distance)
		{
			yerr -= distance;
			uCol += incy;
		}
	}
}


/******************************************************************************
	  函数说明：画矩形
	  入口数据：x1,y1   起始坐标
				x2,y2   终止坐标
	  返回值：  无
******************************************************************************/
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	LCD_DrawLine(x1, y1, x2, y1, color);
	LCD_DrawLine(x1, y1, x1, y2, color);
	LCD_DrawLine(x1, y2, x2, y2, color);
	LCD_DrawLine(x2, y1, x2, y2, color);
}


/******************************************************************************
	  函数说明：画圆
	  入口数据：x0,y0   圆心坐标
				r       半径
	  返回值：  无
******************************************************************************/
void LCD_Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color)
{
	int a, b;
	//	int di;
	a = 0; b = r;
	while (a <= b)
	{
		LCD_DrawPoint(x0 - b, y0 - a, color);             //3           
		LCD_DrawPoint(x0 + b, y0 - a, color);             //0           
		LCD_DrawPoint(x0 - a, y0 + b, color);             //1                
		LCD_DrawPoint(x0 - a, y0 - b, color);             //2             
		LCD_DrawPoint(x0 + b, y0 + a, color);             //4               
		LCD_DrawPoint(x0 + a, y0 - b, color);             //5
		LCD_DrawPoint(x0 + a, y0 + b, color);             //6 
		LCD_DrawPoint(x0 - b, y0 + a, color);             //7
		a++;
		if ((a * a + b * b) > (r * r))//判断要画的点是否过远
		{
			b--;
		}
	}
}


/******************************************************************************
	  函数说明：显示字符
	  入口数据：x,y    起点坐标
				num    要显示的字符
				mode   1叠加方式  0非叠加方式
	  返回值：  无
******************************************************************************/
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint8_t mode, uint16_t color)
{
	uint8_t temp;
	uint8_t pos, t;
	uint16_t x0 = x;
	if (x > LCD_W - 16 || y > LCD_H - 16)return;	    //设置窗口		   
	num = num - ' ';//得到偏移后的值
	LCD_Address_Set(x, y, x + 8 - 1, y + 16 - 1);      //设置光标位置 
	if (!mode) //非叠加方式
	{
		for (pos = 0; pos < 16; pos++)
		{
			temp = asc2_1608[(uint16_t)num * 16 + pos];		 //调用1608字体
			for (t = 0; t < 8; t++)
			{
				if (temp & 0x01)LCD_WR_DATA(color);
				else LCD_WR_DATA(BACK_COLOR);
				temp >>= 1;
				x++;
			}
			x = x0;
			y++;
		}
	}
	else//叠加方式
	{
		for (pos = 0; pos < 16; pos++)
		{
			temp = asc2_1608[(uint16_t)num * 16 + pos];		 //调用1608字体
			for (t = 0; t < 8; t++)
			{
				if (temp & 0x01)LCD_DrawPoint(x + t, y + pos, color);//画一个点     
				temp >>= 1;
			}
		}
	}
}


/******************************************************************************
	  函数说明：显示字符串
	  入口数据：x,y    起点坐标
				*p     字符串起始地址
	  返回值：  无
******************************************************************************/
void LCD_ShowString(uint16_t x, uint16_t y, const char* p, uint16_t color)
{
	while (*p != '\0')
	{
		if (x > LCD_W - 16) { x = 0; y += 16; }
		if (y > LCD_H - 16) { y = x = 0; LCD_Clear(RED); }
		LCD_ShowChar(x, y, *p, 0, color);
		x += 8;
		p++;
	}
}


/******************************************************************************
	  函数说明：显示数字
	  入口数据：m底数，n指数
	  返回值：  无
******************************************************************************/
uint32_t mypow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;
	while (n--)result *= m;
	return result;
}


/******************************************************************************
	  函数说明：显示数字
	  入口数据：x,y    起点坐标
				num    要显示的数字
				len    要显示的数字个数
	  返回值：  无
******************************************************************************/
void LCD_ShowNum(uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t color)
{
	uint8_t t, temp;
	uint8_t enshow = 0;
	for (t = 0; t < len; t++)
	{
		temp = (num / mypow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1))
		{
			if (temp == 0)
			{
				LCD_ShowChar(x + 8 * t, y, ' ', 0, color);
				continue;
			}
			else enshow = 1;

		}
		LCD_ShowChar(x + 8 * t, y, temp + 48, 0, color);
	}
}


/******************************************************************************
	  函数说明：显示小数
	  入口数据：x,y    起点坐标
				num    要显示的小数
				len    要显示的数字个数
	  返回值：  无
******************************************************************************/
void LCD_ShowNum1(uint16_t x, uint16_t y, float num, uint8_t len, uint16_t color)
{
	uint8_t t, temp;
	//	uint8_t enshow = 0;
	uint16_t num1;
	num1 = num * 100;
	for (t = 0; t < len; t++)
	{
		temp = (num1 / mypow(10, len - t - 1)) % 10;
		if (t == (len - 2))
		{
			LCD_ShowChar(x + 8 * (len - 2), y, '.', 0, color);
			t++;
			len += 1;
		}
		LCD_ShowChar(x + 8 * t, y, temp + 48, 0, color);
	}
}


///******************************************************************************
//	  函数说明：显示40x40图片
//	  入口数据：x,y    起点坐标
//	  返回值：  无
//******************************************************************************/
//void LCD_ShowPicture(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
//{
//	int i;
//	LCD_Address_Set(x1, y1, x2, y2);
//	for (i = 0; i < 1600; i++)
//	{
//		LCD_WR_DATA8(image[i * 2 + 1]);
//		LCD_WR_DATA8(image[i * 2]);
//	}
//}
//
//void LCD_Test(void)
//{
//	uint8_t i, m;
////	float t = 0;
//	Lcd_Init();			//初始化OLED
//	LCD_Clear(WHITE);
//	BACK_COLOR = WHITE;
//	while (1)
//	{
//		LCD_Clear(WHITE);
//		LCD_ShowChinese(10, 0, 0, 32, RED);   //中
//		LCD_ShowChinese(45, 0, 1, 32, RED);   //景
//		LCD_ShowChinese(80, 0, 2, 32, RED);   //园
//		LCD_ShowChinese(115, 0, 3, 32, RED);  //电
//		LCD_ShowChinese(150, 0, 4, 32, RED);  //子
//
//		LCD_ShowChinese(10, 75, 0, 16, RED);   //中
//		LCD_ShowChinese(45, 75, 1, 16, RED);   //景
//		LCD_ShowChinese(80, 75, 2, 16, RED);   //园
//		LCD_ShowChinese(115, 75, 3, 16, RED);  //电
//		LCD_ShowChinese(150, 75, 4, 16, RED);  //子
//		LCD_ShowString(10, 35, "2.4 TFT SPI 240*320", RED);
//		LCD_ShowString(10, 55, "LCD_W:", RED);	LCD_ShowNum(70, 55, LCD_W, 3, RED);
//		LCD_ShowString(110, 55, "LCD_H:", RED); LCD_ShowNum(160, 55, LCD_H, 3, RED);
//		for (i = 0; i < 5; i++)
//		{
//			for (m = 0; m < 6; m++)
//			{
//				LCD_ShowPicture(0 + m * 40, 120 + i * 40, 39 + m * 40, 159 + i * 40);
//			}
//		}
//		//while (1)
//		//{
//		//	LCD_ShowNum1(80, 95, t, 5, RED);
//		//	t += 0.01;
//		//}
//	}
//}

//void LCD_Test2(void)
//{
//	Lcd_Init();			//初始化OLED  
//	while (1)
//	{
//
//		LCD_Clear(RED);
//		LCD_Clear(BLUE);
//	}
//}

void LCD_PrintPicture(img_t* src)
{
	int scale = 1;
	int height, width;
	while (1)
	{
		height = src->height / scale;
		width = src->width / scale;
		if (height <= 240 && width <= 240)
		{
			break;
		}
		else {
			++scale;
		}
	}

	uint16_t* rgb = pvPortMalloc(height * width * 2);
	if (src->format == PixelFormatGray)
	{
		uint8_t* p = src->pImg;
        for (uint32_t i = 0; i < height; i += 1)
        {
            for (uint32_t j = 0; j < width; j += 1)
            {
#define GRAY2RGB565(x) (uint16_t)(((x<<8)&0xF800U)|((x<<3)&0x7E0U) |((x>>3)&0x1FU))
				rgb[i * width + j] = GRAY2RGB565(p[i * scale* src->width + j* scale]);
			}
		}
	}
	else if (src->format == PixelFormatRGB565)
	{
		uint16_t* p = src->pImg;
        for (uint32_t i = 0; i < height; i += 1)
        {
            for (uint32_t j = 0; j < width; j += 1)
			{
				rgb[i * width + j] = p[i * scale* src->width + j* scale];
			}
		}
	}

	LCD_Address_Set(0, 0, width - 1, height - 1);
	uint8_t* image = (uint8_t*)rgb;
	for (uint32_t i = 0; i < width * height; i++)
	{
		LCD_WR_DATA8(image[i * 2 + 1]);
		LCD_WR_DATA8(image[i * 2]);
	}
	vPortFree(rgb);
}




