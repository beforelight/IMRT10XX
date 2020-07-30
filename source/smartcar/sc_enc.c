#include "sc_enc.h"

void ENC_PositionClear(ENC_Type* base)
{
    ENC_DoSoftwareLoadInitialPositionValue(base);
}

int16_t ENC_Dateget(ENC_Type* base)
{
    return (int16_t)ENC_GetHoldPositionDifferenceValue(base);
}

int32_t ENC_PositionGet(ENC_Type* base)
{
    return (int32_t)ENC_GetPositionValue(base);
}

int16_t ENC_Revolutionget(ENC_Type* base)
{
    return (int16_t)ENC_GetHoldRevolutionValue(base);
}

void ENC_InitDecoder(ENC_Type* base)
{
    enc_config_t mEncConfigStruct;
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
    mEncConfigStruct.revolutionCountCondition
        = kENC_RevolutionCountOnRollOverModulus;/*使用位置计数器溢出作为转数信号（而不是使用index信号）*/
    ENC_Init(base, &mEncConfigStruct);
    ENC_DoSoftwareLoadInitialPositionValue(base); /* 用初始值更新位置计数器（定义一个新的初始位置）*/
}

void ENC_InitPhaseCounter(ENC_Type* base)
{
    enc_config_t mEncConfigStruct;
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
    mEncConfigStruct.revolutionCountCondition
        = kENC_RevolutionCountOnRollOverModulus;/*使用位置计数器溢出作为转数信号（而不是使用index信号）*/
    mEncConfigStruct.decoderWorkMode = kENC_DecoderWorkAsSignalPhaseCountMode;//只有这一句和上面不太一样
    ENC_Init(base, &mEncConfigStruct);
    ENC_DoSoftwareLoadInitialPositionValue(base); /* 用初始值更新位置计数器（定义一个新的初始位置）*/
}
