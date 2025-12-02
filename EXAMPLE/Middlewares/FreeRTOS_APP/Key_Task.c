#include "FreeRTOS.h"
#include "task.h"
#include "Key_Task.h"

#define KEY_TASK_STACK_SIZE    (512)
#define KEY_TASK_PRIORITY      (2)
TaskHandle_t   Key_task_Handler;             /* 任务句柄 */
void key_task(void *pvParameters);                      /* 任务函数 */


//*************************************************************
//@auto: Li
//@brief: Key task function
//@param: void *pvParameters: task parameters
//@return: none
//************************************************************* 
void key_task(void *pvParameters)
{
    while(1)
    {
        uint8_t key = key_scan(0); /* 调用按键扫描函数, 不支持连按 */
        if (key == KEY0_PRES)
        {
            my_printf("KEY0 Pressed!\r\n");
        }
        else if (key == KEY1_PRES)
        {
            my_printf("KEY1 Pressed!\r\n");
        }
        else if (key == WKUP_PRES)
        {
            my_printf("WKUP Pressed!\r\n");
        }
        vTaskDelay(10); // Example delay
    }
}

//**************************************************************
//@auto: Li
//@brief: Create key task
//@param: none
//@return: none
//************************************************************** 

void key_task_create(void)
{
    xTaskCreate((TaskFunction_t )key_task,(const char*)"key_task",(uint16_t)KEY_TASK_STACK_SIZE,(void*)NULL,(UBaseType_t)KEY_TASK_PRIORITY,(TaskHandle_t*  )&Key_task_Handler);

}