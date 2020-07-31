#include "sc_i2cs.h"

#define SCL_out(base) GPIO_Out(&base->SCL)
#define SDA_out(base) GPIO_Out(&base->SDA)
#define SCL_in(base)  GPIO_In(&base->SCL)
#define SDA_in(base)  GPIO_In(&base->SDA)
#define SCL_H(base)   GPIO_Set(&base->SCL)
#define SDA_H(base)   GPIO_Set(&base->SDA)
#define SCL_L(base)   GPIO_Clear(&base->SCL)
#define SDA_L(base)   GPIO_Clear(&base->SDA)
#define SCL_val(base) GPIO_Read(&base->SCL)
#define SDA_val(base) GPIO_Read(&base->SDA)

status_t I2CS_Init(I2CS_Type* base)
{
	GPIO_Init(&base->SCL);
	GPIO_Init(&base->SDA);
	GPIO_Set(&base->SCL);
	GPIO_Set(&base->SDA);
	return kStatus_Success;
}

status_t I2CS_Read(I2CS_Type* base, uint8_t SlaveAddress7BIT, uint8_t reg, uint8_t* data, uint32_t size)
{
	IIC_Start(base);
	IIC_Send_Byte(base, (SlaveAddress7BIT << 1) | 0);//发送器件地址+写命令	
	if (IIC_Wait_Ack(base))	//等待应答
	{
		IIC_Stop(base);
		return 1;
	}
	IIC_Send_Byte(base, reg);	//写寄存器地址
	IIC_Wait_Ack(base);		//等待应答
	IIC_Start(base);
	IIC_Send_Byte(base, (SlaveAddress7BIT << 1) | 1);//发送器件地址+读命令	
	IIC_Wait_Ack(base);		//等待应答 
	while (size)
	{
		if (size == 1)*data = IIC_Read_Byte(base, 0);//读数据,发送nACK 
		else *data = IIC_Read_Byte(base, 1);		//读数据,发送ACK  
		size--;
		data++;
	}
	IIC_Stop(base);	//产生一个停止条件 
	return 0;
}

status_t I2CS_Write(I2CS_Type* base, uint8_t SlaveAddress7BIT, uint8_t reg, uint8_t* data, uint32_t size)
{
	uint8_t i;
	IIC_Start(base);
	IIC_Send_Byte(base, (SlaveAddress7BIT << 1) | 0);//发送器件地址+写命令	
	if (IIC_Wait_Ack(base))	//等待应答
	{
		IIC_Stop(base);
		return 1;
	}
	IIC_Send_Byte(base, reg);	//写寄存器地址
	IIC_Wait_Ack(base);		//等待应答
	for (i = 0; i < size; i++)
	{
		IIC_Send_Byte(base, data[i]);	//发送数据
		if (IIC_Wait_Ack(base))		//等待ACK
		{
			IIC_Stop(base);
			return 1;
		}
	}
	IIC_Stop(base);
	return 0;
}

status_t I2CS_ReadSCCB(LPI2C_Type* base, uint8_t SlaveAddress7BIT, uint8_t reg, uint8_t* data, uint32_t size)
{
	uint8_t i;
	IIC_Start(base);
	IIC_Send_Byte(base, (SlaveAddress7BIT << 1) | 0);//发送器件地址+写命令	
	IIC_Delay(base);
	IIC_Send_Byte(base, reg);	//写寄存器地址
	IIC_Delay(base);
	IIC_Stop(base);
	IIC_Delay(base);
	IIC_Start(base);
	IIC_Send_Byte(base, (SlaveAddress7BIT << 1) | 1);//发送器件地址+读命令	
	while (size)
	{
		if (size == 1)*data = IIC_Read_Byte(base, 0);//读数据,发送nACK 
		else *data = IIC_Read_Byte(base, 1);		//读数据,发送ACK  
		size--;
		data++;
	}
	IIC_Stop(base);	//产生一个停止条件 
	return kStatus_Success;
}

