/*
 * sc_adc.c
 *
 *  Created on: 2020年1月28日
 *      Author: 17616
 */
#include "sc_adc.h"

const adc_config_t ADC1or2_config = {
        .enableOverWrite = false,
        .enableContinuousConversion = false,
        .enableHighSpeed = false,
        .enableLowPower = false,
        .enableLongSample = false,
        .enableAsynchronousClockOutput = true,
        .referenceVoltageSource = kADC_ReferenceVoltageSourceAlt0,
        .samplePeriodMode = kADC_SamplePeriodShort8Clocks,
        .clockSource = kADC_ClockSourceIPG,
        .clockDriver = kADC_ClockDriver1,
        .resolution = kADC_Resolution12Bit
};

/**
 * @brief  初始化adc1和adc2，12bit（满量程输为2^12-1）,硬件平均4次，采样一次大约需要1us
 * @param  {void} undefined :
 */
void ADC_Init2(void) {
    /* Initialize ADC1 peripheral. */
    ADC_Init(ADC1, &ADC1or2_config);
    ADC_Init(ADC2, &ADC1or2_config);
    /* Configure ADC1 peripheral to average 4 conversions in one measurement. */
    ADC_SetHardwareAverageConfig(ADC1, kADC_HardwareAverageCount4);
    ADC_SetHardwareAverageConfig(ADC2, kADC_HardwareAverageCount4);
    /* Perform ADC1 auto calibration. */
    ADC_DoAutoCalibration(ADC1);
    ADC_DoAutoCalibration(ADC2);
}

/**
 * @brief  读取adc某通道的值
 * @param  {ADC_Type*} base       :
 * @param  {uint32_t} adc_channel : 通道
 * @return {uint32_t}             :
 */
uint32_t ADC_Read(ADC_Type *base, uint32_t adc_channel) {
#define DEMO_ADC_CHANNEL_GROUP 0U
    adc_channel_config_t adcChannelConfigStruct;
    /* Configure the user channel and interrupt. */
    adcChannelConfigStruct.channelNumber = adc_channel;
    adcChannelConfigStruct.enableInterruptOnConversionCompleted = false;//取消采集完成中断

    ADC_SetChannelConfig(base, DEMO_ADC_CHANNEL_GROUP, &adcChannelConfigStruct);
    while (0U == ADC_GetChannelStatusFlags(base, DEMO_ADC_CHANNEL_GROUP)) {
    }
    return ADC_GetChannelConversionValue(base, DEMO_ADC_CHANNEL_GROUP);
}
