#include "sc_i2c.h"

status_t I2C_Init(LPI2C_Type *base, uint32_t baudRate_Bps, uint32_t sourceClock_Hz) {
    lpi2c_master_config_t masterConfig;
    LPI2C_MasterGetDefaultConfig(&masterConfig);
    /* Change the default baudrate configuration */
    masterConfig.baudRate_Hz = baudRate_Bps;
    /* Initialize the LPI2C master peripheral */
    LPI2C_MasterInit(base, &masterConfig, sourceClock_Hz);
    return kStatus_Success;
}

status_t I2C_SetBaudRate(LPI2C_Type *base, uint32_t baudRate_Hz, uint32_t sourceClock_Hz) {
    LPI2C_MasterSetBaudRate(base, sourceClock_Hz, baudRate_Hz);
    return kStatus_Success;
}

status_t I2C_Read(LPI2C_Type *base, uint8_t SlaveAddress7BIT, uint8_t reg, uint8_t *data, uint32_t size) {
    return I2C_Receive(base, SlaveAddress7BIT, reg, 1, data, size);
}

status_t I2C_Write(LPI2C_Type *base, uint8_t SlaveAddress7BIT, uint8_t reg, uint8_t *data, uint32_t size) {
    return I2C_Send(base, SlaveAddress7BIT, reg, 1, data, size);
}

status_t I2C_ReadSCCB(LPI2C_Type* base, uint8_t SlaveAddress7BIT, uint8_t reg, uint8_t* data, uint32_t size){
    return I2C_ReceiveSCCB(base, SlaveAddress7BIT, reg, 1, data, size);
}

status_t I2C_WriteSCCB(LPI2C_Type* base, uint8_t SlaveAddress7BIT, uint8_t reg, uint8_t* data, uint32_t size){
    return I2C_SendSCCB(base, SlaveAddress7BIT, reg, 1, data, size);
}


status_t I2C_Send(LPI2C_Type *base,
             uint8_t deviceAddress,
             uint32_t subAddress,
             uint8_t subAddressSize,
             uint8_t *txBuff, uint32_t txBuffSize) {
    status_t status;
    status =  LPI2C_MasterStart(base, deviceAddress, kLPI2C_Write);
    if (status != kStatus_Success) { return status; }//kStatus_LPI2C_Busy总线忙

    status = LPI2C_MasterSend(base, &subAddress, subAddressSize);
    if (status != kStatus_Success) { LPI2C_MasterStop(base); return status; }//kStatus_LPI2C_Nak无应答

    status = LPI2C_MasterSend(base, txBuff, txBuffSize);
    if (status != kStatus_Success) { LPI2C_MasterStop(base); return status; }//kStatus_LPI2C_Nak无应答

    status = LPI2C_MasterStop(base);
    return status;
}

status_t I2C_Receive(LPI2C_Type *base,
                uint8_t deviceAddress,
                uint32_t subAddress,
                uint8_t subAddressSize,
                uint8_t *rxBuff, uint32_t rxBuffSize) {
    status_t status;
    status = LPI2C_MasterStart(base, deviceAddress, kLPI2C_Write);
    if (status != kStatus_Success) { return status; }//kStatus_LPI2C_Busy总线忙

    status = LPI2C_MasterSend(base, &subAddress, subAddressSize);
    if (status != kStatus_Success) { LPI2C_MasterStop(base); return status; }//kStatus_LPI2C_Nak无应答

    status = LPI2C_MasterStart(base, deviceAddress, kLPI2C_Read);
    if (status != kStatus_Success) { return status; }//kStatus_LPI2C_Busy总线忙

    status = LPI2C_MasterReceive(base, rxBuff, rxBuffSize);
    if (status != kStatus_Success) { LPI2C_MasterStop(base); return status; }//kStatus_LPI2C_Nak无应答

    status = LPI2C_MasterStop(base);
    return status;
}

status_t I2C_SendSCCB(LPI2C_Type *base,
                 uint8_t deviceAddress,
                 uint32_t subAddress,
                 uint8_t subAddressSize,
                 uint8_t *txBuff, uint32_t txBuffSize) {
    //写时序和i2c一样，但是sccb没有规定从机要应答
    status_t status;
    status = LPI2C_MasterStart(base, deviceAddress, kLPI2C_Write);
    if (status != kStatus_Success) { return status; }//kStatus_LPI2C_Busy总线忙

    status = LPI2C_MasterSend(base, &subAddress, subAddressSize);

    status = LPI2C_MasterSend(base, txBuff, txBuffSize);

    status = LPI2C_MasterStop(base);
    return status;
}

status_t I2C_ReceiveSCCB(LPI2C_Type *base,
                    uint8_t deviceAddress,
                    uint32_t subAddress,
                    uint8_t subAddressSize,
                    uint8_t *rxBuff, uint32_t rxBuffSize) {
    //读时序和i2c有不同，而且sccb没有规定从机要应答
    status_t status;
    status = LPI2C_MasterStart(base, deviceAddress, kLPI2C_Write);
    if (status != kStatus_Success) { return status; }//kStatus_LPI2C_Busy总线忙

    status = LPI2C_MasterSend(base, &subAddress, subAddressSize);

    status = LPI2C_MasterStop(base);//中间多了一个停止

    status = LPI2C_MasterStart(base, deviceAddress, kLPI2C_Read);
    if (status != kStatus_Success) { return status; }//kStatus_LPI2C_Busy总线忙

    status = LPI2C_MasterReceive(base, rxBuff, rxBuffSize);

    status = LPI2C_MasterStop(base);
    return status;
}
