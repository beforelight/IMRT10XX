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
  * @file    board.c
  * @brief   Board initialization file.
  */

  /* This is a template for board specific configuration created by MCUXpresso IDE Project Wizard.*/

#include <stdint.h>
#include "board.h"
#include <CMSIS/cmsis_gcc.h>
#include <fsl_common.h>
#include <fsl_debug_console.h>
#include <drivers/fsl_iomuxc.h>
#include <task.h>

BSS_DTC uint32_t UserVectors[256] ALIGN(1024);

void BOARD_ConfigVectors(void) {
	__disable_irq();
	memcpy(UserVectors, (void*)SCB->VTOR, 256 * 4);
	SCB->VTOR = (uint32_t)UserVectors;
	__enable_irq();
}

/* Get debug console frequency. */
uint32_t BOARD_DebugConsoleSrcFreq(void) {
	uint32_t freq;

	/* To make it simple, we assume default PLL and divider settings, and the only variable
	   from application is use PLL3 source or OSC source */
	if (CLOCK_GetMux(kCLOCK_UartMux) == 0) /* PLL3 div6 80M */
	{
		freq = (CLOCK_GetPllFreq(kCLOCK_PllUsb1) / 6U) / (CLOCK_GetDiv(kCLOCK_UartDiv) + 1U);
	}
	else {
		freq = CLOCK_GetOscFreq() / (CLOCK_GetDiv(kCLOCK_UartDiv) + 1U);
	}

	return freq;
}


void BOARD_InitDebugConsole(void) {
	/*int result =*/ DbgConsole_Init(1, 1152000, kSerialPort_Uart, BOARD_DebugConsoleSrcFreq());
}


/* MPU configuration. */
void BOARD_ConfigMPU(void) {
	extern uint32_t __base_SDRAM_NOCACHE;
	extern uint32_t __top_SDRAM_NOCACHE;
	uint32_t nonCacheStart = (uint32_t)(&__base_SDRAM_NOCACHE);
	uint32_t size = (uint32_t)(&__top_SDRAM_NOCACHE) - nonCacheStart;
	uint32_t i = 0;
	/* Disable I cache and D cache */
	if (SCB_CCR_IC_Msk == (SCB_CCR_IC_Msk & SCB->CCR)) {
		SCB_DisableICache();
	}
	if (SCB_CCR_DC_Msk == (SCB_CCR_DC_Msk & SCB->CCR)) {
		SCB_DisableDCache();
	}

	/* Disable MPU */
	ARM_MPU_Disable();

	/* MPU configure:
	 * Use ARM_MPU_RASR(DisableExec, AccessPermission, TypeExtField, IsShareable, IsCacheable, IsBufferable,
	 * SubRegionDisable, Size)
	 * API in mpu_armv7.h.
	 * param DisableExec       Instruction access (XN) disable bit,0=instruction fetches enabled, 1=instruction fetches
	 * disabled.
	 * param AccessPermission  Data access permissions, allows you to configure read/write access for User and
	 * Privileged mode.
	 *      Use MACROS defined in mpu_armv7.h:
	 * ARM_MPU_AP_NONE/ARM_MPU_AP_PRIV/ARM_MPU_AP_URO/ARM_MPU_AP_FULL/ARM_MPU_AP_PRO/ARM_MPU_AP_RO
	 * Combine TypeExtField/IsShareable/IsCacheable/IsBufferable to configure MPU memory access attributes.
	 *  TypeExtField  IsShareable  IsCacheable  IsBufferable   Memory Attribtue    Shareability        Cache
	 *     0             x           0           0             Strongly Ordered    shareable
	 *     0             x           0           1              Device             shareable
	 *     0             0           1           0              Normal             not shareable   Outer and inner write
	 * through no write allocate
	 *     0             0           1           1              Normal             not shareable   Outer and inner write
	 * back no write allocate
	 *     0             1           1           0              Normal             shareable       Outer and inner write
	 * through no write allocate
	 *     0             1           1           1              Normal             shareable       Outer and inner write
	 * back no write allocate
	 *     1             0           0           0              Normal             not shareable   outer and inner
	 * noncache
	 *     1             1           0           0              Normal             shareable       outer and inner
	 * noncache
	 *     1             0           1           1              Normal             not shareable   outer and inner write
	 * back write/read acllocate
	 *     1             1           1           1              Normal             shareable       outer and inner write
	 * back write/read acllocate
	 *     2             x           0           0              Device              not shareable
	 *  Above are normal use settings, if your want to see more details or want to config different inner/outter cache
	 * policy.
	 *  please refer to Table 4-55 /4-56 in arm cortex-M7 generic user guide <dui0646b_cortex_m7_dgug.pdf>
	 * param SubRegionDisable  Sub-region disable field. 0=sub-region is enabled, 1=sub-region is disabled.
	 * param Size              Region size of the region to be configured. use ARM_MPU_REGION_SIZE_xxx MACRO in
	 * mpu_armv7.h.
	 */

	 /* Region 0 setting: Memory with Device type, not shareable, non-cacheable. */
	MPU->RBAR = ARM_MPU_RBAR(0, 0x80000000U);
	MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);

	/* Region 1 setting: Memory with Device type, not shareable,  non-cacheable. */
	MPU->RBAR = ARM_MPU_RBAR(1, 0x60000000U);
	MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_512MB);

