/*
 * task_stats.h
 *
 *  Created on: 2020年1月14日
 *      Author: 17616
 */

#ifndef TASK_STATS_H_
#define TASK_STATS_H_

#include "fsl_debug_console.h"
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "fsl_gpt.h"

#if configCHECK_FOR_STACK_OVERFLOW == 0
#error 使能上面那个检查堆栈溢出的配置
#else

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName);

#endif

#if configUSE_MALLOC_FAILED_HOOK == 0
#error 使能上面那个检查堆栈溢出的配置
#else

void vApplicationMallocFailedHook(void);

#endif

void TaskStatusPrint(void);//打印任务执行情况

void APP_PrintRunFrequency(int32_t run_freq_only);//打印当前运行频率

#if configGENERATE_RUN_TIME_STATS == 0
#error 使能时间统计功能
#else
#define TIMER_GPT GPT2
#define TIMER_GPT_IRQ_ID GPT2_IRQn

void GPT2_IRQHandler(void);//

void TimerInit(void);//初始化计时器，由freertos调用

uint64_t TimerCountGet(void);//返回

uint32_t TimerUsGet(void);//返回从初始化开始经过了多少us

uint32_t TimerMsGet(void);//返回从初始化开始经过了多少ms

void TimerIRQHandler(void);

#endif // configGENERATE_RUN_TIME_STATS==0


#endif /* TASK_STATS_H_ */
