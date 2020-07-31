//
// Created by 17616 on 2020/3/23.
//

#ifndef IMXRT_SC_I2C_H
#define IMXRT_SC_I2C_H

#include "fsl_lpi2c.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
/**
 * @brief   初始化主机模式
 * @param  {LPI2C_Type*} base        : 外设
 * @param  {uint32_t} baudRate_Bps   : 波特率
 * @param  {uint32_t} sourceClock_Hz : 外设时钟（查时钟图）
 * @return {status_t}                : 0表示正常
 */
status_t I2C_Init(LPI2C_Type *base, uint32_t baudRate_Bps, uint32_t sourceClock_Hz);

/**
 * @brief   修改波特率
 * @param  {LPI2C_Type*} base        : 外设
 * @param  {uint32_t} baudRate_Hz    : 波特率
 * @param  {uint32_t} sourceClock_Hz : 外设时钟（查时钟图）
 * @return {status_t}                : 0表示正常
 */
status_t I2C_SetBaudRate(LPI2C_Type *base, uint32_t baudRate_Hz, uint32_t sourceClock_Hz);

/**
 * @brief   接收从机的消息
 * @param  {LPI2C_Type*} base         : 
 * @param  {uint8_t} SlaveAddress7BIT : 
 * @param  {uint8_t} reg              : 
 * @param  {uint8_t*} data            : 
 * @param  {uint32_t} size            : 
 * @return {status_t}                 : 
 */
status_t I2C_Read(LPI2C_Type *base, uint8_t SlaveAddress7BIT, uint8_t reg, uint8_t *data, uint32_t size);

/**
 * @brief   给从机发送
 * @param  {LPI2C_Type*} base         : 
 * @param  {uint8_t} SlaveAddress7BIT : 
 * @param  {uint8_t} reg              : 
 * @param  {uint8_t*} data            : 
 * @param  {uint32_t} size            : 
 * @return {status_t}                 : 
 */
status_t I2C_Write(LPI2C_Type *base, uint8_t SlaveAddress7BIT, uint8_t reg, uint8_t *data, uint32_t size);

/**
 * @brief   接收SCCB从机的消息
 * @param  {LPI2C_Type*} base         : 
 * @param  {uint8_t} SlaveAddress7BIT : 
 * @param  {uint8_t} reg              : 
 * @param  {uint8_t*} data            : 
 * @param  {uint32_t} size            : 
 * @return {status_t}                 : 
 */
status_t I2C_ReadSCCB(LPI2C_Type* base, uint8_t SlaveAddress7BIT, uint8_t reg, uint8_t* data, uint32_t size);

/**
 * @brief   给SCCB从机发送
 * @param  {LPI2C_Type*} base         : 
 * @param  {uint8_t} SlaveAddress7BIT : 
 * @param  {uint8_t} reg              : 
 * @param  {uint8_t*} data            : 
 * @param  {uint32_t} size            : 
 * @return {status_t}                 : 
 */
status_t I2C_WriteSCCB(LPI2C_Type* base, uint8_t SlaveAddress7BIT, uint8_t reg, uint8_t* data, uint32_t size);


status_t I2C_Send(LPI2C_Type *base,
             uint8_t deviceAddress,
             uint32_t subAddress,
             uint8_t subaddressSize,
             uint8_t *txBuff, uint32_t txBuffSize);

status_t I2C_Receive(LPI2C_Type *base,
                uint8_t deviceAddress,
                uint32_t subAddress,
                uint8_t subaddressSize,
                uint8_t *rxBuff, uint32_t rxBuffSize);

status_t I2C_SendSCCB(LPI2C_Type *base,
                 uint8_t deviceAddress,
                 uint32_t subAddress,
                 uint8_t subaddressSize,
                 uint8_t *txBuff, uint32_t txBuffSize);

status_t I2C_ReceiveSCCB(LPI2C_Type *base,
                    uint8_t deviceAddress,
                    uint32_t subAddress,
                    uint8_t subaddressSize,
                    uint8_t *rxBuff, uint32_t rxBuffSize);

#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif //IMXRT_SC_I2C_H
