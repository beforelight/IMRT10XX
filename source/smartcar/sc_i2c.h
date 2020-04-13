//
// Created by 17616 on 2020/3/23.
//

#ifndef IMXRT_SC_I2C_H
#define IMXRT_SC_I2C_H

#include "fsl_lpi2c.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

int I2C_Init(LPI2C_Type *base, uint32_t baudRate_Bps, uint32_t sourceClock_Hz);

int I2C_SetBaudRate(LPI2C_Type *base, uint32_t baudRate_Hz, uint32_t sourceClock_Hz);

int I2C_Read(LPI2C_Type *base, uint8_t SlaveAddress7BIT, uint8_t reg, uint8_t *data, uint32_t size);

int I2C_Write(LPI2C_Type *base, uint8_t SlaveAddress7BIT, uint8_t reg, uint8_t *data, uint32_t size);

int I2C_Send(LPI2C_Type *base,
             uint8_t deviceAddress,
             uint32_t subAddress,
             uint8_t subaddressSize,
             uint8_t *txBuff,
             uint8_t txBuffSize);

int I2C_Receive(LPI2C_Type *base,
                uint8_t deviceAddress,
                uint32_t subAddress,
                uint8_t subaddressSize,
                uint8_t *rxBuff,
                uint8_t rxBuffSize);

int I2C_SendSCCB(LPI2C_Type *base,
                 uint8_t deviceAddress,
                 uint32_t subAddress,
                 uint8_t subaddressSize,
                 uint8_t *txBuff,
                 uint8_t txBuffSize);

int I2C_ReceiveSCCB(LPI2C_Type *base,
                    uint8_t deviceAddress,
                    uint32_t subAddress,
                    uint8_t subaddressSize,
                    uint8_t *rxBuff,
                    uint8_t rxBuffSize);

#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif //IMXRT_SC_I2C_H
