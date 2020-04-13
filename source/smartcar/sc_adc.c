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
