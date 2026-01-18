#include "Ex_Temp_Humit.h"
#include "dht11.h"
#include "FreeRTOS.h"
#include "task.h"

SensorData_t sensor_data;
float temp=0;

//********************************************************************************************
// * @brief       温湿度读取函数
// * @note	      本函数用于读取温湿度传感器的数值
// * @param       无
// * @retval      无
//********************************************************************************************
void Read_Temp_Humit(void)
{
	Measurement_Start();
	vTaskDelay(100);
	Read_Sensor_Data(&sensor_data);
	temp=(float)sensor_data.temperature_raw/(1024*1024)*200-50;
	vTaskDelay(1000);
}