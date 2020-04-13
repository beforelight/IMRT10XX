//
// Created by 17616 on 2020/3/23.
//

#ifndef IMXRT_SC_UART_H
#define IMXRT_SC_UART_H

#include "fsl_lpuart.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

int UART_Init(LPUART_Type *base, uint32_t baudRate_Bps_, uint32_t srcClock_Hz);

static inline int UART_ReadBlocking(LPUART_Type *base, uint8_t *data, size_t length) {
    return LPUART_ReadBlocking(base, data, length);
}

static inline void UART_WriteBlocking(LPUART_Type *base, const uint8_t *data, size_t length) {
    return LPUART_WriteBlocking(base, data, length);
}

static inline uint32_t UART_RxCountGet(LPUART_Type *base){
    return (base->WATER & LPUART_WATER_RXCOUNT_MASK) >> LPUART_WATER_RXCOUNT_SHIFT;
}

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif //IMXRT_SC_UART_H
