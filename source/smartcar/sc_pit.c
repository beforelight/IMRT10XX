#include "sc_pit.h"

/**
 * @brief 初始化pit并立即生效，注意各个通道共用同一个中断服务函数
 * 启用PIT_EnableInterrupts
 * 关闭PIT_DisableInterrupts
 * @param  {pit_chnl_t} channel : 通道
 * @param  {uint64_t} us        : 定时中断时间
 */
void PIT_Init2(pit_chnl_t channel, uint64_t us) {
    pit_config_t pitConfig;

    /*默认初始化数据*/
    PIT_GetDefaultConfig(&pitConfig);
    static int pitIsInit = 0;
    if (pitIsInit == 0) {
        /* Init pit module */
        PIT_Init(PIT, &pitConfig);
        pitIsInit = 1;
    }



    /*设置通道的计时器周期*/
    PIT_SetTimerPeriod(PIT, channel, USEC_TO_COUNT(us, CLOCK_GetFreq(kCLOCK_PerClk)));

    /*启用通道的计时器中断*/
    PIT_EnableInterrupts(PIT, channel, kPIT_TimerInterruptEnable);

    NVIC_SetPriority(PIT_IRQn, 1);

    /*开定时总中断*/
    EnableIRQ(PIT_IRQn);

    /*定时器开始*/
    PIT_StartTimer(PIT, channel);
}

////中断服务函数（不准直接改名字可以用define改名字）
////注意四个pit通道共用一个中断服务函数
//void PIT_IRQHandler(void)
//{
//	/*清除中断标志位 （要用的时候解注释，通道可换）*/
//	if (PIT_GetStatusFlags(PIT, kPIT_Chnl_0) == kPIT_TimerFlag)
//	{
//		PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
//	}
//	/*中断服务函数内容*/
//	__DSB();
//	/*DSB--数据同步屏障
//	 * 作用：因为CPU时钟的主频大于IP总线时钟故会出现中断标志位还未被清掉时中断服务函数中的内容会执行完，而使得程序一遍一遍的进入中断服务函数，
//	 * 而DSB则是为了预防这一情况的发生。*/
//}
