#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "freertos_START.h"

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7); /* 设置时钟,168Mhz */
    delay_init(168);                    /* 延时初始化 */
    usart_init(115200);                 /* 串口初始化为115200 */
    led_init();                         /* 初始化LED */
    freertos_demo();                    /* 运行FreeRTOS */

}
