/*
 * sc_gpio.h
 *
 *  Created on: 2020年1月13日
 *      Author: 17616
 */

#ifndef SC_GPIO_H_
#define SC_GPIO_H_

#include"fsl_gpio.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

typedef struct _gpio_class {
    GPIO_Type *base;
    uint32_t pin;
    gpio_pin_direction_t direction;//记录引脚的输入输出状态
} gpio_t;//gpio_t的对象只能被引用，不能被复制

/**
 * @brief  初始化io
 * @param  {gpio_class_t*} s : 
 */
void GPIO_Init(gpio_t *s);

/**
 * @brief  修改为输出
 * @param  {gpio_t*} s : 
 */
void GPIO_Out(gpio_t *s);

/**
 * @brief  修改为输入
 * @param  {gpio_t*} s : 
 */
void GPIO_In(gpio_t *s);

/**
 * @brief  从引脚输出电平，输入引脚会被重新初始化为输出脚
 * @param  {gpio_class_t*} s : 
 * @param  {uint8_t} output  : 1为高电平，0为低电平
 */
void GPIO_Write(gpio_t *s, uint8_t output);

/**
 * @brief  置1，输入引脚会被重新初始化为输出脚
 * @param  {gpio_class_t*} s : 
 */
void GPIO_Set(gpio_t *s);

/**
 * @brief  清0，输入引脚会被重新初始化为输出脚
 * @param  {gpio_class_t*} s : 
 */
void GPIO_Clear(gpio_t *s);

/**
 * @brief  翻转，输入引脚会被重新初始化为输出脚
 * @param  {gpio_class_t*} s : 
 */
void GPIO_Toggle(gpio_t *s);

/**
 * @brief  返回引脚电平，输出脚会重新初始化为输入脚
 * @param  {gpio_class_t*} s : 
 * @return {uint32_t}        : 1为高电平，0为低电平
 */
uint32_t GPIO_Read(gpio_t *s);
#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif /* SC_GPIO_H_ */
