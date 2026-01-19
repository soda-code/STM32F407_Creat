#include "AI_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "sram.h"
#include "adc.h"

#define AI_TASK_STACK_SIZE    (1024)
#define AI_TASK_PRIORITY      3u
TaskHandle_t   AI_task_Handler;             /* 任务句柄 */
void AI_task(void *pvParameters);                      /* 任务函数 */


uint32_t g_test_buffer[1024] __attribute__((at(AI_DATA_ADRESS_START)));
uint32_t g_test_buffer_1[1024] __attribute__((at(AI_DATA_ADRESS_END)));

//*********************************************************
//@auto 	: 	Li
//@brief	: 	AI task function
//@param	: 	none
//@return	: 	none
//*********************************************************
uint16_t adc_data=0;
void AI_task(void *pvParameters)
{
	uint8_t ADC_Data[4]={0};
	sram_clear(g_test_buffer,1024*3);
	while(1)
	{

		vTaskDelay(100);
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
