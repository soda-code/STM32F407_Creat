#include "./BSP/DHT11/dht11.h"
#include "dht11_app.h"
#include "LVGL/GUI_APP/LV_Parment.h"
#include "FreeRTOS.h"
#include "task.h"


#define DHT11_TASK_PRIO       5           /* 任务优先级 */
#define DHT11_STK_SIZE        128         /* 任务堆栈大小 */
TaskHandle_t DHT11Task_Handler;           /* 任务句柄 */
void DHT11_task(void *pvParameters);      /* 任务函数 */




DHT11_Command_t DHT11_CMD=DHT11_CMD_STOP;
uint8_t DHT11_LOCK=0;

extern uint8_t DHT11_tick;
extern uint8_t DHT11_recev_complte;
extern TIM_HandleTypeDef dht11_cap_handle;   /* 定时器x句柄 */
extern uint32_t DHT11_CAP_TICK[MAX_NUM];

DHT11_DATA dht11_data_inf;
uint32_t DHT11_Data=0;
void DHT11_CAP_DATA_DEAL()
{
	uint8_t num=0;
	uint8_t START_Flag=0;

	for(num=0;num<MAX_NUM;num++)
	{
		if((DHT11_CAP_TICK[num]>80)&&(0x00==START_Flag))
		{
			START_Flag=1;
		}
		else if((0x01==START_Flag)&&(num>=6)&&(0x00==(num%2)))
		{
			if(num<70)
			{
				DHT11_Data=DHT11_Data<<1;
				if(DHT11_CAP_TICK[num]>50)
				{
					DHT11_Data|=0X00000001;
				}
				else
				{
					DHT11_Data&=0Xfffffffe;
				}
			}
			
		}
	}
	dht11_data_inf.temp=((DHT11_Data&0x0000ffff)>>8) ;
	dht11_data_inf.humi=(((DHT11_Data>>16)&0x0000ffff)>>8);
	//dht11_data_inf.temp=((DHT11_Data&0x0000ffff)>>8)*10 | (DHT11_Data&0x000000ff);
	//dht11_data_inf.humi=(((DHT11_Data>>16)&0x0000ffff)>>8)*10 | ((DHT11_Data>>16)&0x000000ff);
}

void mode_switch(void) 
{
  if(( CURVE_SCREEN==Screen_INF.Screen_Pointer)&&(!DHT11_LOCK))
  {
  
	 DHT11_CMD=DHT11_CMD_START;
	 DHT11_recev_complte=1;
	 DHT11_LOCK=1;
  }
  else if(CURVE_SCREEN!=Screen_INF.Screen_Pointer)
  {
 	 __HAL_TIM_DISABLE(&dht11_cap_handle);
	DHT11_CMD=DHT11_CMD_STOP;
	DHT11_LOCK=0;
  }
  
}



void DHT11_Send(void) 
{

	__HAL_TIM_DISABLE(&dht11_cap_handle);
	__HAL_TIM_SET_COUNTER(&dht11_cap_handle, 0);  /* 定时器5计数器清零 */
	DHT11_OUT_RE();
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);  // 拉低
	vTaskDelay(18);	// 延时18ms
	DHT11_IN_RE();

	DHT11_CMD=DHT11_CMD_RECEV;
}



void DHT11_Recev(void) 
{
	if(DHT11_recev_complte)
	{
		DHT11_CAP_DATA_DEAL();
		memset(DHT11_CAP_TICK,0,sizeof(DHT11_CAP_TICK));
	
		__HAL_TIM_SET_COUNTER(&dht11_cap_handle, 0);  /* 定时器5计数器清零 */
		__HAL_TIM_ENABLE(&dht11_cap_handle);
		DHT11_tick=0;
		DHT11_recev_complte=0;
		vTaskDelay(2000);
		DHT11_CMD=DHT11_CMD_SEND;
	}

}



void DHT11_task(void *pvParameters)
{

    while(1)
    {
    	mode_switch();
        switch (DHT11_CMD) 
		{
		   case DHT11_CMD_START:
				DHT11_CMD=DHT11_CMD_SEND;
                break;
            case DHT11_CMD_SEND:
               	DHT11_Send();
                break;
            case DHT11_CMD_RECEV:
      			DHT11_Recev();
                break;
            case DHT11_CMD_STOP:
                
                break;
        }
        vTaskDelay(5);
    }
}


void DHT11_APP_Init(void)
{
	    /* DHT11测试任务 */
    xTaskCreate((TaskFunction_t )DHT11_task,(const char* )"DHT11_task",(uint16_t )DHT11_STK_SIZE,(void* )NULL,(UBaseType_t )DHT11_TASK_PRIO,(TaskHandle_t*  )&DHT11Task_Handler);
}

