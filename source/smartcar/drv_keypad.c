#include "drv_keypad.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
/*
 * sc_keypad.c
 *
 *  Created on: 2020年1月13日
 *      Author: 17616
 */
keypad_t g_keypad;

/**
 * @brief  初始化矩阵键盘
 * @param  {keypad_t*} s        :
 * @param  {gpio_class_t*} Cols : 列线的gpio对象数组，最后一个必须为0以确定有几根列线
 * @param  {gpio_class_t*} Rows : 行线的gpio对象数组，最后一个必须为0以确定有几根行线
 */
void KEYPAD_Init(keypad_t *s, gpio_t *Rows, gpio_t *Cols) {
    //查找有多少行多少列
    gpio_t *buf = Cols;
    s->colNum = 0;
    s->rowNum = 0;
    while (buf->base != 0) {
        buf++;
        s->colNum++;
    }
    buf = Rows;
    while (buf->base != 0) {
        buf++;
        s->rowNum++;
    }


    s->cols = KEYPAD_MALLOC(s->colNum * sizeof(gpio_t));
    s->rows = KEYPAD_MALLOC(s->rowNum * sizeof(gpio_t));
    assert(s->cols);
    assert(s->rows);
    memcpy(s->cols, Cols, s->colNum * sizeof(gpio_t));
    memcpy(s->rows, Rows, s->rowNum * sizeof(gpio_t));

    //初始化引脚
    //行线配置成输出引脚
    for (size_t i = 0; i < s->rowNum; i++) {
        GPIO_Init(&s->rows[i]);
        GPIO_Write(&s->rows[i], 1);
    }

    //列线配置成输入引脚
    for (size_t i = 0; i < s->colNum; i++) {
        GPIO_Init(&s->cols[i]);
        GPIO_Read(&s->cols[i]);
    }
}

/**
 * @brief  返回按键输入，无滤波
 * @param  {keypad_t*} s :
 * @param  {int} row     : 按键、拨码位置
 * @param  {int} col     : 按键、拨码位置
 * @return {uint32_t}    : 1为按下，0为没有按下。
 */
uint32_t KEYPAD_Scan(keypad_t *s, int row, int col) {
    uint32_t val;
    GPIO_Write(&s->rows[row], 0);
    if (GPIO_Read(&s->cols[col])) {
        val = 0;
    } else {
        val = 1;
    }
    GPIO_Write(&s->rows[row], 1);
    return val;
}

/**
 * @brief  返回按键输入，有滤波
 * @param  {keypad_t*} s :
 * @param  {int} row     : 按键、拨码位置
 * @param  {int} col     : 按键、拨码位置
 * @return {uint32_t}    : 1为按下，0为没有按下。
 */
uint32_t KEYPAD_Get(keypad_t *s, int row, int col) {
    int times = 50;
    while (times--) {
        if (KEYPAD_Scan(s, row, col) == 0) {
            return 0;
        }
        vTaskDelay(1);
    }
    return 1;

}

void KEYPAD_Deinit(keypad_t *s) {
    KEYPAD_FREE(s->cols);
    KEYPAD_FREE(s->rows);
    memset(s,0,sizeof(keypad_t));
}
