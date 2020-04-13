/*
 * sc_flash.c
 *
 *  Created on: 2020年1月16日
 *      Author: 17616
 */
#include "fsl_debug_console.h"
#include "sc_flash.h"
#include "fsl_flexspi.h"
//#define RAMFUNC /*__attribute__((section(".ramfunc.$SDRAM_NOCACHE")))*/
#define RAMFUNC __attribute__((section(".ramfunc.$SRAM_OC")))
//#define RAMFUNC RAMFUNC_SDRAM_NOCACHE
/*******************************************************************************
* Definitions
******************************************************************************/
#define EXAMPLE_FLEXSPI FLEXSPI
#define EXAMPLE_FLEXSPI_AMBA_BASE FlexSPI_AMBA_BASE
#define EXAMPLE_FLEXSPI_CLOCK kCLOCK_FlexSpi


#define NOR_CMD_LUT_SEQ_IDX_READID1             10
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS1         11
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE1        12
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR1        13
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE1 14
#define NOR_CMD_LUT_SEQ_IDX_READ_NORMAL1                15

#define FLASH_BUSY_STATUS_POL 1
#define FLASH_BUSY_STATUS_OFFSET 0
#define FLASH_ERROR_STATUS_MASK 0x0e


/*******************************************************************************
 * Variables
 ******************************************************************************/


flexspi_device_config_t deviceconfig = {
        .flexspiRootClk = 1000 * (1000 * 2160 / 17),
        .flashSize = 4 * 1024,
        .CSIntervalUnit = kFLEXSPI_CsIntervalUnit1SckCycle,
        .CSInterval = 2,
        .CSHoldTime = 3,
        .CSSetupTime = 3,
        .dataValidTime = 0,
        .columnspace = 0,
        .enableWordAddress = 0,
        .AWRSeqIndex = 0,
        .AWRSeqNumber = 0,
        .ARDSeqIndex = 0,//Normal Read
        .ARDSeqNumber = 1,
        .AHBWriteWaitUnit = kFLEXSPI_AhbWriteWaitUnit2AhbCycle,
        .AHBWriteWaitInterval = 0,
};

/* Fixed 64 LUTs, suggest to reserve the LUTs in the front */
const uint32_t customLUT[64] = {
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
};

status_t RAMFUNC flexspi_nor_write_enable(FLEXSPI_Type *base, uint32_t baseAddr) {
    flexspi_transfer_t flashXfer;
    status_t status;

    /* Write neable */
    flashXfer.deviceAddress = baseAddr;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Command;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_WRITEENABLE1;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    return status;
}

status_t RAMFUNC flexspi_nor_wait_bus_busy(FLEXSPI_Type *base) {
    /* Wait status ready. */
    bool isBusy;
    uint32_t readValue;
    status_t status;
    flexspi_transfer_t flashXfer;

    flashXfer.deviceAddress = 0;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Read;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_READSTATUS1;
    flashXfer.data = &readValue;
    flashXfer.dataSize = 4;

    do {
        status = FLEXSPI_TransferBlocking(base, &flashXfer);
        if (status != kStatus_Success) {
            return status;
        }
        if (FLASH_BUSY_STATUS_POL) {
            if (readValue & (1U << FLASH_BUSY_STATUS_OFFSET)) {
                isBusy = true;
            } else {
                isBusy = false;
            }
        } else {
            if (readValue & (1U << FLASH_BUSY_STATUS_OFFSET)) {
                isBusy = false;
            } else {
                isBusy = true;
            }
        }

    } while (isBusy);


    return status;
}

status_t RAMFUNC flexspi_nor_flash_erase_sector(FLEXSPI_Type *base, uint32_t address) {
    status_t status;
    flexspi_transfer_t flashXfer;

    /* Write enable */
    flashXfer.deviceAddress = address;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Command;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_WRITEENABLE1;

    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success) {
        return status;
    }

    flashXfer.deviceAddress = address;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Command;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_ERASESECTOR1;
    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success) {
        return status;
    }

    status = flexspi_nor_wait_bus_busy(base);

    return status;
}

status_t RAMFUNC flexspi_nor_flash_page_program(FLEXSPI_Type *base, uint32_t dstAddr, const uint32_t *src) {
    status_t status;
    flexspi_transfer_t flashXfer;

    /* Write neable */
    status = flexspi_nor_write_enable(base, dstAddr);

    if (status != kStatus_Success) {
        return status;
    }

    /* Prepare page program command */
    flashXfer.deviceAddress = dstAddr;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Write;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE1;
    flashXfer.data = (uint32_t *) src;
    flashXfer.dataSize = FLASH_PAGE_SIZE;
    status = FLEXSPI_TransferBlocking(base, &flashXfer);

    if (status != kStatus_Success) {
        return status;
    }

    status = flexspi_nor_wait_bus_busy(base);

    return status;
}

