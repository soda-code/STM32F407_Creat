#include "max30102_app.h"
#include "FreeRTOS.h"
#include "task.h"

#define SAMPLE_NODES  100   // 缓冲区大小，存1秒的数据（假设采样率100Hz）

// 环形缓冲区，用于存放原始数据
uint32_t red_buffer[SAMPLE_NODES];
uint32_t ir_buffer[SAMPLE_NODES];
uint16_t buf_index = 0;
uint8_t  buf_full = 0;

// 最终输出的可用数据
float g_heart_rate = 0.0;
float g_spo2 = 0.95; // 默认95%

/**
  * @brief  向缓冲区压入新数据并触发解算
  * @param  red: 读到的红光原始值
  * @param  ir:  读到的红外原始值
  */
void MAX30102_Data_Push(uint32_t red, uint32_t ir)
{
    // 如果手指没放上去（静态值极低），直接清空并退出
    if (ir < 30000) {
        g_heart_rate = 0;
        g_spo2 = 0;
        buf_index = 0;
        buf_full = 0;
        return;
    }

    // 存入环形缓冲区
    red_buffer[buf_index] = red;
    ir_buffer[buf_index] = ir;
    buf_index++;

    if (buf_index >= SAMPLE_NODES) {
        buf_index = 0;
        buf_full = 1; // 缓冲区填满了，可以开始解算
    }

    // 缓冲区满后，每存入一个新点，就计算一次最新的心率血氧（滑动窗口）
    if (buf_full) 

		{
        MAX30102_Calculate_Health();
    }
}

/**
  * @brief  简易时域法解算心率和血氧
  */
void MAX30102_Calculate_Health(void)
{
    uint32_t ir_max = 0, ir_min = 0xFFFFFFFF;
    uint32_t red_max = 0, red_min = 0xFFFFFFFF;
    
    uint16_t peak_count = 0;
    uint16_t last_peak_pos = 0;
    uint32_t total_peak_interval = 0;

    // 1. 寻找这一秒内的最大值、最小值（用于粗略计算DC和AC）
    for (uint16_t i = 0; i < SAMPLE_NODES; i++) {
        if (ir_buffer[i] > ir_max)  ir_max = ir_buffer[i];
        if (ir_buffer[i] < ir_min)  ir_min = ir_buffer[i];
        
        if (red_buffer[i] > red_max) red_max = red_buffer[i];
        if (red_buffer[i] < red_min) red_min = red_buffer[i];
    }

    // 计算红外和红光的 DC 和 AC
    // 直流分量(DC)取平均基线，交流分量(AC)取峰峰值
    float ir_dc = (float)(ir_max + ir_min) / 2.0f;
    float ir_ac = (float)(ir_max - ir_min);
    
    float red_dc = (float)(red_max + red_min) / 2.0f;
    float red_ac = (float)(red_max - red_min);

    // 2. 心率计算：通过动态阈值检测红外波形的波峰数量与间隔
    float threshold = ir_min + (ir_ac * 0.6f); // 将60%高度作为判定阈值
    uint8_t pre_above_thresh = 0;

    for (uint16_t i = 1; i < SAMPLE_NODES - 1; i++) {
        // 检测一阶导数过零点（局部极大值点），且高于阈值
        if (ir_buffer[i] > threshold && ir_buffer[i] > ir_buffer[i-1] && ir_buffer[i] > ir_buffer[i+1]) {
            if (last_peak_pos != 0) {
                uint16_t interval = i - last_peak_pos;
                // 两次心跳间隔合理性过滤（100Hz下，间隔在25-150点之间代表40-240BPM）
                if (interval > 25 && interval < 150) {
                    total_peak_interval += interval;
                    peak_count++;
                }
            }
            last_peak_pos = i;
            i += 10; // 避开同一个波峰附近的抖动毛刺
        }
    }

    // 3. 转换成最终心率值 (BPM)
		// 优化后的心率限幅与平滑滤波
		if (peak_count > 0 && total_peak_interval > 0) {
		    float avg_interval = (float)total_peak_interval / peak_count;
		    float hr_tmp = (60.0f * 100.0f) / avg_interval;
		    
		    // 【优化1】人静止时心率极少超过 140，如果测出 150+，且手没动，大概率是毛刺
		    // 引入滑动平均滤波，让数据更平稳
		    static float last_hr = 75.0f; 
		    if(hr_tmp >= 40.0f && hr_tmp <= 140.0f) {
		        g_heart_rate = (last_hr * 0.8f) + (hr_tmp * 0.2f); // 80%旧数据+20%新数据
		        last_hr = g_heart_rate;
		    } else if (hr_tmp > 140.0f) {
		        // 如果偶尔飙到150+，先压制，不让数据乱跳
		        g_heart_rate = (last_hr * 0.95f) + (120.0f * 0.05f); 
		    }
		}

    // 4. 血氧计算：R = (AC_red / DC_red) / (AC_ir / DC_ir)
    if (ir_dc > 0 && red_dc > 0 && ir_ac > 0) {
        float R = (red_ac / red_dc) / (ir_ac / ir_dc);
        
        // 经典 MAX30102 经验公式: SpO2 = 104 - 17 * R
        float spo2_tmp = 104.0f - 17.0f * R;
        
        if (spo2_tmp > 100.0f) spo2_tmp = 100.0f;
        if (spo2_tmp < 70.0f)  spo2_tmp = 70.0f; // 医疗常规下限
        
        // 平滑滤波，防止血氧数字乱跳
        g_spo2 = (g_spo2 * 0.9f) + (spo2_tmp * 0.1f);
    }
}

uint32_t pun_red_led;
uint32_t pun_ir_led;
void Max_30102_run(void)
{
	MAX30102_Read_FIFO(&pun_red_led,&pun_ir_led);
	// 【新增：把数据送入处理中心】
  MAX30102_Data_Push(pun_red_led, pun_ir_led);
	vTaskDelay(100);
	
}