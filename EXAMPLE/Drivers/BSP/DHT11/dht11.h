 #ifndef __I2C_HW_H__
#define __I2C_HW_H__

#include "sys.h"

#define SENSOR_7BIT_ADDR 0x38       // 设备的 7 位地址
#define SENSOR_8BIT_WRITE_ADDR (SENSOR_7BIT_ADDR << 1) // HAL 库需要的 8 位写地址 0x70
#define SENSOR_8BIT_READ_ADDR  (SENSOR_7BIT_ADDR << 1 | 0x01) // 8 位读取地址 (0x71)

// 定义接收数据结构体，用于存储解析后的数据
typedef struct 
{
    uint8_t status;
    uint32_t humidity_raw;
    uint32_t temperature_raw;
    uint8_t crc;
} SensorData_t;


void DHT11_II2C_Init(void);
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c);

bool Measurement_Start(void);
bool Read_Sensor_Data(SensorData_t *pSensorData);



#endif // __I2C_HW_H__


