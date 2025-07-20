#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/SRAM/sram.h"
#include "./MALLOC/malloc.h"
#include "./BSP/DHT11/dht11.h"
#include "./BSP/RTC/rtc.h"
#include "dht11_app.h"

#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "LVGL/GUI_APP/lv_mainstart.h"
#include "lvgl.h"
#include "FreeRTOS.h"
#include "task.h"
#include "led_app.h"
#include "KEY_app.h"

#include "lvgl_app.h"
#include "RTC_Read_app.h"

#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_interface.h"
USBD_HandleTypeDef USBD_Device;             /* USB Device处理结构体 */
extern volatile uint8_t g_device_state;     /* USB连接 情况 */

/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO     1           /* 任务优先级 */
#define START_STK_SIZE      128         /* 任务堆栈大小 */
TaskHandle_t StartTask_Handler;         /* 任务句柄 */
void start_task(void *pvParameters);    /* 任务函数 */



/**
 * @brief       lvgl_demo入口函数
 * @param       无
 * @retval      无
 */
void FreeRTOS_Start(void)
{
    lv_init();                                          /* lvgl系统初始化 */
    lv_port_disp_init();                                /* lvgl显示接口初始化,放在lv_init()的后面 */
    lv_port_indev_init();                               /* lvgl输入接口初始化,放在lv_init()的后面 */
    xTaskCreate((TaskFunction_t )start_task,(const char* )"start_task",(uint16_t)START_STK_SIZE,(void* )NULL,(UBaseType_t)START_TASK_PRIO,(TaskHandle_t* )&StartTask_Handler);   /* 任务句柄 */
    vTaskStartScheduler();                              /* 开启任务调度 */
}


/**
 * @brief       start_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void start_task(void *pvParameters)
{
    pvParameters = pvParameters;
    taskENTER_CRITICAL();           /* 进入临界区 */
	LED_APP_Init();
	KEY_APP_Init();
	DHT11_APP_Init();
	LVGL_APP_Init();
	RTC_APP_Init();
    taskEXIT_CRITICAL();            /* 退出临界区 */
    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
}

__attribute__((section(".ext_sram")))
uint8_t buffer[200];  // 100KB 放到外部 SRAM


int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7); /* 设置时钟,168Mhz */
    delay_init(168);                    /* 延时初始化 */
    usart_init(115200);                 /* 串口初始化为115200 */

    led_init();                         /* 初始化LED */
    key_init();                         /* 初始化按键 */
	rtc_init();
    sram_init();                        /* SRAM初始化 */

    my_mem_init(SRAMIN);                /* 初始化内部SRAM内存池 */
   // my_mem_init(SRAMEX);                /* 初始化外部SRAM内存池 */
    USBD_Init(&USBD_Device, &VCP_Desc, 0);                      /* 初始化USB */
    USBD_RegisterClass(&USBD_Device, USBD_CDC_CLASS);           /* 添加类 */
    USBD_CDC_RegisterInterface(&USBD_Device, &USBD_CDC_fops);   /* 为MSC类添加回调函数 */
    USBD_Start(&USBD_Device);                                   /* 开启USB */
    FreeRTOS_Start();                        /* 运行FreeRTOS例程 */

	while(1)
	{

	}
}


