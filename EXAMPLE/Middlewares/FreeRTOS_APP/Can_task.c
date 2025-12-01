#include "Can_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "can.h"

#define 		CAN_RX_TASK_STACK_SIZE    (2048)
#define 		CAN_RX_TASK_PRIORITY      4u
TaskHandle_t   	CAN_RX_task_Handler;             /* 任务句柄 */
void CAN_RX_task(void *pvParameters);             /* 任务函数 */

#define 		CAN_TX_TASK_STACK_SIZE    (1024)
#define 		CAN_TX_TASK_PRIORITY      4u
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
	uint8_t canbuf[8]={0};
    /* CAN初始化, 普通(0)/回环(1)模式, 波特率500Kbps */

    while(1)
    {
		can_send_msg(0x125, canbuf, 8);    /* ID = 0x12, 发送8个字节 */
		canbuf[0]++;
		vTaskDelay(1000);
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
	uint8_t can_rx_buff[8];
    /* CAN初始化, 普通(0)/回环(1)模式, 波特率500Kbps */

    while(1)
    {
		can_receive_msg(0x125,can_rx_buff);
		canbuf[0]++;
		my_printf("\r\n can_rx");
		for(uint8_t i=0;i<8;i++)
		{
			my_printf("%3x",can_rx_buff[i]);
		}
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
