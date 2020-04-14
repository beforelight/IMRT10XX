//
// Created by 17616 on 2020/3/23.
//

#ifndef IMXRT_SC_UART_H
#define IMXRT_SC_UART_H

#include "fsl_lpuart.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
/**
 * @brief   初始化串口
 * @param  {LPUART_Type*} base      : 外设
 * @param  {uint32_t} baudRate_Bps_ : 波特率
 * @param  {uint32_t} srcClock_Hz   : 外设时钟
 * @return {status_t}               : 0表示正常
 */
status_t UART_Init(LPUART_Type *base, uint32_t baudRate_Bps_, uint32_t srcClock_Hz);

/**
 * @brief   接收串口数据，如果没有接收到，会一直阻塞
 * @param  {LPUART_Type*} base : 外设
 * @param  {uint8_t*} data     : 数据缓存
 * @param  {size_t} length     : 数据大小
 * @return {status_t}          : 0表示正常
 */
static inline status_t UART_ReadBlocking(LPUART_Type *base, uint8_t *data, size_t length) {
    return LPUART_ReadBlocking(base, data, length);
}

/**
 * @brief   发送串口数据
 * @param  {LPUART_Type*} base : 外设
 * @param  {uint8_t*} data     : 数据缓存
 * @param  {size_t} length     : 数据大小
 */
static inline void UART_WriteBlocking(LPUART_Type *base, const uint8_t *data, size_t length) {
    return LPUART_WriteBlocking(base, data, length);
}

/**
 * @brief   返回串口接收fifo中接收到多少个字节的数据
 * @param  {LPUART_Type*} base : 外设
 * @return {uint32_t}          : 多少字节
 */
static inline uint32_t UART_RxCountGet(LPUART_Type *base){
    return (base->WATER & LPUART_WATER_RXCOUNT_MASK) >> LPUART_WATER_RXCOUNT_SHIFT;
}

/**
 * @brief   返回串口发送是否发送完成
 * @param  {LPUART_Type*} base : 外设
 * @return {uint32_t}          :  Transmission Complete Flag 0b0..Transmitter active 0b1..Transmitter idle
 */
static inline uint32_t UART_TcGet(LPUART_Type* base) {
    return base->STAT & LPUART_STAT_TC_MASK;
}

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif //IMXRT_SC_UART_H
