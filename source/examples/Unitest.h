//
// Created by 17616 on 2020/7/22.
//

#ifndef IMRT10XX_UNITEST_H
#define IMRT10XX_UNITEST_H

#include <projdefs.h>
#define TEST_PIT
typedef struct UnitestItem
{
	TaskFunction_t pxTaskCode;
	const char* const pcName;
	void* const pvParameters;
}UnitestItem_t;



extern UnitestItem_t default_item_list[];
void Unitest(UnitestItem_t item_list[]);

void U_keypad(void* pv);
void U_lcd(void* pv);
void U_oled(void* pv);
void U_ov7725(void* pv);
void U_zzf(void* pv);
void U_adc(void* pv);
void U_enc(void* pv);
void U_flash(void* pv);
void U_flash_with_lfs(void* pv);
//void U_gpio(void* pv);
#ifdef TEST_PIT
void U_pit(void* pv);
#endif //TEST_PIT
void U_pwm(void* pv);
//void U_uart(void* pv);
void U_msc(void* pv);
void U_sd(void* pv);
void U_file_dump(void* pv);
void U_status(void* pv);
void U_i2c_soft(void* pv);
void U_sccb_soft(void* pv);
void U_i2c_mt9v034(void* pv);
void U_cam_mt9v03x(void* pv);





#endif //IMRT10XX_UNITEST_H
