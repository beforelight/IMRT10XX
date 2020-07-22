//
// Created by 17616 on 2020/7/22.
//

#ifndef IMRT10XX_UNITEST_H
#define IMRT10XX_UNITEST_H

#include <projdefs.h>

typedef struct UnitestItem
{
	TaskFunction_t pxTaskCode;
	const char* const pcName;
	void* const pvParameters;
}UnitestItem_t;

void Unitest(void);

void U_keypad(void* pv);

void U_lcd(void* pv);
void U_oled(void* pv);
void U_ov7725(void* pv);
void U_zzf(void* pv);
void U_adc(void* pv);
void U_enc(void* pv);
void U_flash(void* pv);
void U_flash_with_lfs(void* pv);
void U_gpio(void* pv);
void U_pit(void* pv);
void U_pwm(void* pv);
void U_uart(void* pv);
void U_sd(void* pv);
void U_status(void* pv);




#endif //IMRT10XX_UNITEST_H
