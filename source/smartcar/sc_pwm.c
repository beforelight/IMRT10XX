/*
 * sc_pwm.h
 *
 *  Created on: 2020年1月26日
 *      Author: 17616
 */
#include "sc_pwm.h"

#define PWM_SRC_CLK_FREQ CLOCK_GetFreq(kCLOCK_IpgClk) //获得IPG时钟

pwm_module_control_t PWM_Module2Module(pwm_submodule_t x) {
    if (x == kPWM_Module_0) {
        return kPWM_Control_Module_0;
    } else if (x == kPWM_Module_1) {
        return kPWM_Control_Module_1;
    } else if (x == kPWM_Module_2) {
        return kPWM_Control_Module_2;
    } else if (x == kPWM_Module_3) {
        return kPWM_Control_Module_3;
    } else {
        assert(0);
        return 0;
    }
}

/**
 * @brief 初始化pwm模块并立即生效
 * @param  {pwm_t* []} pwmList : pwm_t指针数组，最后一项必须为NULL
 */
void PWM_Init2(pwm_t *pwmList[]) {
    uint32_t pwmSourceClockInHz;
    pwm_signal_param_t pwmSignal[2];        //pwm属性
    pwmSourceClockInHz = PWM_SRC_CLK_FREQ;
    //检查初始化列表里面有多少个
    size_t L = 0;
    while (pwmList[L] != NULL) {
        L++;
    }


    //开始初始化
    pwm_config_t pwmConfig;
    PWM_GetDefaultConfig(&pwmConfig);
    /*
     * pwmConfig.enableDebugMode = false;							 //调试模式下暂停
     * pwmConfig.enableWait = false;								 //等待模式下暂停
     * pwmConfig.reloadSelect = kPWM_LocalReload;					 //使用本地再加载
     * pwmConfig.clockSource = kPWM_BusClock;						 //使用IPBus作为时钟
     * pwmConfig.prescale = kPWM_Prescale_Divide_1; 				 //分频为1
     * pwmConfig.initializationControl = kPWM_Initialize_LocalSync;  //本地同步导致初始化
     * pwmConfig.forceTrigger = kPWM_Force_Local;					 //CTRL2[FORCE]信号触发强制输出
     * pwmConfig.reloadFrequency = kPWM_LoadEveryOportunity;		 //重载频率
     * pwmConfig.reloadLogic = kPWM_ReloadImmediate;				 //设置LDOK时，加载值
     * pwmConfig.pairOperation = kPWM_Independent;					 //PWMA，PWMB各自独立输出
     */
    /*使用全循环重新加载*/
    pwmConfig.reloadLogic = kPWM_ReloadPwmFullCycle;
//    初始化pwm
    for (size_t i = 0; i < L; i++) {
        pwmList[i]->base->SM[pwmList[i]->subModule].DISMAP[0] = 0;//关闭错误处理
        pwmList[i]->base->SM[pwmList[i]->subModule].DISMAP[1] = 0;
        assert(kStatus_Success == PWM_Init(pwmList[i]->base, pwmList[i]->subModule, &pwmConfig));
    }

//    初始化pwm-submodule
    for (size_t i = 0; i < L; i++) {

        PWM_SetupForceSignal(pwmList[i]->base, pwmList[i]->subModule, kPWM_PwmA, kPWM_UsePwm);
        PWM_SetupForceSignal(pwmList[i]->base, pwmList[i]->subModule, kPWM_PwmB, kPWM_UsePwm);
        pwmSignal[0].pwmChannel = kPWM_PwmA;        //通道
        pwmSignal[0].level = pwmList[i]->level;    //以高电平表示
        pwmSignal[0].dutyCyclePercent = pwmList[i]->dutyA;            //占空比
        pwmSignal[0].deadtimeValue = 0;    //死区时间，仅互补模式有效

        pwmSignal[1].pwmChannel = kPWM_PwmB;        //通道
        pwmSignal[1].level = pwmList[i]->level;    //以高电平表示
        pwmSignal[1].dutyCyclePercent = pwmList[i]->dutyB;            //占空比
        pwmSignal[1].deadtimeValue = 0;    //死区时间，仅互补模式有效
        assert(kStatus_Success ==
               PWM_SetupPwm(pwmList[i]->base, pwmList[i]->subModule, pwmSignal, 2, PWM_MODE, pwmList[i]->Freq,
                            pwmSourceClockInHz));
    }

    //打开pwm计数器开始生成pwm
    for (size_t i = 0; i < L; i++) {
        /* Set the load okay bit for all submodules to load registers from their buffer */
        PWM_SetPwmLdok(pwmList[i]->base, PWM_Module2Module(pwmList[i]->subModule), true);

        /* Start the PWM generation from Submodules 0, 1 and 2 */
        PWM_StartTimer(pwmList[i]->base, PWM_Module2Module(pwmList[i]->subModule));
    }
}

