//
// Created by 17616 on 2020/3/23.
//

#include "sc_spi.h"

status_t SPI_Init(LPSPI_Type *base, uint32_t which_pcs, uint32_t baudRate_Bps, uint32_t srcClock_Hz) {
    lpspi_master_config_t masterConfig;

    /*Master config*/
    masterConfig.baudRate = baudRate_Bps;
    masterConfig.bitsPerFrame = 8;
    masterConfig.cpol = kLPSPI_ClockPolarityActiveHigh;
    masterConfig.cpha = kLPSPI_ClockPhaseFirstEdge;
    masterConfig.direction = kLPSPI_MsbFirst;

    masterConfig.pcsToSckDelayInNanoSec = 1000000000 / masterConfig.baudRate;
    masterConfig.lastSckToPcsDelayInNanoSec = 1000000000 / masterConfig.baudRate;
    masterConfig.betweenTransferDelayInNanoSec = 1000000000 / masterConfig.baudRate;

    masterConfig.whichPcs = which_pcs;
    masterConfig.pcsActiveHighOrLow = kLPSPI_PcsActiveLow;

    masterConfig.pinCfg = kLPSPI_SdiInSdoOut;
    masterConfig.dataOutConfig = kLpspiDataOutRetained;

    LPSPI_MasterInit(base, &masterConfig, srcClock_Hz);
    return kStatus_Success;
}

status_t SPI_Transfer(LPSPI_Type *base, uint32_t whichpcs, uint8_t *rxbuf, uint8_t *txbuf, uint32_t len) {
    lpspi_transfer_t xfer;
    xfer.txData = txbuf;
    xfer.rxData = rxbuf;
    xfer.dataSize = len;
    xfer.configFlags = whichpcs << LPSPI_MASTER_PCS_SHIFT;
    return LPSPI_MasterTransferBlocking(base, &xfer);
}