#if defined(XIP_EXTERNAL_FLASH) && (XIP_EXTERNAL_FLASH == 1)
	/* Region 2 setting: Memory with Normal type, not shareable, outer/inner write back. */
	MPU->RBAR = ARM_MPU_RBAR(2, 0x60000000U);
	//MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_4MB);
	MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_RO, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_4MB);
#endif

	/* Region 3 setting: Memory with Device type, not shareable, non-cacheable. */
	MPU->RBAR = ARM_MPU_RBAR(3, 0x00000000U);
	MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 2, 0, 0, 0, 0, ARM_MPU_REGION_SIZE_1GB);

	/* Region 4 setting: Memory with Normal type, not shareable, outer/inner write back */
	MPU->RBAR = ARM_MPU_RBAR(4, 0x00000000U);
	MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_128KB);

	/* Region 5 setting: Memory with Normal type, not shareable, outer/inner write back */
	MPU->RBAR = ARM_MPU_RBAR(5, 0x20000000U);
	MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_128KB);

	/* Region 6 setting: Memory with Normal type, not shareable, outer/inner write back */
	MPU->RBAR = ARM_MPU_RBAR(6, 0x20200000U);
	MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_256KB);

	/* Region 7 setting: Memory with Normal type, not shareable, outer/inner write back */
	MPU->RBAR = ARM_MPU_RBAR(7, 0x80000000U);
	MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 0, 0, 1, 1, 0, ARM_MPU_REGION_SIZE_16MB);

	while ((size >> i) > 0x1U) {
		i++;
	}

	if (i != 0) {
		/* The MPU region size should be 2^N, 5<=N<=32, region base should be multiples of size. */
		assert(!(nonCacheStart % size));
		assert(size == (uint32_t)(1 << i));
		assert(i >= 5);

		/* Region 8 setting: Memory with Normal type, not shareable, non-cacheable */
		MPU->RBAR = ARM_MPU_RBAR(8, nonCacheStart);
		MPU->RASR = ARM_MPU_RASR(0, ARM_MPU_AP_FULL, 1, 0, 0, 0, 0, i - 1);
	}

	/* Enable MPU */
	ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);

	/* Enable I cache and D cache */
	SCB_EnableDCache();
	SCB_EnableICache();
}

void BOARD_SD_Pin_Config(uint32_t speed, uint32_t strength)
{
	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_00_USDHC1_CMD,
		IOMUXC_SW_PAD_CTL_PAD_SPEED(speed) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |
		IOMUXC_SW_PAD_CTL_PAD_DSE(strength));
	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_01_USDHC1_CLK,
		IOMUXC_SW_PAD_CTL_PAD_SPEED(speed) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(0) |
		IOMUXC_SW_PAD_CTL_PAD_DSE(strength));
	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_02_USDHC1_DATA0,
		IOMUXC_SW_PAD_CTL_PAD_SPEED(speed) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |
		IOMUXC_SW_PAD_CTL_PAD_DSE(strength));
	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_03_USDHC1_DATA1,
		IOMUXC_SW_PAD_CTL_PAD_SPEED(speed) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |
		IOMUXC_SW_PAD_CTL_PAD_DSE(strength));
	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_04_USDHC1_DATA2,
		IOMUXC_SW_PAD_CTL_PAD_SPEED(speed) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |
		IOMUXC_SW_PAD_CTL_PAD_DSE(strength));
	IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_B0_05_USDHC1_DATA3,
		IOMUXC_SW_PAD_CTL_PAD_SPEED(speed) | IOMUXC_SW_PAD_CTL_PAD_SRE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_PKE_MASK | IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
		IOMUXC_SW_PAD_CTL_PAD_HYS_MASK | IOMUXC_SW_PAD_CTL_PAD_PUS(1) |
		IOMUXC_SW_PAD_CTL_PAD_DSE(strength));
}

BSS_DTC uint8_t heap_heap1[64 * 1024] ALIGN(8);
BSS_OC uint8_t heap_heap2[128 * 1024] ALIGN(8);
BSS_SDRAM uint8_t heap_heap3[4 * 1024 * 1024] ALIGN(8);
const HeapRegion_t xHeapRegions[] =
{
		{&heap_heap1[0], sizeof(heap_heap1)},
		{&heap_heap2[0], sizeof(heap_heap2)},
		{&heap_heap3[0], sizeof(heap_heap3)},
		{NULL,           0} /* Terminates the array. */
};

//*****************************************************************************
// The entry point for the application.
// __main() is the entry point for Redlib based applications
// main() is the entry point for Newlib based applications
//*****************************************************************************
#if defined (__REDLIB__)
extern void __main(void);
#endif
extern int main(void);
TaskHandle_t main_task_handle;
void main_task(void* pv)
{
#if defined (__REDLIB__)
	// Call the Redlib library, which in turn calls main()
	__main();
#else
	main();
#endif
	while (1)
	{
		vTaskDelay(1);
	}
}
void entrance2main(void) {
	vPortDefineHeapRegions(xHeapRegions);
	xTaskCreate(main_task, "main task", 2048, NULL, 2, &main_task_handle);
	vTaskStartScheduler();
	return;
}


