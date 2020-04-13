/*
 * sc_adc.h
 *
 *  Created on: 2020年1月28日
 *      Author: 17616
 */

#ifndef SC_ADC_H_
#define SC_ADC_H_

#include "fsl_debug_console.h"
#include "fsl_adc.h"

/**
 * @brief  初始化adc1和adc2，12bit（满量程输为2^12-1）,硬件平均4次，采样一次大约需要1us
 * @param  {void} undefined : 
 */
void ADC_Init2(void);

/**
 * @brief  读取adc某通道的值
 * @param  {ADC_Type*} base       : 
 * @param  {uint32_t} adc_channel : 通道
 * @return {uint32_t}             : 
 */
uint32_t ADC_Read(ADC_Type *base, uint32_t adc_channel);

#endif /* SC_ADC_H_ */