status_t I2CS_WriteSCCB(LPI2C_Type* base, uint8_t SlaveAddress7BIT, uint8_t reg, uint8_t* data, uint32_t size)
{
	uint8_t i;
	IIC_Start(base);
	IIC_Send_Byte(base, (SlaveAddress7BIT << 1) | 0);//发送器件地址+写命令	
	IIC_Delay(base);
	IIC_Send_Byte(base, reg);	//写寄存器地址
	IIC_Delay(base);
	for (i = 0; i < size; i++)
	{
		IIC_Send_Byte(base, data[i]);	//发送数据
		IIC_Delay(base);
	}
	IIC_Stop(base);
	return kStatus_Success;
}

//以下是i2c操作函数
void IIC_Delay(I2CS_Type* base)
{
	uint32_t a = base->delay;
	while (--a) {}
}

void IIC_Delay2(I2CS_Type* base)
{
	uint32_t a = base->delay >> 1;
	while (--a) {}
}

void IIC_Delay2(LPI2C_Type* base)
{
}

void IIC_Start(I2CS_Type* base)
{
	SDA_H(base);
	SCL_H(base);
	SDA_out(base);     //sda线输出
	SCL_out(base);

	IIC_Delay(base);
	SDA_L(base);//START:when CLK is high,DATA change form high to low 
	IIC_Delay(base);
	SCL_L(base); //钳住I2C总线，准备发送或接收数据
}

void IIC_Stop(I2CS_Type* base)
{
	SCL_L(base);
	SDA_L(base);//STOP:when CLK is high DATA change form low to high
	SDA_out(base);     //sda线输出
	IIC_Delay(base);
	SCL_H(base);
	SDA_H(base);//发送I2C总线结束信号
	IIC_Delay(base);
}

void IIC_Send_Byte(I2CS_Type* base, uint8_t txd)
{
	uint8_t t;
	SDA_out(base);
	SCL_L(base);// 拉低时钟开始数据传输
	for (t = 0; t < 8; t++)
	{
		if ((txd & 0x80) >> 7) {
			SDA_H(base);
		}
		else {
			SDA_L(base);
		}
		txd <<= 1;
		SCL_H(base);
		IIC_Delay(base);
		SCL_L(base);
		IIC_Delay(base);
	}
}

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t IIC_Read_Byte(I2CS_Type* base, uint8_t ack)
{
	uint8_t i, receive = 0;
	SDA_in(base);//SDA设置为输入
	for (i = 0; i < 8; i++)
	{
		SCL_L(base);
		IIC_Delay(base);
		SCL_H(base);
		receive <<= 1;
		if (SDA_val(base))receive++;
		IIC_Delay(base);
	}
	if (!ack)
		IIC_NAck(base);//发送nACK
	else
		IIC_Ack(base); //发送ACK   
	return receive;
}

uint8_t IIC_Wait_Ack(I2CS_Type* base)
{
	uint8_t ucErrTime = 0;
	SDA_in(base);      //SDA设置为输入  
	SDA_H(base); IIC_Delay(base);
	SCL_H(base); IIC_Delay(base);
	while (SDA_val(base))
	{
		IIC_Delay(base);
		ucErrTime++;
		if (ucErrTime > 10)
		{
			IIC_Stop(base);
			return 1;
		}
	}
	SCL_L(base);//时钟输出0 	   
	return 0;
}


void IIC_Ack(I2CS_Type* base)
{
	SCL_L(base);
	SDA_out(base);
	SDA_L(base);
	IIC_Delay(base);
	SCL_H(base);
	IIC_Delay(base);
	SCL_L(base);
}

void IIC_NAck(I2CS_Type* base)
{
	SCL_L(base);
	SDA_out(base);
	SDA_H(base);
	IIC_Delay(base);
	SCL_H(base);
	IIC_Delay(base);
	SCL_L(base);
}