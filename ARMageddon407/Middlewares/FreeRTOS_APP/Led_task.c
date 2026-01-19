#include "Led_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Led_App.h"

#define LED_TASK_STACK_SIZE    (512)
#define LED_TASK_PRIORITY      1u
TaskHandle_t   Led_task_Handler;             /* 任务句柄 */
void led_task(void *pvParameters);                      /* 任务函数 */

//*********************************************************
//@auto: Li
//@brief: LED task function
//@param: none
//@return: none
//*********************************************************

void led_task(void *pvParameters)
{
    while(1)
    {
        Led_On();
        vTaskDelay(500);
				Led_Off();
        vTaskDelay(500);
    }
}

//*********************************************************
//@auto: Li
//@brief: Create LED task
//@param: none
//@return: none
//*********************************************************
void led_task_create(void)
{
    xTaskCreate((TaskFunction_t )led_task,(const char*)"led_task",(uint16_t)LED_TASK_STACK_SIZE,(void*)NULL,(UBaseType_t)LED_TASK_PRIORITY,(TaskHandle_t*  )&Led_task_Handler);

}