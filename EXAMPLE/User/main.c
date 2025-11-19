#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "can.h"
#include "lcd.h"
#include "malloc.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_interface.h"
#include "freertos_START.h"

int main(void)
{
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7); /* 设置时钟,168Mhz */
    delay_init(168);                    /* 延时初始化 */
    usart_init(115200);                 /* 串口初始化为115200 */
    led_init();                         /* 初始化LED */
	lcd_init();                             /* 初始化LCD */
	can_init(CAN_SJW_1TQ, CAN_BS2_6TQ, CAN_BS1_7TQ, 6, CAN_MODE_NORMAL);  /* CAN初始化, 环回模式, 波特率500Kbps */
	my_mem_init(SRAMIN);                /* 初始化内部SRAM内存池 */
    USB_Init();                        /* 初始化USB */
	freertos_demo();                    /* 运行FreeRTOS */


}
