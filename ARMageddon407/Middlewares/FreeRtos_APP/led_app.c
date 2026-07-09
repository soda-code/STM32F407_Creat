#include "led_app.h"
#include "./BSP/LED/led.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"

#include "./BSP/NORFLASH/norflash.h"

/* 要写入到FLASH的字符串数组 */
 uint8_t g_text_buf[] = {"STM32 SPI TEST"};

#define TEXT_SIZE   sizeof(g_text_buf)      /* TEXT字符串长度 */

uint8_t datatemp[TEXT_SIZE];

uint32_t flashsize = 16 * 1024 * 1024; 	/* FLASH 大小为16M字节 */


void led_run(void)
{

    while (1)
    {
        LED0_TOGGLE();
        LED1_TOGGLE();
				norflash_write((uint8_t *)g_text_buf, flashsize - 100, TEXT_SIZE);      /* 从倒数第100个地址处开始,写入SIZE长度的数据 */
			  vTaskDelay(500); /* 延时500ms */
				g_text_buf[0]++;
        norflash_read(datatemp, flashsize - 100, TEXT_SIZE);       
        vTaskDelay(500); /* 延时500ms */
    }
}
