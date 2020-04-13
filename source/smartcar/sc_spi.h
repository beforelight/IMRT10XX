//
// Created by 17616 on 2020/3/23.
//

#ifndef IMXRT_SC_SPI_H
#define IMXRT_SC_SPI_H

#include "fsl_lpspi.h"
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
/**
 * @brief   初始化spi主机模式
 * @param  {LPSPI_Type*} base      : 外设
 * @param  {uint32_t} which_pcs    : 片选
 * @param  {uint32_t} baudRate_Bps : 波特率
 * @param  {uint32_t} srcClock_Hz  : 外设时钟
 * @return {status_t}              : 0表示正常
 */
status_t SPI_Init(LPSPI_Type* base,uint32_t which_pcs,uint32_t baudRate_Bps,uint32_t srcClock_Hz);

/**
 * @brief   spi双向传输
 * @param  {LPSPI_Type*} base  : 外设
 * @param  {uint32_t} whichpcs : 片选
 * @param  {uint8_t*} rxbuf    : 接收缓存，如果不收数据的话为NULL
 * @param  {uint8_t*} txbuf    : 发送缓存，如果不发数据的话为NULL
 * @param  {uint32_t} len      : 数据长度
 * @return {status_t}          : 0位正常
 */
status_t SPI_Transfer(LPSPI_Type* base, uint32_t whichpcs, uint8_t* rxbuf, uint8_t* txbuf, uint32_t len);
#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif //IMXRT_SC_SPI_H
