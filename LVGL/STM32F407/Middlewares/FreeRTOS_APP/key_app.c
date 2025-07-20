#include "./BSP/KEY/key.h"
#include "FreeRTOS.h"
#include "task.h"
#include "KEY_app.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_interface.h"


/* KEY_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define KEY_TASK_PRIO       4           /* 任务优先级 */
#define KEY_STK_SIZE        128         /* 任务堆栈大小 */
TaskHandle_t KEYTask_Handler;           /* 任务句柄 */
void KEY_task(void *pvParameters);      /* 任务函数 */

KEY_Inf Key_Fun=KEY_CMD_ON;

void KEY_task(void *pvParameters)
{
    while(1)
    {
        switch (key_scan(0)) 
		{
            case KEY0_PRES:
                Key_Fun=KEY_CMD_ON;
                break;
            case KEY1_PRES:
                Key_Fun=KEY_CMD_OFF;
                break;
            case WKUP_PRES:
                Key_Fun=KEY_CMD_TOGGLE;
                break;
			default:
				break;
        }
        vTaskDelay(200);
    }
}


void KEY_APP_Init(void)
{
	    /* KEY测试任务 */
    xTaskCreate((TaskFunction_t )KEY_task,(const char* )"KEY_task",(uint16_t )KEY_STK_SIZE,(void* )NULL,(UBaseType_t )KEY_TASK_PRIO,(TaskHandle_t*  )&KEYTask_Handler);
}

