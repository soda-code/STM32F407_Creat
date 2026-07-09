#include "./BSP/MAX30102/max30102.h"

I2C_HandleTypeDef hi2c1;

static void MAX30102_IO_Init(void) 
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_I2C1_CLK_ENABLE();

  // 2. 配置 PB6(SCL) 和 PB7(SDA)
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;       // 必须是复用开漏
  GPIO_InitStruct.Pull = GPIO_PULLUP;          // 芯片内部上拉（建议外部再接4.7k电阻）
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; // 高频
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;   // F407特有的复用功能映射：AF4
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  hi2c1.Instance = I2C1;
  // 1. 时钟速率配置
  hi2c1.Init.ClockSpeed = 400000;              // 100kHz (标准模式) 或 400000 (快速模式)
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;     // 快速模式下的占空比，标准模式下此参数无效
  
  // 2. 寻址与模式配置
  hi2c1.Init.OwnAddress1 = 0;                  // 作为从机时的自身地址
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT; // 7位地址模式
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE; // 允许时钟延长

  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {

  }
}

void I2C_Bus_Unclock(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // 1. 先将 SCL/SDA 配置为普通 GPIO 输出模式
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD; // 开漏输出
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    // 2. 如果检测到 SDA 被从机拉低了
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7) == GPIO_PIN_RESET)
    {
        // 连续产生 9 个时钟脉冲，强制让从机释放 SDA 总线
        for(int i = 0; i < 9; i++)
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
            HAL_Delay(1);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
            HAL_Delay(1);
        }
    }
}



// 写 MAX30102 寄存器
uint8_t MAX30102_WriteReg(uint8_t reg, uint8_t data)
{
    return (HAL_I2C_Mem_Write(&hi2c1, MAX30102_ADDR, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 100) == HAL_OK);
}

// 读 MAX30102 寄存器
uint8_t MAX30102_ReadReg(uint8_t reg, uint8_t *pdata)
{
    return (HAL_I2C_Mem_Read(&hi2c1, MAX30102_ADDR, reg, I2C_MEMADD_SIZE_8BIT, pdata, 1, 100) == HAL_OK);
}


void MAX30102_Init(void)
{

  uint8_t id = 0;
  
	MAX30102_IO_Init();
  // 1. 读取芯片 ID 验证通信是否正常 (Part ID 默认通常为 0x11)
  MAX30102_ReadReg(0xFF, &id); 
  if(id != 0x15) 
  {
      // 通信错误或芯片不匹配处理
      return; 
  }

  // 2. 软复位
  MAX30102_WriteReg(0x0C, 0x40); 
  HAL_Delay(10); // 等待复位完成

  // 3. 中断使能配置 (使能 FIFO 几乎满中断、新数据准备就绪中断)
  MAX30102_WriteReg(0x02, 0xC0); 
  MAX30102_WriteReg(0x03, 0x00);

  // 4. FIFO 配置
  // 0x4F: 样本平均(4样本平均), FIFO满时循环覆盖(允许), FIFO几乎满阈值(15个未读数据时触发)
  MAX30102_WriteReg(0x08, 0x4F); 

  // 5. 工作模式配置
  // 0x03: SpO2 模式（红光 LED 和 红外 LED 同时工作，用于心率和血氧）
  // 如果只需要心率，可以配置为 0x02 (Heart Rate Mode，仅红外 LED 工作)
  MAX30102_WriteReg(0x09, 0x03); 

  // 6. SpO2 传感器配置
  // 0x27: LED满量程范围 4096nA, 采样率 100Hz, 脉冲宽度 411us(ADC分辩率18位)
  MAX30102_WriteReg(0x0A, 0x27); 

  // 7. LED 电流控制 (决定发射光的亮度)
  MAX30102_WriteReg(0x0C, 0x24); // 红光 LED 电流 (~7.2mA)
  MAX30102_WriteReg(0x0D, 0x24); // 红外 LED 电流 (~7.2mA)

  // 8. 清空 FIFO 指针，准备开始采集
  MAX30102_WriteReg(0x04, 0x00); // FIFO Write Pointer
  MAX30102_WriteReg(0x05, 0x00); // Overflow Counter
  MAX30102_WriteReg(0x06, 0x00); // FIFO Read Pointer

}



void MAX30102_Read_FIFO(uint32_t *pun_red_led,uint32_t *pun_ir_led)
{
    uint8_t data_buf[6];

    // 连续读取 6 个字节的数据（3字节红光 + 3字节红外）
    // 这里使用 HAL_I2C_Mem_Read 的块读功能，MAX30102 内部地址会自动自增
    if(HAL_I2C_Mem_Read(&hi2c1, MAX30102_ADDR, 0x07, I2C_MEMADD_SIZE_8BIT, data_buf, 6, 100) == HAL_OK)
    {
        // 拼接红光数据 (24位，最高4位无效)
        *pun_red_led = ((uint32_t)data_buf[0] << 16) | ((uint32_t)data_buf[1] << 8) | data_buf[2];
        *pun_red_led &= 0x03FFFF; // 保持 18 位有效数据

        // 拼接红外数据 (24位)
        *pun_ir_led = ((uint32_t)data_buf[3] << 16) | ((uint32_t)data_buf[4] << 8) | data_buf[5];
        *pun_ir_led &= 0x03FFFF;
    }
}

