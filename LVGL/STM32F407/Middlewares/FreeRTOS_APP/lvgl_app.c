#include "FreeRTOS.h"
#include "task.h"
#include "lvgl_app.h"
#include "./CMSIS/DSP/Include/arm_math.h"


#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "lv_port_indev_template.h"
#include "LVGL/GUI_APP/lv_mainstart.h"
#include "LVGL/GUI_APP/lv_curve_Screen.h"


/* LED_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LVGL_TASK_PRIO       3           /* �������ȼ� */
#define LVGL_STK_SIZE        1024         /* �����ջ��С */
TaskHandle_t LVGLTask_Handler;           /* ������ */
void lvgl_task(void *pvParameters);      /* ������ */



void lvgl_task(void *pvParameters)
{

	HOME_DISPLAY();         //*LVGL���


}


void LVGL_APP_Init(void)
{
	    /* LED�������� */
    xTaskCreate((TaskFunction_t )lvgl_task,(const char* )"lvgl_task",(uint16_t )LVGL_STK_SIZE,(void* )NULL,(UBaseType_t )LVGL_TASK_PRIO,(TaskHandle_t*  )&LVGLTask_Handler);
}
