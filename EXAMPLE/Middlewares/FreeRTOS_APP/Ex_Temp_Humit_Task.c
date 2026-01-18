#include "Ex_Temp_Humit_Task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "Ex_Temp_Humit.h"

#define 		EX_TEMP_HUMIT_TASK_STACK_SIZE    (512)
#define 		EX_TEMP_HUMIT_TASK_PRIORITY       3u
TaskHandle_t   	EX_TEMP_HUMIT_task_Handler;      /* 任务句柄 */
void EX_TEMP_HUMIT(void *pvParameters);     /* 任务函数 */


//*********************************************************
//@auto:   Li
//@brief:  EX_TEMP_HUMIT task function
//@param:  none
//@return: none
//*********************************************************

void EX_TEMP_HUMIT(void *pvParameters)
{
    while(1)
    {
		Read_Temp_Humit();
    }
}

//*********************************************************
//@auto:   Li
//@brief:  Create EX_TEMP_HUMIT task
//@param:  none
//@return: none
//*********************************************************
void EX_Temp_Humit_task_create(void)
{
    xTaskCreate((TaskFunction_t )EX_TEMP_HUMIT,(const char*)"EX_TEMP_HUMIT",(uint16_t)EX_TEMP_HUMIT_TASK_STACK_SIZE,(void*)NULL,(UBaseType_t)EX_TEMP_HUMIT_TASK_PRIORITY,(TaskHandle_t*  )&EX_TEMP_HUMIT_task_Handler);

}
