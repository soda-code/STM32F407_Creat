#include "AI_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"

#define AI_TASK_STACK_SIZE    (2048)
#define AI_TASK_PRIORITY      3u
TaskHandle_t   AI_task_Handler;             /* 任务句柄 */
void AI_task(void *pvParameters);                      /* 任务函数 */



//*********************************************************
//@auto: Li
//@brief: AI task function
//@param: none
//@return: none
//*********************************************************

void AI_task(void *pvParameters)
{

    while(1)
    {
		my_printf("\n--- 训练结束，开始测试 ---\r\n");

		vTaskDelay(3000);
    }
}

//*********************************************************
//@auto: Li
//@brief: Create AI task
//@param: none
//@return: none
//*********************************************************
void AI_task_create(void)
{
    xTaskCreate((TaskFunction_t )AI_task,(const char*)"AI_task",(uint16_t)AI_TASK_STACK_SIZE,(void*)NULL,(UBaseType_t)AI_TASK_PRIORITY,(TaskHandle_t*  )&AI_task_Handler);

}
