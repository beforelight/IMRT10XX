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
#include "sc_sd.h"
BSS_DTC uint8_t heap_heap1[64 * 1024] ALIGN(8);
BSS_OC uint8_t heap_heap2[128 * 1024] ALIGN(8);
BSS_SDRAM uint8_t heap_heap3[4 * 1024 * 1024] ALIGN(8);
const HeapRegion_t xHeapRegions[] =
        {
                {&heap_heap1[0], sizeof(heap_heap1)},
                {&heap_heap2[0], sizeof(heap_heap2)},
                {&heap_heap3[0], sizeof(heap_heap3)},
                {NULL,           0} /* Terminates the array. */
        };

gpio_t wakeUp = {WAKEUP_GPIO, WAKEUP_PIN, kGPIO_DigitalInput};
gpio_t led = {LED1_GPIO, LED1_PIN, kGPIO_DigitalOutput};
TaskHandle_t LED_task_handle;
TaskHandle_t start_task_handle;

void LED_task(void *pvData) {
    while (1) {
        GPIO_Set(&led);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        GPIO_Clear(&led);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
//extern void adc(void *pv);
//extern void cpp(void *pv);
//extern void flash_lfs(void *pv);
//extern void flash(void *pv);
//extern void pit(void *pv);

TaskHandle_t handle;

void start_task(void *pvData) {
    PRINTF("press wakeup key to start\r\n");
    while (GPIO_Read(&wakeUp)) {
        vTaskDelay(10);
    }

    SD_MscInit();

//    xTaskCreate(adc,"adc example",512,NULL,1,&handle);
//    xTaskCreate(cpp,"cpp example",512,NULL,1,&handle);
//    xTaskCreate(pit,"pit example",512,NULL,1,&handle);
    vTaskDelete(NULL);
}

int main(void) {
    BOARD_ConfigVectors();
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();
    PRINTF("\r\n*********\n");
    cm_backtrace_init("imxrt.axf", "0.1.0", "0.0.1");
    vPortDefineHeapRegions(xHeapRegions);
    xTaskCreate(LED_task, "LED_task", 64, NULL, 3, &LED_task_handle);
    //这个任务除了测试flash之外还会记录一共启动了多少次
//    xTaskCreate(flash_lfs,"flash_lfs example",512,NULL,3,&handle);
    xTaskCreate(start_task, "start_task", 1028, NULL, 2, &start_task_handle);
    vTaskStartScheduler();
    return 0;
}
