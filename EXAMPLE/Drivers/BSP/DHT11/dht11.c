#include "delay.h"
#include "dht11.h"
#include "usart.h"

/* i2c.c */

I2C_HandleTypeDef hi2c1;

/**
  * @brief I2C1 Initialization Function
  */
  
// 注：引脚 (GPIO) 和时钟的初始化通常包含在 HAL_I2C_MspInit() 中，
// 也是 CubeMX 自动生成，负责将 PB6/PB7 等引脚映射为 I2C 复用功能并使能时钟
void DHT11_II2C_Init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();	  // PB6 PB7
	__HAL_RCC_I2C1_CLK_ENABLE();	  // ← 必须有这一句

    hi2c1.Instance = I2C1; // 选择 I2C1 外设
    // 时钟配置 (例如，标准模式 100 KHz)
    hi2c1.Init.ClockSpeed = 100000; 
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2; // 占空比 Tlow/Thigh = 2
    
    // 地址配置 (主设备时通常不重要)
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
    	my_printf("IIC_ERROT");
    }
	HAL_I2C_MspInit(&hi2c1);

}
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(hi2c->Instance == I2C1)
    {
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_I2C1_CLK_ENABLE();     // ★★ I2C1 时钟使能 ★★

        GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
	//HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE);
//	HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 1);   // 设置数字滤波为4周期
}


/**
  * @brief I2C 主设备写入指定寄存器
  * @param DevAddr: 设备 I2C 7位地址 (左移一位后的值，如 0xA0)
  * @param RegAddr: 寄存器地址
  * @param pData: 待写入数据指针
  * @param Size: 写入数据长度
  * @retval HAL_StatusTypeDef 状态
  */



// 待发送的数据缓冲区
uint8_t tx_buffer[] = 
{
    0xAC,   // 测量命令 (Command Byte)
    0x33,   // DATA0
    0x00    // DATA1
};

// 发送的数据长度 (3 字节)
uint16_t tx_size = sizeof(tx_buffer);

/**
  * @brief IIC 触发测量及数据写入操作 (对应图示)
  * @retval HAL_StatusTypeDef 状态
  */
HAL_StatusTypeDef IIC_Trigger_Measurement(void)
{
    HAL_StatusTypeDef status;
    
    // 缓冲区已在上面定义

    // 执行 IIC 写入操作
    // 参数: 句柄, 设备地址, 缓冲区, 长度, 超时时间
    status = HAL_I2C_Master_Transmit( &hi2c1,SENSOR_8BIT_WRITE_ADDR,  tx_buffer,tx_size, 1000 );

    return status;
}




/**
  * @brief IIC 读取温湿度数据 (对应图示的连续接收操作)
  * @param pSensorData: 存储解析后数据的结构体指针
  * @retval HAL_StatusTypeDef 状态
  */
HAL_StatusTypeDef IIC_Read_Sensor_Data(SensorData_t *pSensorData)
{
    HAL_StatusTypeDef status;
    // 接收缓冲区总长：1(状态) + 2(湿度) + 2(温度) + 1(CRC) = 6 字节
    uint8_t rx_buffer[6];
    uint16_t rx_size = sizeof(rx_buffer);

    // 1. 调用 HAL_I2C_Master_Receive 执行读取操作
    // HAL 库会自动处理：[START] -> [R_ADDR + ACK] -> [Data... + ACK/NAK] -> [STOP]
    // 自动发送 ACK 直到倒数第二个字节，最后一个字节自动发送 NAK，并发送 STOP。
    status = HAL_I2C_Master_Receive( &hi2c1, SENSOR_8BIT_READ_ADDR, rx_buffer, rx_size, 1000 );

    if (status != HAL_OK)
    {
        // I2C 通信失败，返回错误状态
        return status;
    }

    // 2. 数据解析 (大端格式，即高字节在前)
    if (pSensorData != NULL)
    {
        pSensorData->status = rx_buffer[0];
        
        // 湿度数据：rx_buffer[1] (高字节) + rx_buffer[2] (低字节)
        pSensorData->humidity_raw = (uint16_t)(rx_buffer[1] << 8 | rx_buffer[2]);
        
        // 温度数据：rx_buffer[3] (高字节) + rx_buffer[4] (低字节)
        pSensorData->temperature_raw = (uint16_t)(rx_buffer[3] << 8 | rx_buffer[4]);
        
        // CRC 数据
        pSensorData->crc = rx_buffer[5];
    }
    
    return HAL_OK;
}

