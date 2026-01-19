#include "FreeRTOS.h"
#include "task.h"
#include "Light_Task.h"

#define Light_TASK_STACK_SIZE    (512)
#define Light_TASK_PRIORITY      4u
TaskHandle_t   Light_task_Handler;             /* 任务句柄 */
void Light_task(void *pvParameters);                      /* 任务函数 */

//********************************************************* 
//@auto: Li
//@brief: Error_Log task function
//@param: none
//@return: none
//*********************************************************

void Light_task(void *pvParameters)
{
	while(1)
	{
		Light_Read();
		vTaskDelay(1);
	}
}

//*********************************************************
//@auto: Li
//@brief: Create Error_Log task
//@param: none
//@return: none
//*********************************************************
void Light_task_create(void)
{
    xTaskCreate((TaskFunction_t )Light_task,(const char*)"Light_task",(uint16_t)Light_TASK_STACK_SIZE,(void*)NULL,(UBaseType_t)Light_TASK_PRIORITY,(TaskHandle_t*  )&Light_task_Handler);

}
