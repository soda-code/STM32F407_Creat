#include "led_app.h"
#include "./BSP/LED/led.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"




void led_run(void)
{

    while (1)
    {
        LED0_TOGGLE();
        LED1_TOGGLE();
        vTaskDelay(500); /* 延时500ms */
    }
}
