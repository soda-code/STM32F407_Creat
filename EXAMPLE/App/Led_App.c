#include "usart.h"
#include "exfuns.h"
#include "sdio_sdcard.h"
#include "Led_App.h"
#include "led.h"

void Led_Init(void)
{
	
}
void Led_On(void)
{
	LED1(1);
	LED0(0);
}
void Led_Off(void)
{
	LED1(0);
	LED0(1);
}