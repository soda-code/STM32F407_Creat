#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/bsp_app.h"

void led_init(void);                    /* LED初始化函数声明 */

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7); /* 设置时钟,168Mhz */
    delay_init(168);                    /* 延时初始化 */
    led_init();                         /* 初始化LED */
    
    while(1)
    {
		LED0(1);
		LED1(1);
        delay_ms(500);
  		LED0(0);
		LED1(0);
        delay_ms(500);
    }
}

