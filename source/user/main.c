/**
 * @file    IMRT10XX.c
 * @brief   Application entry point.
 */
#include "cm_backtrace/cm_backtrace.h"
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MIMXRT1052.h"
#include "fsl_debug_console.h"
#include "smartcar/sc_gpio.h"
#include "task.h"

//
//TaskHandle_t LED_task_handle;
//void LED_task(void* pvData) {
//	led_t led1 = { {XSNVS_PMIC_STBY_GPIO, XSNVS_PMIC_STBY_PIN,0} };
//	LED_Init(&led1);
//	while (1) {
//		LED_ON(&led1);
//		vTaskDelay(1 * configTICK_RATE_HZ);
//		LED_OFF(&led1);
//		vTaskDelay(1 * configTICK_RATE_HZ);
//	}
//}
//
//
//
//TaskHandle_t start_task_handle;
//void start_task(void* pvData) {
//	gpio_t wakeUp = { XSNVS_WAKEUP_GPIO, XSNVS_WAKEUP_PIN, 0 };
//	PRINTF("press wakeup key to start auto test\r\n");
//	while (GPIO_Read(&wakeUp)) {
//		vTaskDelay(10);
//	}
//	test(adc, "adc", 0, 1, 1);
//	test(camera, "camera ov7725", 0, 0, 0);
//	test(camera, "camera zzf", 0, 0, 0);
//	test(enc, "enc", 0, 1, 1);
//	test(flash, "enc", 1000, 0, 1);
//	test(flash_lfs, "enc", 1000, 0, 1);
//	test(keypad, "keypad", 0, 1, 1);
//	test(lcd, "lcd", 0, 1, 0);
//	test(oled, "oled", 0, 1, 0);
//	test(pit, "pit", 0, 1, 0);
//	test(pwm, "pwm", 0, 1, 0);
//	test(sd, "sd", 0, 1, 0);
//	test(status, "status", 0, 1, 1);
//	vTaskDelete(NULL);
//}

int main(void) {
	BOARD_ConfigVectors();
	BOARD_ConfigMPU();
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitBootPeripherals();
	BOARD_InitDebugConsole();
	PRINTF("\r\n*********\n");
	cm_backtrace_init("IMRT10XX.axf", "1.3", "2020.7.29");
	size_t i = 0;
	while (1) {
		i++;
		__NOP()
	}
}
