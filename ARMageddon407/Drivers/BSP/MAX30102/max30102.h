
#ifndef __MAX30102_H
#define __MAX30102_H


#include "./SYSTEM/sys/sys.h"

// MAX30102 I2C地址（AD0接地时为0x57，接高电平时为0x58）
#define MAX30102_ADDR   0xAE  // 8位 I2C 写地址

// 核心寄存器地址
#define MAX30102_INT_STATUS_1   0x00
#define MAX30102_INT_STATUS_2   0x01
#define MAX30102_INT_ENABLE_1   0x02
#define MAX30102_INT_ENABLE_2   0x03
#define MAX30102_FIFO_WR_PTR    0x04
#define MAX30102_OVF_COUNTER    0x05
#define MAX30102_FIFO_RD_PTR    0x06
#define MAX30102_FIFO_DATA      0x07
#define MAX30102_FIFO_CONFIG    0x08
#define MAX30102_MODE_CONFIG    0x09
#define MAX30102_SPO2_CONFIG    0x0A
#define MAX30102_LED1_PA        0x0C  // 红光LED
#define MAX30102_LED2_PA        0x0D  // 红外光LED
#define MAX30102_MULTI_LED_CTRL1 0x11
#define MAX30102_MULTI_LED_CTRL2 0x12
#define MAX30102_TEMP_INT       0x1F
#define MAX30102_TEMP_FRAC      0x20
#define MAX30102_TEMP_CONFIG    0x21
#define MAX30102_REVISION_ID    0xFE
#define MAX30102_PART_ID        0xFF

// 模式配置
#define MAX30102_MODE_HR_ONLY   0x02  // 仅心率（红光）
#define MAX30102_MODE_SPO2      0x03  // 血氧（红光+红外光）

// FIFO配置宏
#define MAX30102_SAMPLE_AVG_1   0x00
#define MAX30102_SAMPLE_AVG_4   0x01
#define MAX30102_SAMPLE_AVG_8   0x02
#define MAX30102_SAMPLE_AVG_16  0x03
#define MAX30102_ROLLOVER_ON    0x01
#define MAX30102_ROLLOVER_OFF   0x00


uint8_t MAX30102_WriteReg(uint8_t reg, uint8_t data);
uint8_t MAX30102_ReadReg(uint8_t reg, uint8_t *pdata);

void MAX30102_Init(void);

void MAX30102_Read_FIFO(uint32_t *pun_red_led,uint32_t *pun_ir_led);


#endif


















