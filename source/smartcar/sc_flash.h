/*
 * sc_flash.h
 *
 *  Created on: 2020年1月16日
 *      Author: 17616
 */

#ifndef SC_FLASH_H_
#define SC_FLASH_H_

#include "fsl_debug_console.h"
#include "lfs.h"

#define FLASH_PAGE_SIZE (256)
#define FLASH_SECTOR_SIZE (4*1024)
#define FLASH_RWADDR_START (0x300000)
#define FLASH_RWSIZE (0x100000)
//#define FLASH_DEBUG_PRINTF PRINTF
#ifndef FLASH_DEBUG_PRINTF
#define FLASH_DEBUG_PRINTF(fmt, ...)
#endif

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
 * @return {int}            : 
 */
int FLASH_Init(void);

/**
 * @brief  flash读
 * @param  {uint32_t} address : Flash地址，flash存储的第一个字节的地址为0x0，第二个字节的地址为0x1，以此类推
 * @param  {uint8_t*} buffer  : 缓存地址
 * @param  {uint32_t} length  : 读出的长度，单位字节
 * @return {int}              : 错误代码
 */
int FLASH_Read(uint32_t address, uint8_t *buffer, uint32_t length);

/**
 * @brief  对flash的一页编程，页大小为FLASH_PAGE_SIZE
 * @param  {uint32_t} address : Flash地址，flash存储的第一个字节的地址为0x0，第二个字节的地址为0x1，以此类推
 *                              必须为FLASH_PAGE_SIZE的整数倍
 * @param  {uint8_t*} buffer  : 缓存地址
 * @return {int}              : 错误代码
 */
int FLASH_Prog(uint32_t address, uint8_t *buffer);

/**
 * @brief  擦除flash的一个扇区，扇区大小为FLASH_SECTOR_SIZE
 * @param  {uint32_t} address : Flash地址，flash存储的第一个字节的地址为0x0，第二个字节的地址为0x1，以此类推
 *                              必须为FLASH_SECTOR_SIZE的整数倍
 * @return {int}              : 错误代码
 */
int FLASH_Erase(uint32_t address);

/**
 * @brief  同步缓存
 * @param  {void} undefined : 
 * @return {int}            : 
 */
int FLASH_Sync(void);


//以下是对littleFS的封装
int FLASH_LfsGetDefaultConfig(struct lfs_config *lfsc);

int FLASH_LfsRead(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);

int FLASH_LfsProg(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);

int FLASH_LfsErase(const struct lfs_config *c, lfs_block_t block);

int FLASH_LfsSync2(const struct lfs_config *c);

#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif /* SC_FLASH_H_ */
