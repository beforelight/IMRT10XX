/*
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file    board.h
 * @brief   Board initialization header file.
 */

/* This is a template for board specific configuration created by MCUXpresso IDE Project Wizard.*/

#ifndef _BOARD_H_
#define _BOARD_H_

/**
 * @brief	The board name 
 */
#define BOARD_NAME "imxRT1052BoardV1.0"

#include "fsl_gpio.h"
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

#define BOARD_FLASH_SIZE (0x300000)

#if 1    //链接过程控制
#define ALIGN(n) __attribute__((aligned(n)))

#define RAMFUNC_ITC __attribute__((section(".ramfunc.$SRAM_ITC")))
#define RAMFUNC_OC __attribute__((section(".ramfunc.$SRAM_OC")))
#define RAMFUNC_SDRAM __attribute__((section(".ramfunc.$SRAM_SDRAM")))
#define RAMFUNC_SDRAM_NOCACHE __attribute__((section(".ramfunc.$SDRAM_NOCACHE")))

#define DATA_DTC __attribute__((section(".data.$SRAM_DTC")))
#define DATA_OC __attribute__((section(".data.$SRAM_OC")))
#define DATA_SDRAM __attribute__((section(".data.$SDRAM")))
#define DATA_SDRAM_NOCACHE __attribute__((section(".data.$SDRAM_NOCACHE")))

#define BSS_DTC __attribute__((section(".bss.$SRAM_DTC")))
#define BSS_OC __attribute__((section(".bss.$SRAM_OC")))
#define BSS_SDRAM __attribute__((section(".bss.$SDRAM")))
#define BSS_SDRAM_NOCACHE __attribute__((section(".bss.$SDRAM_NOCACHE")))
#endif


#if 1 //sd卡相关
#define BOARD_SD_SUPPORT_180V 0U
#define BOARD_USDHC_CD_GPIO_BASE GPIO2
#define BOARD_USDHC_CD_GPIO_PIN 28
#define BOARD_USDHC_CD_PORT_IRQ GPIO2_Combined_16_31_IRQn
//#define BOARD_USDHC_CD_PORT_IRQ_HANDLER GPIO2_Combined_16_31_IRQHandler
#define BOARD_USDHC1_BASEADDR USDHC1
#define BOARD_USDHC2_BASEADDR USDHC2
#define BOARD_USDHC1_CLK_FREQ (CLOCK_GetSysPfdFreq(kCLOCK_Pfd0) / (CLOCK_GetDiv(kCLOCK_Usdhc1Div) + 1U))
#define BOARD_USDHC2_CLK_FREQ (CLOCK_GetSysPfdFreq(kCLOCK_Pfd0) / (CLOCK_GetDiv(kCLOCK_Usdhc2Div) + 1U))
#define BOARD_SD_HOST_BASEADDR BOARD_USDHC1_BASEADDR
#define BOARD_SD_HOST_CLK_FREQ BOARD_USDHC1_CLK_FREQ
#define BOARD_SD_HOST_IRQ USDHC1_IRQn
#define BOARD_MMC_HOST_BASEADDR BOARD_USDHC2_BASEADDR
#define BOARD_MMC_HOST_CLK_FREQ BOARD_USDHC2_CLK_FREQ
#define BOARD_MMC_HOST_IRQ USDHC2_IRQn
#define BOARD_USDHC_CD_GPIO_INIT()                                                          \
    {                                                                                       \
        gpio_pin_config_t sw_config = {                                                     \
            kGPIO_DigitalInput,                                                             \
            0,                                                                              \
            kGPIO_IntRisingOrFallingEdge,                                                   \
        };                                                                                  \
        GPIO_PinInit(BOARD_USDHC_CD_GPIO_BASE, BOARD_USDHC_CD_GPIO_PIN, &sw_config);        \
        /*GPIO_PortEnableInterrupts(BOARD_USDHC_CD_GPIO_BASE, 1U << BOARD_USDHC_CD_GPIO_PIN);*/ \
        /*GPIO_PortClearInterruptFlags(BOARD_USDHC_CD_GPIO_BASE, ~0);*/                         \
    }
#define BOARD_USDHC_CD_STATUS() (GPIO_PinRead(BOARD_USDHC_CD_GPIO_BASE, BOARD_USDHC_CD_GPIO_PIN))
#define BOARD_USDHC_CD_INTERRUPT_STATUS() (GPIO_PortGetInterruptFlags(BOARD_USDHC_CD_GPIO_BASE))
#define BOARD_USDHC_CD_CLEAR_INTERRUPT(flag) (GPIO_PortClearInterruptFlags(BOARD_USDHC_CD_GPIO_BASE, flag))
#define BOARD_USDHC_MMCCARD_POWER_CONTROL_INIT()
#define BOARD_USDHC_SDCARD_POWER_CONTROL_INIT()
#define BOARD_USDHC_SDCARD_POWER_CONTROL(enable)
void BOARD_SD_Pin_Config(uint32_t speed, uint32_t strength);
#endif

void entrance2main(void);
void BOARD_InitDebugConsole(void);
void BOARD_ConfigMPU(void);//初始化内存保护单元和cache
void BOARD_ConfigVectors(void);//将中断向量表加载到内存中

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */


