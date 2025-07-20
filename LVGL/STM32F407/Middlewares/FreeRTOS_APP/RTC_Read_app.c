#include "./BSP/LED/led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "RTC_Read_app.h"
#include "./BSP/RTC/rtc.h"

/* LED_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define RTC_TASK_PRIO       4           /* 任务优先级 */
#define RTC_STK_SIZE        128         /* 任务堆栈大小 */
TaskHandle_t RTCTask_Handler;           /* 任务句柄 */
void RTC_task(void *pvParameters);      /* 任务函数 */

RTC_STRUCT RTC_Info={0};

void RTC_task(void *pvParameters)
{
    pvParameters = pvParameters;

    while(1)
    {
		rtc_get_time(&RTC_Info);
		rtc_get_date(&RTC_Info);
        vTaskDelay(500);
    }
}


void RTC_APP_Init(void)
{
    xTaskCreate((TaskFunction_t )RTC_task,(const char* )"RTC_task",(uint16_t )RTC_STK_SIZE,(void* )NULL,(UBaseType_t )RTC_TASK_PRIO,(TaskHandle_t*  )&RTCTask_Handler);
}
