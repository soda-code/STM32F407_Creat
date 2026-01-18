# STM32F407 综合例程 (HAL库 + FreeRTOS + FATFS)

这是一个基于STM32 HAL库开发的STM32F407一站式综合例程，完整整合FreeRTOS实时操作系统与FATFS文件系统，实现LED、KEY、SD卡、NORFLASH、外部SRAM等常用外设的HAL库驱动适配与应用演示，是F407 HAL库开发的入门学习、项目移植的优质参考。

## 📋 核心功能
### 1. 基础框架
- 基于STM32Cube HAL库搭建底层驱动
- FreeRTOS 基于HAL库移植（任务管理、消息队列、信号量等基础功能）
- FATFS 文件系统适配HAL库（支持SD卡/NORFLASH多介质挂载）

### 2. HAL库外设驱动
- **基础外设**：
  - LED：HAL库GPIO驱动（状态指示、闪烁任务）
  - KEY：HAL库GPIO/EXTI中断驱动（按键扫描、中断响应）
- **存储外设**：
  - SD卡：HAL库 SDIO/SPI 驱动（FATFS挂载，文件读写）
  - NORFLASH：HAL库SPI/FMC驱动（扇区擦除、数据读写）
  - EX_SRAM：HAL库FMC驱动（外部SRAM初始化、内存读写）

## 🎯 适用场景
- STM32F407 HAL库入门，快速掌握HAL库+RTOS+文件系统整合开发
- 基于HAL库的工业控制、嵌入式存储类项目底层驱动参考
- STM32CubeMX配置工程的实战移植案例

## 🛠️ 开发环境
- 编译器：Keil MDK-ARM (V5/V6) 或 STM32CubeIDE
- 固件库：STM32CubeF4 HAL库（V1.27.0+，可根据实际版本调整）
- 工具：STM32CubeMX（可选，用于重新配置外设引脚）
- 硬件：STM32F407开发板（带SD卡、NORFLASH、外部SRAM外设）

## 🚀 快速使用
1. 克隆仓库：`git clone [你的仓库地址]`
2. 根据开发环境导入工程，确认HAL库路径配置正确
3. 若需调整外设引脚，可通过STM32CubeMX重新生成HAL库代码并替换对应文件
4. 编译下载到开发板，验证各外设功能

## 📌 注意事项
- 所有外设驱动均基于HAL库标准接口，无寄存器裸机操作，便于移植
- FreeRTOS任务延时、中断处理均适配HAL库中断机制
- FATFS底层磁盘IO接口已基于HAL库实现，可直接替换存储介质驱动
