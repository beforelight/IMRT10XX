/*
 * sc_flash.h
 *
 *  Created on: 2020年1月16日
 *      Author: 17616
 */

#ifndef SC_FLASH_H_
#define SC_FLASH_H_


#include "lfs.h"

#define FLASH_PAGE_SIZE (256)
#define FLASH_SECTOR_SIZE (4*1024)
#define FLASH_RWADDR_START (3*1024*1024)
#define FLASH_RWSIZE (1*1024*1024)
//#define FLASH_DEBUG_PRINTF PRINTF
#ifndef FLASH_DEBUG_PRINTF
#define FLASH_DEBUG_PRINTF(fmt, ...)
#endif


#define NOR_CMD_LUT_SEQ_IDX_READID1             10
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS1         11
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE1        12
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR1        13
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE1 14
#define NOR_CMD_LUT_SEQ_IDX_READ_NORMAL1        15


#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/**
 * @brief  进入flash读写保护临界区
 * @param  {void} undefined : 
 */
void FLASH_EnterCritical(void);

/**
 * @brief  退出flash读写保护临界区
 * @param  {void} undefined : 
 */
void FLASH_ExitCritical(void);

/**
 * @brief  初始化
 * @param  {void} undefined : 
 * @return {status_t}       : 错误代码，0表示正常
 */
status_t FLASH_Init(void);

/**
 * @brief  flash读
 * @param  {uint32_t} address : Flash地址，flash存储的第一个字节的地址为0x0，第二个字节的地址为0x1，以此类推
 * @param  {uint8_t*} buffer  : 缓存地址
 * @param  {uint32_t} length  : 读出的长度，单位字节
 * @return {status_t}         : 错误代码，0表示正常
 */
status_t FLASH_Read(uint32_t address, uint8_t *buffer, uint32_t length);

/**
 * @brief  对flash的一页编程，页大小为FLASH_PAGE_SIZE
 * @param  {uint32_t} address : Flash地址，flash存储的第一个字节的地址为0x0，第二个字节的地址为0x1，以此类推
 *                              必须为FLASH_PAGE_SIZE的整数倍
 * @param  {uint8_t*} buffer  : 缓存地址
 * @return {status_t}         : 错误代码，0表示正常
 */
status_t FLASH_Prog(uint32_t address, uint8_t *buffer);

/**
 * @brief  擦除flash的一个扇区，扇区大小为FLASH_SECTOR_SIZE
 * @param  {uint32_t} address : Flash地址，flash存储的第一个字节的地址为0x0，第二个字节的地址为0x1，以此类推
 *                              必须为FLASH_SECTOR_SIZE的整数倍
 * @return {status_t}         : 错误代码，0表示正常
 */
status_t FLASH_Erase(uint32_t address);

/**
 * @brief  同步缓存
 * @param  {void} undefined : 
 * @return {status_t}       : 错误代码，0表示正常
 */
status_t FLASH_Sync(void);


//以下是对littleFS的移植封装，littleFS的使用见示例
int FLASH_LfsGetDefaultConfig(struct lfs_config *lfsc);

int FLASH_LfsRead(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);

int FLASH_LfsProg(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);

int FLASH_LfsErase(const struct lfs_config *c, lfs_block_t block);

int FLASH_LfsSync2(const struct lfs_config *c);

#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif /* SC_FLASH_H_ */
