# STM32F407 综合例程 (HAL库 + FreeRTOS + FATFS)

<div align="center">
  <img src="https://img.shields.io/badge/STM32F407-STM32-blue.svg" alt="STM32F407">
  <img src="https://img.shields.io/badge/HAL%20Library-STM32CubeF4-green.svg" alt="HAL">
  <img src="https://img.shields.io/badge/FreeRTOS-v10.x-orange.svg" alt="FreeRTOS">
  <img src="https://img.shields.io/badge/FATFS-R0.15-red.svg" alt="FATFS">
</div>

一个**结构清晰、功能完整、实用性强**的 STM32F407 嵌入式开发模板，适合学习进阶与实际项目使用。

---

## 📋 项目特色

- **完整框架集成**：HAL库 + FreeRTOS + FATFS
- **外设驱动全覆盖**：采用标准 HAL 库接口，无裸寄存器操作，移植性极强
- **RTOS 与外设深度融合**：ADC + DMA + FreeRTOS 低占用设计
- **多存储介质支持**：SD卡 + NOR FLASH 同时挂载 FATFS
- **代码模块化**：驱动层与应用层分离，便于二次开发和扩展

---

## 📁 项目结构
STM32F407_Creat/
├── ARMageddon407/          # Keil 工程主目录
├── DOC/                    # 文档资料（启动流程、版本记录等）
├── README.md
└── ...（其他配置文件）

---

## 🚀 核心功能

### 1. 基础框架
- **FreeRTOS**（基于 HAL 库移植）
  - 任务创建、管理、优先级调度
  - 消息队列、信号量、互斥量、二值信号量
  - 软件定时器、延时函数
- **FATFS 文件系统**
  - 支持 SD 卡（SDIO / SPI）和 NOR FLASH 多介质挂载
  - 完整文件操作（创建、读写、删除、文件夹管理）

### 2. HAL 库外设驱动

**基础外设**
- LED（GPIO + FreeRTOS 任务控制闪烁）
- KEY（GPIO + EXTI 中断 + 软件消抖）

**存储外设**
- SD 卡（SDIO + SPI 双驱动支持）
- NOR FLASH（SPI / FMC）
- EX_SRAM（FMC 外部 SRAM，用于大容量数据缓冲）

**数据采集与通信**
- ADC（多通道 + DMA 连续采样）
- DMA（通用 DMA 驱动，可适配 ADC、串口等）
- 串口（UART/USART 中断 + DMA 双模式）
  - 支持 `printf` 重定向
  - FreeRTOS 专用串口接收任务

---

## 🛠️ 开发环境

- **IDE**：Keil MDK-ARM（推荐最新版本）
- **固件库**：STM32CubeF4 HAL 库（推荐 V1.27.0 及以上）
- **硬件平台**：STM32F407 系列开发板（需支持 SD 卡、NOR FLASH、外部 SRAM）

---

## 🎯 适用人群

- STM32 HAL 库进阶学习者
- 希望一次性掌握 **RTOS + 文件系统 + DMA + 外设综合应用** 的开发者
- 数据采集、工业控制、仪器仪表、嵌入式存储等项目开发者

---

## 🚀 快速上手

1. 克隆本仓库
   ```bash
   git clone https://github.com/soda-code/STM32F407_Creat.git

使用 Keil MDK 打开 ARMageddon407 目录下的工程
检查并配置：
HAL 库路径
FreeRTOS 与 FATFS 配置
根据自己开发板调整外设引脚（如 ADC 通道、串口引脚等）

编译 → 下载 → 通过串口观察运行效果

---
## 📌 注意事项

所有驱动均使用 HAL 标准接口，强烈建议不要随意修改底层寄存器
ADC 使用 DMA 连续采集模式，极大降低 CPU 负载
FATFS 与 DMA、串口任务已做好配合，可直接实现“采集数据 → 存文件”流程
如需修改时钟配置，请同步修改 FreeRTOS 的时基

---
## 📚 文档资料
详见 DOC 文件夹，包含：

STM32 启动流程说明
版本更新记录
常见问题处理等

---
### 💡 后续计划（欢迎 Star & Fork）

完善更多外设例程（SPI、I2C、CAN、ETH 等）
添加更多实用应用层 Demo
优化内存管理与低功耗支持

欢迎大家 Star 支持！
有任何问题或改进建议，欢迎 Issue 或 Pull Request。

**作者**：soda-code
**邮箱**：18895391769@163.com
