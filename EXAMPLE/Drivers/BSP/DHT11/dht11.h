 #ifndef __I2C_HW_H__
#define __I2C_HW_H__

#include "stm32f4xx_hal.h"  // 根据你的系列改为 stm32xxx_hal.h
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 外部 I2C 句柄（在 CubeMX 生成的代码里定义） */
extern I2C_HandleTypeDef hi2c1;

/* 库返回值 */
typedef enum {
    I2C_HW_OK = 0,
    I2C_HW_BUSY,
    I2C_HW_ERROR,
    I2C_HW_TIMEOUT,
    I2C_HW_INVALID_PARAM
} i2c_hw_status_t;

/* 初始化（如果使用 CubeMX 可不调用此函数） */
i2c_hw_status_t i2c_hw_init(I2C_HandleTypeDef *hi2c);

/* 阻塞（Polling）模式写 */
i2c_hw_status_t i2c_hw_master_write(I2C_HandleTypeDef *hi2c, uint16_t dev_addr,
                                    uint8_t *pData, uint16_t size, uint32_t timeout);

/* 阻塞（Polling）模式读 */
i2c_hw_status_t i2c_hw_master_read(I2C_HandleTypeDef *hi2c, uint16_t dev_addr,
                                   uint8_t *pData, uint16_t size, uint32_t timeout);

/* 内存寄存器写（8-bit reg）阻塞 */
i2c_hw_status_t i2c_hw_mem_write(I2C_HandleTypeDef *hi2c, uint16_t dev_addr,
                                 uint16_t mem_addr, uint16_t mem_add_size,
                                 uint8_t *pData, uint16_t size, uint32_t timeout);

/* 内存寄存器读（8-bit reg）阻塞 */
i2c_hw_status_t i2c_hw_mem_read(I2C_HandleTypeDef *hi2c, uint16_t dev_addr,
                                uint16_t mem_addr, uint16_t mem_add_size,
                                uint8_t *pData, uint16_t size, uint32_t timeout);

/* 非阻塞中断方式写/读（使用 HAL IT） */
i2c_hw_status_t i2c_hw_master_write_it(I2C_HandleTypeDef *hi2c, uint16_t dev_addr,
                                       uint8_t *pData, uint16_t size);
i2c_hw_status_t i2c_hw_master_read_it(I2C_HandleTypeDef *hi2c, uint16_t dev_addr,
                                      uint8_t *pData, uint16_t size);

/* 非阻塞 DMA 方式写/读 */
i2c_hw_status_t i2c_hw_master_write_dma(I2C_HandleTypeDef *hi2c, uint16_t dev_addr,
                                        uint8_t *pData, uint16_t size);
i2c_hw_status_t i2c_hw_master_read_dma(I2C_HandleTypeDef *hi2c, uint16_t dev_addr,
                                       uint8_t *pData, uint16_t size);

/* 在 HAL 回调中调用（库帮你处理状态）*/
void i2c_hw_evt_callback(I2C_HandleTypeDef *hi2c);
void i2c_hw_err_callback(I2C_HandleTypeDef *hi2c);

/* 工具：转换 7bit/8bit 地址（HAL 使用 7-bit 地址） */
static inline uint16_t i2c_addr_7bit(uint16_t addr8bit) { return (addr8bit >> 1); }

#ifdef __cplusplus
}
#endif

#endif // __I2C_HW_H__
