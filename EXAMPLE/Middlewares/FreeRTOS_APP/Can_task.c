#include "Can_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Can_App.h"


#define 		CAN_RX_TASK_STACK_SIZE    (2048)
#define 		CAN_RX_TASK_PRIORITY      4u
TaskHandle_t   	CAN_RX_task_Handler;             /* 任务句柄 */
void CAN_RX_task(void *pvParameters);             /* 任务函数 */

#define 		CAN_TX_TASK_STACK_SIZE    (1024)
#define 		CAN_TX_TASK_PRIORITY      5u
TaskHandle_t   	CAN_TX_task_Handler;             /* 任务句柄 */
void CAN_TX_task(void *pvParameters);                      /* 任务函数 */


//*********************************************************
//@auto: Li
//@brief: CAN task function
//@param: none
//@return: none
//*********************************************************

void CAN_TX_task(void *pvParameters)
{
    /* CAN初始化, 普通(0)/回环(1)模式, 波特率500Kbps */
    while(1)
    {
		CAN_TX_App();
		vTaskDelay(20);
    }
}
//*********************************************************
//@auto: Li
//@brief: CAN task function 
//@param: none
//@return: none
//*********************************************************

void CAN_RX_task(void *pvParameters)
{
	while(1)
	{
		CAN_RX_App();
		vTaskDelay(1000);
	}
}


//*********************************************************
//@auto: Li
//@brief: Create CAN task
//@param: none
//@return: none
//*********************************************************
void CAN_task_create(void)
{
		can_init(CAN_SJW_1TQ, CAN_BS2_6TQ, CAN_BS1_7TQ, 6,	CAN_MODE_NORMAL);
    xTaskCreate((TaskFunction_t )CAN_TX_task,(const char*)"CAN_TX_task",(uint16_t)CAN_TX_TASK_STACK_SIZE,(void*)NULL,(UBaseType_t)CAN_TX_TASK_PRIORITY,(TaskHandle_t*  )&CAN_TX_task_Handler);
    xTaskCreate((TaskFunction_t )CAN_RX_task,(const char*)"CAN_RX_task",(uint16_t)CAN_RX_TASK_STACK_SIZE,(void*)NULL,(UBaseType_t)CAN_RX_TASK_PRIORITY,(TaskHandle_t*  )&CAN_RX_task_Handler);
}
