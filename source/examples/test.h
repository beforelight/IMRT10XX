/*
 * test.h
 *
 *  Created on: 2020年4月15日
 *      Author: 17616
 */
 //#define EXAMPLES_TEST_H_

#ifndef EXAMPLES_TEST_H_
#define EXAMPLES_TEST_H_
#include"FreeRTOS.h"
#include"task.h"
#include"fsl_debug_console.h"
#include"smartcar/status.h"
void test(TaskFunction_t pxTaskCode, const char* const pcName, uint32_t TimeOutMs,int runDirect) {
	if (TimeOutMs == 0) { TimeOutMs = ~0; }
	TaskStatus_t xTaskDetails;
	vTaskGetInfo(NULL, &xTaskDetails, pdTRUE, eInvalid);
//    xTaskDetails.uxCurrentPriority;//调用Test函数任务的优先级
    if (runDirect == 0)
    {
        PRINTF(">>run task %s? y/n \r\n", pcName);
        char ch = GETCHAR();
        if(ch=='y'||ch=='Y'){}
        else
        {
            return;
        }
    }
    PRINTF("run task %s>>\r\n",pcName);
    uint32_t msp = TimerUsGet();
    TaskHandle_t task_handle;
    xTaskCreate(pxTaskCode, pcName, 2048, NULL, xTaskDetails.uxCurrentPriority, &task_handle);
    vTaskDelay(100);
    eTaskState e; 
    while (1)
    {
        e = eTaskGetState(task_handle);
        if (e == eDeleted || e == eInvalid)
        {
            PRINTF("task %s exit!\r\n", pcName);
            return;
        }
        else if(TimerUsGet()- msp>= TimeOutMs)
        {
            vTaskDelete(task_handle);
            PRINTF("task %s time out!\r\n", pcName);
            return;
        }
        vTaskDelay(100);
    }
}







#endif /* EXAMPLES_TEST_H_ */
