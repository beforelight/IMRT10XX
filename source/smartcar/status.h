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
/**
 * @brief   当freertos任务堆栈溢出的时候，会调用这个函数来通知
 * @param  {TaskHandle_t} xTask : 溢出的任务句柄
 * @param  {char*} pcTaskName   : 溢出的任务名字
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName);

#endif

#if configUSE_MALLOC_FAILED_HOOK == 0
#error 使能上面那个检查堆栈溢出的配置
#else
/**
 * @brief   当freertos内置的动态内存分配分配失败的时候会调用这个函数来通知
 * @param  {void} undefined : 
 */
void vApplicationMallocFailedHook(void);

#endif
/**
 * @brief   打印任务执行情况
 * @param  {void} undefined : 
 */
void TaskStatusPrint(void);
/**
 * @brief   打印当前单片机运行的频率
 * @param  {int32_t} run_freq_only : 0详细其他不详细
 */
void APP_PrintRunFrequency(int32_t run_freq_only);

#if configGENERATE_RUN_TIME_STATS == 0
#error 使能时间统计功能
#else
#define TIMER_GPT GPT2
#define TIMER_GPT_IRQ_ID GPT2_IRQn
/**
 * @brief   GPT2中断函数
 * @param  {void} undefined : 
 */
void GPT2_IRQHandler(void);

/**
 * @brief 初始化通用定时器，由freertos自动调用
 * @param  {void} undefined : 
 */
void TimerInit(void);

/**
 * @brief   返回自计时器初始化以来经历的脉冲数
 * @param  {void} undefined : 
 * @return {uint64_t}       : 
 */
uint64_t TimerCountGet(void);

/**
 * @brief   返回自计时器初始化以来经历的时间
 * @param  {void} undefined : 
 * @return {uint32_t}       : us
 */
uint32_t TimerUsGet(void);

/**
 * @brief   返回自计时器初始化以来经历的时间
 * @param  {void} undefined : 
 * @return {uint32_t}       : ms
 */
uint32_t TimerMsGet(void);
/**
 * @brief   计时器中断入口
 * @param  {void} undefined : 
 */
void TimerIRQHandler(void);

#endif // configGENERATE_RUN_TIME_STATS==0


#endif /* TASK_STATS_H_ */
