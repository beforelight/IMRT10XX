/**
 * @file    main.c
 * @brief   Application entry point.
 */
#include <source/examples/Unitest.h>
#include <FreeRTOS.h>
#include <task.h>
#include "fsl_debug_console.h"

//注意，最先运行的函数是entrance2main，其中初始化了引脚，时钟，外设，console，CmBacktrace，RTOS
int main(void) {
	PRINTF("\r\n****IMRT10XX****\r\n");
	Unitest(default_item_list);//进入单元测试
	size_t i = 0;
	while (1) {
		i++;
		vTaskDelay(10);//等待10ticks，大概是10ms
		__NOP();
	}
}
