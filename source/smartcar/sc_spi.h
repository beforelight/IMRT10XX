//
// Created by 17616 on 2020/3/23.
//

#ifndef IMXRT_SC_SPI_H
#define IMXRT_SC_SPI_H

#include "fsl_lpspi.h"
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

int SPI_Init(LPSPI_Type* base,uint32_t which_pcs,uint32_t baudRate_Bps,uint32_t srcClock_Hz);

int SPI_Transfer(LPSPI_Type* base, uint32_t whichpcs, uint8_t* rxbuf, uint8_t* txbuf, uint32_t len);
#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif //IMXRT_SC_SPI_H