status_t RAMFUNC flexspi_nor_flash_read_sector(FLEXSPI_Type *base, uint32_t address, uint32_t *src, size_t leng) {
    //uint32_t temp;
    flexspi_transfer_t flashXfer;
    flashXfer.deviceAddress = address;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Read;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_READ_NORMAL1;
    flashXfer.data = (uint32_t *) src;
    flashXfer.dataSize = leng;

    status_t status = FLEXSPI_TransferBlocking(base, &flashXfer);

    // *vendorId = temp;

    return status;
}

status_t RAMFUNC flexspi_nor_get_vendor_id(FLEXSPI_Type *base, uint8_t *vendorId) {
    uint32_t temp;
    flexspi_transfer_t flashXfer;
    flashXfer.deviceAddress = 0;
    flashXfer.port = kFLEXSPI_PortA1;
    flashXfer.cmdType = kFLEXSPI_Read;
    flashXfer.SeqNumber = 1;
    flashXfer.seqIndex = NOR_CMD_LUT_SEQ_IDX_READID1;
    flashXfer.data = &temp;
    flashXfer.dataSize = 1;

    status_t status = FLEXSPI_TransferBlocking(base, &flashXfer);

    *vendorId = temp;

    return status;
}

/**
 * @brief  初始化
 * @param  {void} undefined :
 * @return {status_t}       : 错误代码，0表示正常
 */
RAMFUNC status_t FLASH_Init(void) {
    FLASH_EnterCritical();

    //    /* Update LUT table. */
    FLEXSPI_UpdateLUT(EXAMPLE_FLEXSPI,
                      NOR_CMD_LUT_SEQ_IDX_READID1 * 4,
                      &customLUT[NOR_CMD_LUT_SEQ_IDX_READID1 * 4], 64);
    //    /* Get vendor ID. */
    uint8_t vendorID;
    status_t status = flexspi_nor_get_vendor_id(EXAMPLE_FLEXSPI, &vendorID);
    FLASH_ExitCritical();
    if (status != kStatus_Success) {
        PRINTF("Get Vendor ID Failure!");
        assert(0);//return status;
    }
    PRINTF("Vendor ID: 0x%x\r\n", vendorID);
    return status;
}

/**
 * @brief  flash读
 * @param  {uint32_t} address : Flash地址，flash存储的第一个字节的地址为0x0，第二个字节的地址为0x1，以此类推
 * @param  {uint8_t*} buffer  : 缓存地址
 * @param  {uint32_t} length  : 读出的长度，单位字节
 * @return {status_t}         : 错误代码，0表示正常
 */
RAMFUNC status_t FLASH_Read(uint32_t address, uint8_t *buffer, uint32_t length) {
    FLASH_DEBUG_PRINTF("read addr0x%x,buff0x%x,len0x%x\r\n",address,(int)buffer,length);
    FLASH_EnterCritical();
    status_t status = flexspi_nor_flash_read_sector(EXAMPLE_FLEXSPI, address,(uint32_t *) buffer, length);
    FLASH_ExitCritical();
    return status;
}

/**
 * @brief  对flash的一页编程，页大小为FLASH_PAGE_SIZE
 * @param  {uint32_t} address : Flash地址，flash存储的第一个字节的地址为0x0，第二个字节的地址为0x1，以此类推
 *                              必须为FLASH_PAGE_SIZE的整数倍
 * @param  {uint8_t*} buffer  : 缓存地址
 * @return {status_t}         : 错误代码，0表示正常
 */
RAMFUNC status_t FLASH_Prog(uint32_t address, uint8_t *buffer) {
    FLASH_DEBUG_PRINTF("prog addr0x%x,buff0x%x\r\n",address,(int)buffer);
    assert(0 == address % FLASH_PAGE_SIZE);
    FLASH_EnterCritical();
    status_t status = flexspi_nor_flash_page_program(EXAMPLE_FLEXSPI, address, (const uint32_t *)buffer);
    FLASH_ExitCritical();
    return status;
}