static inline uint16_t PWM_GetComplementU16(uint16_t value) {
    return (~value + 1U);
}

//重载官方库，可以使用高精度pwm占空比
void
PWM_UpdatePwmDutycycle2(PWM_Type *base, pwm_submodule_t subModule, pwm_channels_t pwmSignal, pwm_mode_t currPwmMode,
                        float dutyCyclePercent) {
    assert(dutyCyclePercent <= 100U);
    assert((uint16_t) pwmSignal < 2U);
    uint16_t pulseCnt = 0, pwmHighPulse = 0;
    uint16_t modulo = 0;

    switch (currPwmMode) {
        case kPWM_SignedCenterAligned:
            modulo = base->SM[subModule].VAL1;
            pulseCnt = modulo * 2U;
            /* Calculate pulse width */
            pwmHighPulse = (pulseCnt * dutyCyclePercent) / 100U;

            /* Setup the PWM dutycycle */
            if (pwmSignal == kPWM_PwmA) {
                base->SM[subModule].VAL2 = PWM_GetComplementU16(pwmHighPulse / 2U);
                base->SM[subModule].VAL3 = (pwmHighPulse / 2U);
            } else {
                base->SM[subModule].VAL4 = PWM_GetComplementU16(pwmHighPulse / 2U);
                base->SM[subModule].VAL5 = (pwmHighPulse / 2U);
            }
            break;
        case kPWM_CenterAligned:
            pulseCnt = base->SM[subModule].VAL1;
            /* Calculate pulse width */
            pwmHighPulse = (pulseCnt * dutyCyclePercent) / 100U;

            /* Setup the PWM dutycycle */
            if (pwmSignal == kPWM_PwmA) {
                base->SM[subModule].VAL2 = ((pulseCnt - pwmHighPulse) / 2U);
                base->SM[subModule].VAL3 = ((pulseCnt + pwmHighPulse) / 2U);
            } else {
                base->SM[subModule].VAL4 = ((pulseCnt - pwmHighPulse) / 2U);
                base->SM[subModule].VAL5 = ((pulseCnt + pwmHighPulse) / 2U);
            }
            break;
        case kPWM_SignedEdgeAligned:
            modulo = base->SM[subModule].VAL1;
            pulseCnt = modulo * 2U;
            /* Calculate pulse width */
            pwmHighPulse = (pulseCnt * dutyCyclePercent) / 100U;

            /* Setup the PWM dutycycle */
            if (pwmSignal == kPWM_PwmA) {
                base->SM[subModule].VAL2 = PWM_GetComplementU16(modulo);
                base->SM[subModule].VAL3 = PWM_GetComplementU16(modulo) + pwmHighPulse;
            } else {
                base->SM[subModule].VAL4 = PWM_GetComplementU16(modulo);
                base->SM[subModule].VAL5 = PWM_GetComplementU16(modulo) + pwmHighPulse;
            }
            break;
        case kPWM_EdgeAligned:
            pulseCnt = base->SM[subModule].VAL1;
            /* Calculate pulse width */
            pwmHighPulse = (pulseCnt * dutyCyclePercent) / 100U;

            /* Setup the PWM dutycycle */
            if (pwmSignal == kPWM_PwmA) {
                base->SM[subModule].VAL2 = 0;
                base->SM[subModule].VAL3 = pwmHighPulse;
            } else {
                base->SM[subModule].VAL4 = 0;
                base->SM[subModule].VAL5 = pwmHighPulse;
            }
            break;
        default:
            assert(false);
            break;
    }
}

/**
 * @brief 修改pwm A/B通道占空比
 * @param  {pwm_t*} p :
 */
void PWM_Change(pwm_t *p) {
    if (p->dutyA > 100) { p->dutyA = 100; }
    else if (p->dutyA < 0) { p->dutyA = 0; }

    if (p->dutyB > 100) { p->dutyB = 100; }
    else if (p->dutyB < 0) { p->dutyB = 0; }

    PWM_UpdatePwmDutycycle2(p->base, p->subModule, kPWM_PwmA, PWM_MODE, p->dutyA);
    PWM_UpdatePwmDutycycle2(p->base, p->subModule, kPWM_PwmB, PWM_MODE, p->dutyB);
    PWM_SetPwmLdok(p->base, PWM_Module2Module(p->subModule), true);
}