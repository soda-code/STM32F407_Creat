#include "FreeRTOS.h"
#include "task.h"
#include "lvgl_app.h"
#include "./CMSIS/DSP/Include/arm_math.h"


#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "LVGL/GUI_APP/lv_mainstart.h"
#include "LVGL/GUI_APP/lv_curve_Screen.h"


/* LED_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LVGL_TASK_PRIO       3           /* 任务优先级 */
#define LVGL_STK_SIZE        1024         /* 任务堆栈大小 */
TaskHandle_t LVGLTask_Handler;           /* 任务句柄 */
void lvgl_task(void *pvParameters);      /* 任务函数 */



void lvgl_task(void *pvParameters)
{

	HOME_DISPLAY();         //*LVGL入口


}


void LVGL_APP_Init(void)
{
	    /* LED测试任务 */
    xTaskCreate((TaskFunction_t )lvgl_task,(const char* )"lvgl_task",(uint16_t )LVGL_STK_SIZE,(void* )NULL,(UBaseType_t )LVGL_TASK_PRIO,(TaskHandle_t*  )&LVGLTask_Handler);
}
