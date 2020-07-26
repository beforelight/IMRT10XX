/**
 * @file    main.c
 * @brief   Application entry point.
 */
#include <source/examples/Unitest.h>
#include <FreeRTOS.h>
#include <task.h>
#include "fsl_debug_console.h"


int main(void) {
    //注意，最先运行的函数是entrance2main，其中初始化了引脚，时钟，外设，console，CmBacktrace，RTOS
	PRINTF("\r\n****IMRT10XX****\r\n");
	Unitest();//进入单元测试
	size_t i = 0;
	while (1) {
		i++;
		vTaskDelay(10);//等待10ticks，大概是10ms
		__NOP();
	}
}
