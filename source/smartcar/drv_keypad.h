/*
 * sc_keypad.h
 *
 *  Created on: 2020年1月13日
 *      Author: 17616
 */

#ifndef SC_KEYPAD_H_
#define SC_KEYPAD_H_

#include "sc_gpio.h"

#define KEYPAD_MALLOC pvPortMalloc
#define KEYPAD_FREE vPortFree

typedef struct _keypad {
    gpio_t *rows;//行
    gpio_t *cols;//列
    int colNum;
    int rowNum;
} keypad_t;
extern keypad_t g_keypad;//默认的矩阵键盘对象

typedef struct _keypad_key {
    uint8_t row;
    uint8_t col;
} keypad_key_t;//定义一个按键在行列的位置

/**
 * @brief  初始化矩阵键盘，注意:行线配置成输出引脚,列线配置成输入引脚
 * @param  {keypad_t*} s  : 
 * @param  {gpio_t*} Rows : 行线的gpio对象数组，最后一个必须为0以确定有几根行线
 * @param  {gpio_t*} Cols : 列线的gpio对象数组，最后一个必须为0以确定有几根列线
 */
void KEYPAD_Init(keypad_t *s, gpio_t *Rows, gpio_t *Cols);

/**
 * @brief  取消初始化，并释放分配的内存
 * @param  {keypad_t*} s : 
 */
void KEYPAD_Deinit(keypad_t *s);

/**
 * @brief  返回按键输入，无滤波
 * @param  {keypad_t*} s : 
 * @param  {int} row     : 按键、拨码位置
 * @param  {int} col     : 按键、拨码位置
 * @return {uint32_t}    : 1为按下，0为没有按下。
 */
uint32_t KEYPAD_Scan(keypad_t *s, int row, int col);

/**
 * @brief  返回按键输入，有滤波
 * @param  {keypad_t*} s :
 * @param  {int} row     : 按键、拨码位置
 * @param  {int} col     : 按键、拨码位置
 * @return {uint32_t}    : 1为按下，0为没有按下。 
 */
uint32_t KEYPAD_Get(keypad_t *s, int row, int col);

/**
 * @brief  返回按键输入，无滤波
 * @param  {keypad_t*} s      :
 * @param  {keypad_key_t} key : 按键、拨码位置
 * @return {uint32_t}         : 1为按下，0为没有按下。
 */
static inline uint32_t KEYPAD_Scan2(keypad_t *s, keypad_key_t key) { return KEYPAD_Scan(s, key.row, key.col); }

/**
 * @brief  返回按键输入，有滤波
 * @param  {keypad_t*} s      :
 * @param  {keypad_key_t} key : 按键、拨码位置
 * @return {uint32_t}         : 1为按下，0为没有按下。
 */
static inline uint32_t KEYPAD_Get2(keypad_t *s, keypad_key_t key) { return KEYPAD_Get(s, key.row, key.col); }


#endif /* SC_KEYPAD_H_ */
