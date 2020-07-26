/**
 * @file    main.c
 * @brief   Application entry point.
 */
#include <source/examples/Unitest.h>
#include <FreeRTOS.h>
#include <task.h>
#include <board/clock_config.h>
#include "cm_backtrace/cm_backtrace.h"
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"


int main(void) {
	BOARD_InitBootPins();//初始化引脚
	BOARD_InitBootClocks();//初始化时钟
	BOARD_InitBootPeripherals();//初始化外设
	BOARD_InitDebugConsole();//初始化console
	PRINTF("\r\n*********\n");
	//CmBacktrace是什么https://github.com/armink/CmBacktrace
	cm_backtrace_init("IMRT10XX.axf", "1.3", "2020.7.29");//初始化MCU错误追踪库
	Unitest();//进入单元测试
	size_t i = 0;
	while (1) {
		i++;
		vTaskDelay(10);//等待10ticks，大概是10ms
		__NOP();
	}
}
