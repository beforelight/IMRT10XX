//
// Created by 17616 on 2020/3/23.
//

#ifndef IMXRT_SC_I2CS_H
#define IMXRT_SC_I2CS_H

#include "sc_gpio.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

	typedef struct struct_I2CS {
		uint32_t delay;//用于延时控制软件i2c波特率，越大波特率越小。100时约为400khz-800khz
		gpio_t SDA;
		gpio_t SCL;
	}I2CS_Type;

/**
 * @brief	初始化软件i2c
 * @param  {I2CS_Type*} base : 
 * @return {status_t}        : 
 */
status_t I2CS_Init(I2CS_Type*base);

/**
 * @brief   接收从机的消息
 * @param  {I2CS_Type*} base          : 
 * @param  {uint8_t} SlaveAddress7BIT : 
 * @param  {uint8_t} reg              : 
 * @param  {uint8_t*} data            : 
 * @param  {uint32_t} size            : 
 * @return {status_t}                 : 
 */
status_t I2CS_Read(I2CS_Type* base, uint8_t SlaveAddress7BIT, uint8_t reg, uint8_t *data, uint32_t size);

/**
 * @brief   给从机发送
 * @param  {I2CS_Type*} base          : 
 * @param  {uint8_t} SlaveAddress7BIT : 
 * @param  {uint8_t} reg              : 
 * @param  {uint8_t*} data            : 
 * @param  {uint32_t} size            : 
 * @return {status_t}                 : 
 */
status_t I2CS_Write(I2CS_Type*base, uint8_t SlaveAddress7BIT, uint8_t reg, uint8_t *data, uint32_t size);

/**
 * @brief   接收SCCB从机的消息
 * @param  {LPI2C_Type*} base         : 
 * @param  {uint8_t} SlaveAddress7BIT : 
 * @param  {uint8_t} reg              : 
 * @param  {uint8_t*} data            : 
 * @param  {uint32_t} size            : 
 * @return {status_t}                 : 
 */
status_t I2CS_ReadSCCBforOV7725(I2CS_Type* base, uint8_t SlaveAddress7BIT, uint8_t reg, uint8_t* data, uint32_t size);

/**
 * @brief   给SCCB从机发送
 * @param  {LPI2C_Type*} base         : 
 * @param  {uint8_t} SlaveAddress7BIT : 
 * @param  {uint8_t} reg              : 
 * @param  {uint8_t*} data            : 
 * @param  {uint32_t} size            : 
 * @return {status_t}                 : 
 */
status_t I2CS_WriteSCCBforOV7725(I2CS_Type* base, uint8_t SlaveAddress7BIT, uint8_t reg, uint8_t* data, uint32_t size);



status_t I2CS_ReadSCCB(I2CS_Type* base, uint8_t SlaveAddress7BIT, uint8_t reg, uint8_t* data, uint32_t size);
status_t I2CS_WriteSCCB(I2CS_Type* base, uint8_t SlaveAddress7BIT, uint8_t reg, uint8_t* data, uint32_t size);


//软件IIC所有操作函数
void IIC_Delay(I2CS_Type* base);				//IIC延时函数
void IIC_Start(I2CS_Type* base);				//发送IIC开始信号
void IIC_Stop(I2CS_Type* base);	  			//发送IIC停止信号
void IIC_Send_Byte(I2CS_Type* base, uint8_t txd);			//IIC发送一个字节
uint8_t IIC_Read_Byte(I2CS_Type* base, uint8_t ack);//IIC读取一个字节
uint8_t IIC_Wait_Ack(I2CS_Type* base); 				//IIC等待ACK信号
void IIC_Ack(I2CS_Type* base);					//IIC发送ACK信号
void IIC_NAck(I2CS_Type* base);				//IIC不发送ACK信号

//SCCB for ov7725的操作函数
uint8_t SCCB_RegWrite(I2CS_Type* base, uint8_t Device, uint8_t Address, uint8_t Data);
uint8_t SCCB_SendByte(I2CS_Type* base, uint8_t Data);
void SCCB_Wait(I2CS_Type* base);

void SCCB_Star(I2CS_Type* base);
void SCCB_Stop(I2CS_Type* base);
void SCCB_NAck(I2CS_Type* base );
uint8_t SCCB_RegRead(I2CS_Type* base, uint8_t Device, uint8_t Address, uint8_t* Data);

uint8_t SCCB_ReadByte(I2CS_Type* base);

#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif //IMXRT_SC_I2CS_H
