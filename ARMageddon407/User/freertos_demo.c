#include "freertos_demo.h"
#include "./BSP/LED/led.h"
#include "GUI.h"
#include "GUIDEMO.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"

#include "mem_pool.h"

// 嵌入式中常用静态大数组作为内存池
#define EXTERNAL_SRAM_ADDR   ((void*)0x68000000)   // 示例：STM32 FSMC/FMC 映射地址
#define EXTERNAL_SRAM_SIZE   (1024 * 1024 * 1)     // 示例：1MB

/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO 1                   /* 任务优先级 */
#define START_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            StartTask_Handler;  /* 任务句柄 */
void start_task(void *pvParameters);        /* 任务函数 */

/* LED 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LED_PRIO      5                   /* 任务优先级 */
#define LED_STK_SIZE  2*1024                 /* 任务堆栈大小 */
TaskHandle_t          LedTask_Handler;  /* 任务句柄 */
void led_task(void *pvParameters);             /* 任务函数 */

/* TASK2 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK2_PRIO      3                   /* 任务优先级 */
#define TASK2_STK_SIZE  512                 /* 任务堆栈大小 */
TaskHandle_t            Task2Task_Handler;  /* 任务句柄 */
void task2(void *pvParameters);             /* 任务函数 */

/* LED0 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LED0_PRIO      4                   /* 任务优先级 */
#define LED0_STK_SIZE  512                 /* 任务堆栈大小 */
TaskHandle_t           Led0Task_Handler;   /* 任务句柄 */
void led0_task(void *pvParameters);        /* 任务函数 */

/******************************************************************************************************/

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
		mem_pool_init(EXTERNAL_SRAM_ADDR, EXTERNAL_SRAM_SIZE, true);
		void* p1 = mem_pool_alloc(10*1024);
		void* p2 = mem_pool_alloc(300*1024);
		void* p3 = mem_pool_alloc(200*1024);
	    uint8_t *reg = (uint8_t*)mem_pool_alloc(512);
	    uint32_t *reg1 = (uint32_t*)mem_pool_alloc(512);
    taskENTER_CRITICAL();           /* 进入临界区 */
    /* 创建任务1 */
    xTaskCreate((TaskFunction_t )led_task,
                (const char*    )"led_task",
                (uint16_t       )LED_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_PRIO,
                (TaskHandle_t*  )&LedTask_Handler);
    /* 创建任务2 */
    xTaskCreate((TaskFunction_t )task2,	
                (const char*    )"task2",
                (uint16_t       )TASK2_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK2_PRIO,
                (TaskHandle_t*  )&Task2Task_Handler);
                
                
    /* 创建LED任务 */
    xTaskCreate((TaskFunction_t )led0_task,
                (const char*    )"led0_task",
                (uint16_t       )LED0_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED0_PRIO,
                (TaskHandle_t*  )&Led0Task_Handler);
                
    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
    taskEXIT_CRITICAL();            /* 退出临界区 */
}

/**
 * @brief       led_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void led_task(void *pvParameters)
{
  while(1)
  {
    led_run();
    GUI_Delay(1);
  }
}

/**
 * @brief       task2
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void task2(void *pvParameters)
{
    GUIDEMO_Main();
    
    while (1)
    {
        vTaskDelay(1);
    }
}

/**
 * @brief       led0_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */

void led0_task(void *pvParameters)
{
    while (1)
    {
        vTaskDelay(1);
    }
}
