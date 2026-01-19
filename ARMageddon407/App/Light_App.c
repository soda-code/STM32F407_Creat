#include "usart.h"
#include "exfuns.h"
#include "sdio_sdcard.h"
#include "adc.h"
#include "Light_App.h"
#include "usart.h"
Light_TypeDef LightSensor;

//********************************************************************************************
// * @brief       光照传感器初始化函数
// * @note	      本函数用于初始化光照传感器
// * @param       无
// * @retval      无
//********************************************************************************************
void Light_Init(void)
{	
	LightSensor.status = LIGHT_OK;
}

//********************************************************************************************
// * @brief       光照传感器读取函数
// * @note	      本函数用于读取光照传感器的数值
// * @param       无
// * @retval      无
//********************************************************************************************
void Light_Read(void)
{
	if(LightSensor.status ==LIGHT_OK)
	{
		LightSensor.light_value = adc_get_result(ADC_ADCX_CHY);
	}
	else
	{
		
	}
	Get_Light_Test();
}


//********************************************************************************************
// * @brief       光照传感器接口函数
// * @note	      本函数用于获取光照传感器的数值
// * @param       无
// * @retval      无
//********************************************************************************************

uint16_t Get_Light_Data(void)
{
	return LightSensor.light_value;
}

//********************************************************************************************
// * @brief       光照传感器测试函数
// * @note	      本函数用于测试光照传感器的数值
// * @param       无
// * @retval      无
//********************************************************************************************

bool Get_Light_Test(void)
{
		uint8_t send_data[4]={0};
		send_data[0] = LightSensor.light_value&0xff;
		send_data[1] = (LightSensor.light_value>>8)&0xff;
		send_data[2] = 0x2e;
		//Usart_Send(send_data,3);
		my_printf("%d\r\n",LightSensor.light_value);
}