/**
 * @brief  擦除flash的一个扇区，扇区大小为FLASH_SECTOR_SIZE
 * @param  {uint32_t} address : Flash地址，flash存储的第一个字节的地址为0x0，第二个字节的地址为0x1，以此类推
 *                              必须为FLASH_SECTOR_SIZE的整数倍
 * @return {status_t}         : 错误代码，0表示正常
 */
RAMFUNC status_t FLASH_Erase(uint32_t address) {
    FLASH_DEBUG_PRINTF("erase addr0x%x\r\n",address);
    assert(0 == address % FLASH_SECTOR_SIZE);
    FLASH_EnterCritical();
    status_t status = flexspi_nor_flash_erase_sector(EXAMPLE_FLEXSPI, address);
    FLASH_ExitCritical();
    return status;
}

/**
 * @brief  同步缓存
 * @param  {void} undefined :
 * @return {status_t}       : 错误代码，0表示正常
 */
RAMFUNC status_t FLASH_Sync(void) {
    FLASH_EnterCritical();
    status_t status = 0;
    FLASH_ExitCritical();
    return status;
}

volatile int ICacheIsDisable = 0;
volatile int DCacheIsDisable = 0;
volatile int CriticalNesting = 0;
volatile uint32_t regPrimask;

/**
 * @brief  进入flash读写保护临界区
 * @param  {void} undefined :
 */
RAMFUNC void FLASH_EnterCritical(void) {
    if (CriticalNesting <= 0) { regPrimask = DisableGlobalIRQ();}
    //if (CriticalNesting == 0) { PRINTF("FLASH_EnterCritical\r\n"); }
    if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR)) {
        SCB_InvalidateICache();
        SCB_DisableICache();
        ICacheIsDisable = 1;
    }
    if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR)) {
        SCB_InvalidateDCache();
        SCB_DisableDCache();
        DCacheIsDisable = 1;
    }
    ARM_MPU_Disable();
    CriticalNesting++;
//    FLASH_DEBUG_PRINTF("enter %d\r\n",CriticalNesting);
}

/**
 * @brief  退出flash读写保护临界区
 * @param  {void} undefined :
 */
RAMFUNC void FLASH_ExitCritical(void) {
    CriticalNesting--;
//    FLASH_DEBUG_PRINTF("exit %d\r\n",CriticalNesting);
    if (CriticalNesting <= 0) {
        //PRINTF("FLASH_ExitCritical\r\n");
        EnableGlobalIRQ(regPrimask);
        ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);
        if (ICacheIsDisable == 1) {
            ICacheIsDisable = 0;
            SCB_EnableICache();
        }
        if (DCacheIsDisable == 1) {
            DCacheIsDisable = 0;
            SCB_EnableDCache();
        }
    }
}

RAMFUNC int FLASH_LfsRead(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size) {
    FLASH_EnterCritical();
    int status = FLASH_Read(FLASH_RWADDR_START + c->block_size * block + off, buffer, size);
    FLASH_ExitCritical();
    return status;
}

RAMFUNC int
FLASH_LfsProg(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size) {
    FLASH_EnterCritical();
    int status = 0;
    for (int i = 0; i < size / FLASH_PAGE_SIZE; ++i) {
        status = FLASH_Prog(FLASH_RWADDR_START + c->block_size * block + off + i * FLASH_PAGE_SIZE,
                            (uint8_t *) (i * FLASH_PAGE_SIZE + (uint32_t) buffer));
    }

    FLASH_ExitCritical();
    return status;
}

RAMFUNC int FLASH_LfsErase(const struct lfs_config *c, lfs_block_t block) {
    FLASH_EnterCritical();
    int status = FLASH_Erase(FLASH_RWADDR_START + c->block_size * block);
    FLASH_ExitCritical();
    return status;
}

RAMFUNC int FLASH_LfsSync2(const struct lfs_config *c) {
    FLASH_EnterCritical();
    int status = FLASH_Sync();
    FLASH_ExitCritical();
    return status;
}

int FLASH_LfsGetDefaultConfig(struct lfs_config *lfsc) {
    lfsc->context = NULL;
    lfsc->read = FLASH_LfsRead;
    lfsc->prog = FLASH_LfsProg;
    lfsc->erase = FLASH_LfsErase;
    lfsc->sync = FLASH_LfsSync2;
    lfsc->read_size = 16;
    lfsc->prog_size = FLASH_PAGE_SIZE;
    lfsc->block_size = FLASH_SECTOR_SIZE;
    lfsc->block_count = FLASH_RWSIZE / FLASH_SECTOR_SIZE;
    lfsc->lookahead = 128;
    return 0;
}

