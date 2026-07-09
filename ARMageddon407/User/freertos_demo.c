#include "freertos_demo.h"
#include "./BSP/LED/led.h"
#include "max30102_app.h"

/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "led_app.h"

/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO 1                   /* 任务优先级 */
#define START_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            StartTask_Handler;  /* 任务句柄 */
void start_task(void *pvParameters);        /* 任务函数 */

/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LED_PRIO      5                   /* 任务优先级 */
#define LED_STK_SIZE  4*1024                 /* 任务堆栈大小 */
TaskHandle_t          LedTask_Handler;  /* 任务句柄 */
void led_task(void *pvParameters);             /* 任务函数 */

/* TASK2 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define DATA_CAP_PRIO      6                   /* 任务优先级 */
#define DATA_CAP_STK_SIZE  512                 /* 任务堆栈大小 */
TaskHandle_t           DataCapTask_Handler;   /* 任务句柄 */
void DataCap_task(void *pvParameters);        /* 任务函数 */

/******************************************************************************************************/

/* LCD刷屏时使用的颜色 */

/**
 * @brief       FreeRTOS例程入口函数
 * @param       无
 * @retval      无
 */
void freertos_demo(void)
{
    
    xTaskCreate((TaskFunction_t )start_task,            /* 任务函数 */
                (const char*    )"start_task",          /* 任务名称 */
                (uint16_t       )START_STK_SIZE,        /* 任务堆栈大小 */
                (void*          )NULL,                  /* 传入给任务函数的参数 */
                (UBaseType_t    )START_TASK_PRIO,       /* 任务优先级 */
                (TaskHandle_t*  )&StartTask_Handler);   /* 任务句柄 */
    vTaskStartScheduler();
}

/**
 * @brief       start_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void start_task(void *pvParameters)
{
    __HAL_RCC_CRC_CLK_ENABLE();
    taskENTER_CRITICAL();           /* 进入临界区 */
    /* 创建任务1 */
    xTaskCreate((TaskFunction_t )led_task,
                (const char*    )"led_task",
                (uint16_t       )LED_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_PRIO,
                (TaskHandle_t*  )&LedTask_Handler);
    /* 创建任务2 */
    xTaskCreate((TaskFunction_t )DataCap_task,
                (const char*    )"DataCap_task",
                (uint16_t       )DATA_CAP_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )DATA_CAP_PRIO,
                (TaskHandle_t*  )&DataCapTask_Handler);
    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
    taskEXIT_CRITICAL();            /* 退出临界区 */
}

/**
 * @brief       task1
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void led_task(void *pvParameters)
{
    
    while(1)
    {
    led_run();
    }
}

/**
 * @brief       task2
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void DataCap_task(void *pvParameters)
{
    
    while(1)
    {
    	Max_30102_run();
    }
}
