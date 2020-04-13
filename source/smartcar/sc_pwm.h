/*
 * sc_pwm.h
 *
 *  Created on: 2020年1月13日
 *      Author: 17616
 */

#ifndef SC_PWM_H_
#define SC_PWM_H_

#include "fsl_debug_console.h"
#include "fsl_pwm.h"

#define PWM_MODE kPWM_SignedCenterAligned
typedef struct _pwm {
    PWM_Type *base;
    pwm_submodule_t subModule;
    uint32_t Freq;//频率 单位Hz
    float dutyA;  /*!< PWM pulse width, value should be between 0 to 100 ... 0=inactive signal(0% duty cycle)... 100=always active signal (100% duty cycle)*/
    float dutyB;  /*!< PWM pulse width, value should be between 0 to 100 ... 0=inactive signal(0% duty cycle)... 100=always active signal (100% duty cycle)*/
    pwm_level_select_t level;  /*!< PWM output active level select 高电平输出还是低电平输出*/
} pwm_t;

/**
 * @brief 初始化pwm模块并立即生效
 * @param  {pwm_t* []} pwmList : pwm_t指针数组，最后一项必须为NULL以确定有多少组pwm被初始化
 */
void PWM_Init2(pwm_t *pwmList[]);

/**
 * @brief 修改pwm A/B通道占空比（将结构体中的占空比同步给外设）
 * @param  {pwm_t*} p :
 */
void PWM_Change(pwm_t *p);


#endif /* SC_PWM_H_ */
