# STM32F407 综合例程 (HAL库 + FreeRTOS + FATFS)

## 📋 核心功能
### 1. 基础框架
- FreeRTOS 基于HAL库移植（任务管理、消息队列、信号量、延时等基础功能）
- FATFS 文件系统适配HAL库（支持SD卡/NORFLASH多介质挂载、文件读写）

### 2. HAL库外设驱动（全量）
- **基础控制外设**：
  - LED：HAL库GPIO驱动（状态指示、FreeRTOS任务控制闪烁）
  - KEY：HAL库GPIO/EXTI中断驱动（按键扫描、中断响应、消抖处理）
- **存储外设**：
  - SD卡：HAL库 SDIO/SPI 驱动（FATFS挂载，文件读写）
  - NORFLASH：HAL库SPI/FMC驱动（扇区擦除、数据读写）
  - EX_SRAM：HAL库FMC驱动（外部SRAM初始化、内存读写、数据缓存）
- **数据采集/传输外设**：
  - ADC：HAL库方式采集（多通道连续采样、数据缓存、数值转换）
  - DMA：HAL库通用DMA驱动（适配ADC采集、串口数据传输）
  - 串口(UART/USART)：HAL库中断/DMA模式（数据收发、printf重定向、FreeRTOS串口任务）

## 🎯 适用场景
- STM32F407 HAL库入门，掌握HAL库+RTOS+文件系统+DMA/ADC/串口全流程开发
- 工业控制、数据采集、嵌入式存储类项目的底层驱动参考

## 🛠️ 开发环境
- 编译器：Keil MDK-ARM
- 固件库：STM32CubeF4 HAL库（V1.27.0+，可根据实际版本调整）
- 硬件：STM32F407开发板（需带对应外设：SD卡、NORFLASH、外部SRAM、ADC采集通道）

## 🚀 快速使用
1. 克隆仓库：`git clone [git@github.com:soda-code/STM32F407_Creat.git]
2. 根据开发环境导入工程，确认HAL库路径、FreeRTOS/FATFS配置正确
3. 若外围电路有差异请调整外设参数（如ADC采样通道、串口波特率）
4. 编译下载到开发板，验证各外设功能（如ADC采样值串口打印、SD卡文件写入）

## 📌 注意事项
- 所有外设均基于HAL库标准接口开发，无寄存器裸操作，移植性强
- ADC采用DMA连续采集，避免CPU占用，适配FreeRTOS任务调度
- 串口支持中断/DMA双模式，可根据数据量灵活切换
- FATFS底层IO接口与DMA/串口无冲突，可实现采集数据直接存储到SD卡
