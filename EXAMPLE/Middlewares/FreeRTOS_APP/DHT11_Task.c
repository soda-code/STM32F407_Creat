#include "DHT11_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "dht11.h"

#define 		DHT11_TASK_STACK_SIZE    (2048)
#define 		DHT11_TASK_PRIORITY       4u
TaskHandle_t   	DHT11_task_Handler;      /* 任务句柄 */
void DHT11_task(void *pvParameters);     /* 任务函数 */






SensorData_t sensor_data;




//*********************************************************
//@auto:   Li
//@brief:  DHT11 task function
//@param:  none
//@return: none
//*********************************************************
	float temp=0;
void DHT11_task(void *pvParameters)
{
    while(1)
    {
			IIC_Trigger_Measurement();
			vTaskDelay(100);
			IIC_Read_Sensor_Data(&sensor_data);
			temp=(float)sensor_data.temperature_raw/(1024*1024)*200-50;

			vTaskDelay(1000);
    }
}

//*********************************************************
//@auto:   Li
//@brief:  Create DHT11 task
//@param:  none
//@return: none
//*********************************************************
void Read_DHT20_task_create(void)
{
    xTaskCreate((TaskFunction_t )DHT11_task,(const char*)"DHT11_task",(uint16_t)DHT11_TASK_STACK_SIZE,(void*)NULL,(UBaseType_t)DHT11_TASK_PRIORITY,(TaskHandle_t*  )&DHT11_task_Handler);

}
