/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "evkbimxrt1050_flexspi_nor_config.h"
#include "smartcar/sc_flash.h"
#include "fsl_flexspi.h"
/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "platform.drivers.xip_board"
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
#if defined(XIP_BOOT_HEADER_ENABLE) && (XIP_BOOT_HEADER_ENABLE == 1)
#if defined(__CC_ARM) || defined(__ARMCC_VERSION) || defined(__GNUC__)
__attribute__((section(".boot_hdr.conf")))
#elif defined(__ICCARM__)
#pragma location = ".boot_hdr.conf"
#endif

const flexspi_nor_config_t Qspiflash_config =
        {
                .memConfig =
                        {
                                .tag = FLEXSPI_CFG_BLK_TAG,
                                .version = FLEXSPI_CFG_BLK_VERSION,
                                .readSampleClkSrc = kFlexSPIReadSampleClk_LoopbackInternally,
                                .csHoldTime = 3u,
                                .csSetupTime = 3u,
                                .deviceModeCfgEnable = true,
                                .deviceModeType = 1,//Quad Enable command
                                .deviceModeSeq.seqNum = 1,
                                .deviceModeSeq.seqId = 4,
                                .deviceModeArg = 0x000200,//Set QE
                                .deviceType = kFlexSpiDeviceType_SerialNOR,
                                .sflashPadType = kSerialFlash_4Pads,
                                .serialClkFreq = kFlexSpiSerialClk_133MHz,//80MHz for Winbond, 100MHz for GD, 133MHz for ISSI
                                .sflashA1Size = 16u * 1024u * 1024u,//4MBytes
                                .dataValidTime = {16u, 16u},
                                .lookupTable =
                                        {
                                                //         //Fast Read Sequence
                                                //         [0]  = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x0B, RADDR_SDR, FLEXSPI_1PAD, 0x18),
                                                //         [1]  = FLEXSPI_LUT_SEQ(DUMMY_SDR, FLEXSPI_1PAD, 0x08, READ_SDR, FLEXSPI_1PAD, 0x08),
                                                //         [2]  = FLEXSPI_LUT_SEQ(JMP_ON_CS, 0, 0, 0, 0, 0),
                                                //Quad Input/output read sequence
                                                [0] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0xEB, RADDR_SDR,
                                                                      FLEXSPI_4PAD, 0x18),
                                                [1] = FLEXSPI_LUT_SEQ(DUMMY_SDR, FLEXSPI_4PAD, 0x06, READ_SDR,
                                                                      FLEXSPI_4PAD, 0x04),
                                                [2] = FLEXSPI_LUT_SEQ(0, 0, 0, 0, 0, 0),
                                                //Read Status
                                                [1 * 4] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x05, READ_SDR,
                                                                          FLEXSPI_1PAD, 0x04),
                                                //Write Enable
                                                [3 * 4] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x06, STOP, 0, 0),
                                                //Write status
                                                [4 * 4] = FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x01, WRITE_SDR,
                                                                          FLEXSPI_1PAD, 0x2),
                                                /* Read ID */
                                                [4 * NOR_CMD_LUT_SEQ_IDX_READID1] =
                                                FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0xAB, kFLEXSPI_Command_DUMMY_SDR, kFLEXSPI_1PAD, 0x18),
                                                [4 * NOR_CMD_LUT_SEQ_IDX_READID1 + 1] =
                                                FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
                                                /* Read extend parameters */
                                                [4 * NOR_CMD_LUT_SEQ_IDX_READSTATUS1] =
                                                FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x05, kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04),
                                                /* Write Enable */
                                                [4 * NOR_CMD_LUT_SEQ_IDX_WRITEENABLE1] =
                                                FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x06, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
                                                /* Erase Sector  */
                                                [4 * NOR_CMD_LUT_SEQ_IDX_ERASESECTOR1] =
                                                FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x20, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
                                                /* Page Program - single mode */
                                                [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE1] =
                                                FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x02, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
                                                [4 * NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE1 + 1] =
                                                FLEXSPI_LUT_SEQ(kFLEXSPI_Command_WRITE_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
                                                /* Normal read mode -SDR */
                                                [4 * NOR_CMD_LUT_SEQ_IDX_READ_NORMAL1] =
                                                FLEXSPI_LUT_SEQ(kFLEXSPI_Command_SDR, kFLEXSPI_1PAD, 0x03, kFLEXSPI_Command_RADDR_SDR, kFLEXSPI_1PAD, 0x18),
                                                [4 * NOR_CMD_LUT_SEQ_IDX_READ_NORMAL1 + 1] =
                                                FLEXSPI_LUT_SEQ(kFLEXSPI_Command_READ_SDR, kFLEXSPI_1PAD, 0x04, kFLEXSPI_Command_STOP, kFLEXSPI_1PAD, 0),
                                        },
                        },
                .pageSize = 256u,
                .sectorSize = 4u * 1024u,
        };
#endif /* XIP_BOOT_HEADER_ENABLE */
