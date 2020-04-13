//
// Created by 17616 on 2020/3/23.
//

#include "sc_uart.h"

int UART_Init(LPUART_Type *base, uint32_t baudRate_Bps_, uint32_t srcClock_Hz) {
    lpuart_config_t config;
    /*
 * config.baudRate_Bps = 115200U;
 * config.parityMode = kLPUART_ParityDisabled;
 * config.stopBitCount = kLPUART_OneStopBit;
 * config.txFifoWatermark = 0;
 * config.rxFifoWatermark = 0;
 * config.enableTx = false;
 * config.enableRx = false;
 */
    LPUART_GetDefaultConfig(&config);
    config.baudRate_Bps = baudRate_Bps_;
    config.enableTx     = true;
    config.enableRx     = true;

    return LPUART_Init(base, &config, srcClock_Hz);
}
