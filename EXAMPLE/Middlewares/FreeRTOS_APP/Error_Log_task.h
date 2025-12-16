#ifndef __ERROR_LOG_TASK_H
#define __ERROR_LOG_TASK_H

#include "sys.h"
#define MAX_LOG_PAYLOAD_SIZE 100

typedef struct
{
	uint32_t Free_Num;
	uint32_t Toal_Num;
}Nor_Flash;

typedef struct
{
	uint32_t Free_Num;
	uint32_t Toal_Num;
}SD_Struct;


// 定义日志条目结构体
typedef struct
{
    uint32_t timestamp_ms;          // 时间戳：系统启动后的毫秒数
    uint8_t  log_level;             // 日志级别：Fatal/Error/Warn/Info/Debug
    uint16_t error_code;            // 错误代码：预定义的唯一错误ID (DTC或内部代码)
    uint8_t  source_module_id;      // 模块ID：哪个ECU或软件组件产生的日志
    uint8_t  data_size;             // 附加数据长度：data_payload 的实际有效字节数
    uint32_t context_data_1;        // 上下文数据 1：关键状态变量（如车速、电压）
    uint32_t context_data_2;        // 上下文数据 2：另一个状态变量或附加信息
    uint8_t  data_payload[MAX_LOG_PAYLOAD_SIZE]; // 附加数据：可选的原始字节数据或CAN ID
} LogEntry_t;


extern Nor_Flash Flash_Fat_fs;
extern SD_Struct SD_Inf;
void Error_Log_task_create(void);


#endif
