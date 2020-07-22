/*
 * sc_gpio.c
 *
 *  Created on: 2020年1月13日
 *      Author: 17616
 */
#include "sc_gpio.h"


void GPIO_Init(gpio_t *s) {
    gpio_pin_config_t pinConfig;
    s->direction = kGPIO_DigitalInput;
    pinConfig.direction = s->direction;
    pinConfig.outputLogic = 0;
    pinConfig.interruptMode = kGPIO_NoIntmode;
    GPIO_PinInit(s->base, s->pin, &pinConfig);
}


void GPIO_Write(gpio_t *s, uint8_t output) {
    if (output) {
        GPIO_Set(s);
    } else {
        GPIO_Clear(s);
    }
}


void GPIO_Set(gpio_t *s) {
    GPIO_PortSet(s->base, 1 << s->pin);
    GPIO_Out(s);
}


void GPIO_Clear(gpio_t *s) {
    GPIO_PortClear(s->base, 1 << s->pin);
    GPIO_Out(s);
}


void GPIO_Toggle(gpio_t *s) {
    GPIO_PortToggle(s->base, 1 << s->pin);
    GPIO_Out(s);
}


uint32_t GPIO_Read(gpio_t *s) {
    GPIO_In(s);
    return GPIO_PinRead(s->base, s->pin);
}

void GPIO_In(gpio_t *s) {
    if (s->direction == kGPIO_DigitalInput) {}
    else {
        s->base->GDIR &= ~(1UL << s->pin);;//修改成输入
        s->direction = kGPIO_DigitalInput;
    }
}

void GPIO_Out(gpio_t *s) {
    if (s->direction == kGPIO_DigitalOutput) {}
    else {
        s->base->GDIR |= (1UL << s->pin);//修改成输出
        s->direction = kGPIO_DigitalOutput;
    }
}