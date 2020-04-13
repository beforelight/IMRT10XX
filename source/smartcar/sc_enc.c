/*
 * sc_enc.c
 *
 *  Created on: 2020年1月28日
 *      Author: 17616
 */
#include "sc_enc.h"
/*
 * brief ENC初始化
 *
 * param base ENC peripheral base address.（such as "ENC1","ENC2","ENC3"，"ENC4"）
 */
void ENC_Init_t(ENC_Type *base)
{
	enc_config_t mEncConfigStruct;

	//XBARA针对IO口功能的二次复用,管理ENC
    XBARA_Init(XBARA1);
    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn21, kXBARA1_OutputEnc1PhaseAInput);
    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn22, kXBARA1_OutputEnc1PhaseBInput);
    XBARA_SetSignalsConnection(XBARA1, kXBARA1_InputIomuxXbarIn23, kXBARA1_OutputEnc1Index);

    /* Initialize the ENC module. */
    ENC_GetDefaultConfig(&mEncConfigStruct);//得到默认设置
    /*   config->enableReverseDirection                = false;
     *   config->decoderWorkMode                       = kENC_DecoderWorkAsNormalMode;
     *   config->HOMETriggerMode                       = kENC_HOMETriggerDisabled;
     *   config->INDEXTriggerMode                      = kENC_INDEXTriggerDisabled;
     *   config->enableTRIGGERClearPositionCounter     = false;
     *   config->enableTRIGGERClearHoldPositionCounter = false;
     *   config->enableWatchdog                        = false;
     *   config->watchdogTimeoutValue                  = 0U;
     *   config->filterCount                           = 0U;
     *   config->filterSamplePeriod                    = 0U;
     *   config->positionMatchMode                     = kENC_POSMATCHOnPositionCounterEqualToComapreValue;
     *   config->positionCompareValue                  = 0xFFFFFFFFU;
     *   config->revolutionCountCondition              = kENC_RevolutionCountOnINDEXPulse;
     *   config->enableModuloCountMode                 = false;
     *   config->positionModulusValue                  = 0U;
     *   config->positionInitialValue                  = 0U;
     * endcode*/
    ENC_Init(base, &mEncConfigStruct);
    ENC_DoSoftwareLoadInitialPositionValue(base); /* 用初始值更新位置计数器（定义一个新的初始位置）*/
}
/*
 * brief ENC初始化
 *
 * param base ENC peripheral base address.（such as "ENC1","ENC2","ENC3"，"ENC4"）
 */
//更新位置的初始值
void ENC_Dateclear(ENC_Type *base)
{
	ENC_DoSoftwareLoadInitialPositionValue(base);
}
//两次采样的位置差值
int16_t ENC_Dateget(ENC_Type *base)
{
	return (int16_t)ENC_GetHoldPositionDifferenceValue(base);
}
//得到位置
uint32_t ENC_Positionget(ENC_Type *base)
{
	return ENC_GetPositionValue(base);
}
//得到转的圈数
int16_t ENC_Revolutionget(ENC_Type *base)
{
	return ENC_GetHoldRevolutionValue(base);
}
