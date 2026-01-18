#include "Data_Save_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Data_Save.h"

#define DATA_TASK_STACK_SIZE    (2048)
#define DATA_TASK_PRIORITY      2u
TaskHandle_t   Data_task_Handler;             /* 任务句柄 */
void Data_task(void *pvParameters);                      /* 任务函数 */

//********************************************************* 
//@auto: Li
//@brief: Error_Log task function
//@param: none
//@return: none
//*********************************************************

void Data_task(void *pvParameters)
{
  Data_Save_Init();
  while(1)
  {
		Data_save();
		vTaskDelay(1000);
  }
}

//*********************************************************
//@auto: Li
//@brief: Create Error_Log task
//@param: none
//@return: none
//*********************************************************
void Data_Save_task_create(void)
{
    xTaskCreate((TaskFunction_t )Data_task,(const char*)"Data_task",(uint16_t)DATA_TASK_STACK_SIZE,(void*)NULL,(UBaseType_t)DATA_TASK_PRIORITY,(TaskHandle_t*  )&Data_task_Handler);

}
