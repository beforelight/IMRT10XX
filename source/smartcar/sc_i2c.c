//
// Created by 17616 on 2020/3/23.
//

#include "sc_i2c.h"

int I2C_Init(LPI2C_Type *base, uint32_t baudRate_Bps, uint32_t sourceClock_Hz) {
    lpi2c_master_config_t masterConfig;
    LPI2C_MasterGetDefaultConfig(&masterConfig);
    /* Change the default baudrate configuration */
    masterConfig.baudRate_Hz = baudRate_Bps;
    /* Initialize the LPI2C master peripheral */
    LPI2C_MasterInit(base, &masterConfig, sourceClock_Hz);
    return 0;
}

int I2C_SetBaudRate(LPI2C_Type *base, uint32_t baudRate_Hz, uint32_t sourceClock_Hz) {
    LPI2C_MasterSetBaudRate(base, sourceClock_Hz, baudRate_Hz);
    return 0;
}

int I2C_Read(LPI2C_Type *base, uint8_t SlaveAddress7BIT, uint8_t reg, uint8_t *data, uint32_t size) {
    lpi2c_master_transfer_t xfer;

    xfer.flags = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress = SlaveAddress7BIT;
    xfer.direction = kLPI2C_Read;
    xfer.subaddress = reg;
    xfer.subaddressSize = 1;
    xfer.data = data;
    xfer.dataSize = size;

    return LPI2C_MasterTransferBlocking(base, &xfer);
}

int I2C_Write(LPI2C_Type *base, uint8_t SlaveAddress7BIT, uint8_t reg, uint8_t *data, uint32_t size) {
    lpi2c_master_transfer_t xfer;

    xfer.flags = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress = SlaveAddress7BIT;
    xfer.direction = kLPI2C_Write;
    xfer.subaddress = reg;
    xfer.subaddressSize = 1;
    xfer.data = data;
    xfer.dataSize = size;

    return LPI2C_MasterTransferBlocking(base, &xfer);
}


int I2C_Send(LPI2C_Type *base,
             uint8_t deviceAddress,
             uint32_t subAddress,
             uint8_t subAddressSize,
             uint8_t *txBuff,
             uint8_t txBuffSize) {
    lpi2c_master_transfer_t xfer;

    xfer.flags = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress = deviceAddress;
    xfer.direction = kLPI2C_Write;
    xfer.subaddress = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data = txBuff;
    xfer.dataSize = txBuffSize;

    return LPI2C_MasterTransferBlocking(base, &xfer);
}

int I2C_Receive(LPI2C_Type *base,
                uint8_t deviceAddress,
                uint32_t subAddress,
                uint8_t subAddressSize,
                uint8_t *rxBuff,
                uint8_t rxBuffSize) {
    lpi2c_master_transfer_t xfer;

    xfer.flags = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress = deviceAddress;
    xfer.direction = kLPI2C_Read;
    xfer.subaddress = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data = rxBuff;
    xfer.dataSize = rxBuffSize;

    return LPI2C_MasterTransferBlocking(base, &xfer);
}

int I2C_SendSCCB(LPI2C_Type *base,
                 uint8_t deviceAddress,
                 uint32_t subAddress,
                 uint8_t subAddressSize,
                 uint8_t *txBuff,
                 uint8_t txBuffSize) {
    lpi2c_master_transfer_t xfer;

    xfer.flags = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress = deviceAddress;
    xfer.direction = kLPI2C_Write;
    xfer.subaddress = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data = txBuff;
    xfer.dataSize = txBuffSize;

    return LPI2C_MasterTransferBlocking(base, &xfer);
}

int I2C_ReceiveSCCB(LPI2C_Type *base,
                    uint8_t deviceAddress,
                    uint32_t subAddress,
                    uint8_t subAddressSize,
                    uint8_t *rxBuff,
                    uint8_t rxBuffSize) {
    status_t status;
    lpi2c_master_transfer_t xfer;

    xfer.flags = kLPI2C_TransferDefaultFlag;
    xfer.slaveAddress = deviceAddress;
    xfer.direction = kLPI2C_Write;
    xfer.subaddress = subAddress;
    xfer.subaddressSize = subAddressSize;
    xfer.data = NULL;
    xfer.dataSize = 0;

    status = LPI2C_MasterTransferBlocking(base, &xfer);

    if (kStatus_Success == status) {
        xfer.subaddressSize = 0;
        xfer.direction = kLPI2C_Read;
        xfer.data = rxBuff;
        xfer.dataSize = rxBuffSize;

        status = LPI2C_MasterTransferBlocking(base, &xfer);
    }

    return status;
}
