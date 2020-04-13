#include "status.h"
/*
 * task_stats.c
 *
 *  Created on: 2020年1月14日
 *      Author: 17616
 */

void vApplicationStackOverflowHook(TaskHandle_t xTask, char* pcTaskName)
{
	PRINTF("task %s over flow!!\r\n", pcTaskName);

	vTaskEndScheduler();
	__disable_irq();
	while (1) { PRINTF("task %s over flow!!\r\n", pcTaskName); }
}

void vApplicationMallocFailedHook(void)
{
	PRINTF("malloc fail!!\r\n");
	vTaskEndScheduler();
	__disable_irq();
	while (1) { PRINTF("malloc fail!!\r\n"); }
}

void TaskStatusPrint(void)
{
	int buff_size = 4000;
	char* pWriteBuffer = pvPortMalloc(buff_size * sizeof(char));
	pWriteBuffer[buff_size] = 0;
	vTaskList(pWriteBuffer);
	if (pWriteBuffer[buff_size-1] != 0)
	{
		__disable_irq();
		while (1) { PRINTF("TASK_StatsPrint fail!!\r\n"); }
	}
	PRINTF("=======================================================\r\n");
	PRINTF("任务名           任务状态   优先级      剩余栈   任务序号\r\n");
	PRINTF("%s\n", pWriteBuffer);
#if configGENERATE_RUN_TIME_STATS==1
	vTaskGetRunTimeStats(pWriteBuffer);
	PRINTF("\r\n任务名            运行计数              使用率\r\n");
	if (pWriteBuffer[buff_size - 1] != 0)
	{
		__disable_irq();
		while (1) { PRINTF("TASK_StatsPrint fail!!\r\n"); }
	}
	PRINTF("%s\r\n", pWriteBuffer);

#endif // configGENERATE_RUN_TIME_STATS==1

	vPortFree(pWriteBuffer);


}

uint32_t TimerCountH;
uint32_t TImerClk;
uint32_t TImerClkPerUs;
uint32_t TImerClkPerMs;
void TimerInit(void)
{
	TimerCountH = 0;
	TImerClk = CLOCK_GetFreq(kCLOCK_PerClk);
	TImerClkPerMs = TImerClk / 1000;
	TImerClkPerUs = TImerClk / 1000000;
//	uint32_t gptFreq;
	gpt_config_t gptConfig;
	GPT_GetDefaultConfig(&gptConfig);
	gptConfig.enableFreeRun = true;
	GPT_Init(TIMER_GPT, &gptConfig);
	GPT_SetClockDivider(TIMER_GPT, 1);
	GPT_EnableInterrupts(TIMER_GPT, kGPT_RollOverFlagInterruptEnable);
	EnableIRQ(TIMER_GPT_IRQ_ID);
	GPT_StartTimer(TIMER_GPT);
}
uint64_t TimerCountGet(void)
{
	return ((uint64_t)TimerCountH << 32) | (uint64_t)GPT_GetCurrentTimerCount(TIMER_GPT);
}

uint32_t TimerUsGet(void)
{
	return TimerCountGet() / TImerClkPerUs;
}

uint32_t TimerMsGet(void)
{
	return TimerCountGet() / TImerClkPerMs;
}

void TimerIRQHandler(void)
{
	++TimerCountH;
}
void GPT2_IRQHandler(void)
{
	GPT_ClearStatusFlags(TIMER_GPT, kGPT_RollOverFlagInterruptEnable);
    TimerIRQHandler();
}


void APP_PrintRunFrequency(int32_t run_freq_only)
{
	PRINTF("\r\n");
	PRINTF("***********************************************************\r\n");
	PRINTF("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
	PRINTF("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
	PRINTF("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
	PRINTF("IPG:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_IpgClk));
	PRINTF("PER:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_PerClk));
	PRINTF("OSC:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_OscClk));
	PRINTF("RTC:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_RtcClk));
	if (!run_freq_only)
	{
		PRINTF("ARMPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_ArmPllClk));
		PRINTF("USB1PLL:         %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllClk));
		PRINTF("USB1PLLPFD0:     %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk));
		PRINTF("USB1PLLPFD1:     %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd1Clk));
		PRINTF("USB1PLLPFD2:     %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd2Clk));
		PRINTF("USB1PLLPFD3:     %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb1PllPfd3Clk));
		PRINTF("USB2PLL:         %d Hz\r\n", CLOCK_GetFreq(kCLOCK_Usb2PllClk));
		PRINTF("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
		PRINTF("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
		PRINTF("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
		PRINTF("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
		PRINTF("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));
		PRINTF("ENETPLL0:        %d Hz\r\n", CLOCK_GetFreq(kCLOCK_EnetPll0Clk));
		PRINTF("ENETPLL1:        %d Hz\r\n", CLOCK_GetFreq(kCLOCK_EnetPll1Clk));
		PRINTF("AUDIOPLL:        %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AudioPllClk));
		PRINTF("VIDEOPLL:        %d Hz\r\n", CLOCK_GetFreq(kCLOCK_VideoPllClk));
	}
	PRINTF("***********************************************************\r\n");
	PRINTF("\r\n");
}

//请把下列添加到task.c底下以支持cmb
//		/*-----------------------------------------------------------*/
//		/*< Support For CmBacktrace >*/
//uint32_t* vTaskStackAddr()
//{
//	return pxCurrentTCB->pxStack;
//}
//
//uint32_t vTaskStackSize()
//{
//#if ( portSTACK_GROWTH > 0 )
//
//	return (pxNewTCB->pxEndOfStack - pxNewTCB->pxStack + 1);
//
//#else /* ( portSTACK_GROWTH > 0 )*/
//
//	//return pxCurrentTCB->uxSizeOfStack;
//	return  (pxCurrentTCB->pxEndOfStack - pxCurrentTCB->pxStack + 1);;
//
//#endif /* ( portSTACK_GROWTH > 0 )*/
//}
//
//char* vTaskName()
//{
//	return pxCurrentTCB->pcTaskName;
//}
///*-----------------------------------------------------------*/
