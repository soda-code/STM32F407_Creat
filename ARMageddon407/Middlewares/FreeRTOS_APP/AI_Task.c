#include "AI_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "sram.h"
#include "adc.h"

//********************************************************************
#define AI_TASK_STACK_SIZE    		(1024)
#define AI_TASK_PRIORITY      		7u
TaskHandle_t   AI_task_Handler;             /* 任务句柄 */
void AI_task(void *pvParameters);                      /* 任务函数 */



#define NUM_TAPS     64
#define DELAY_SAMPLES 16   // 延迟深度：决定了你能滤除多宽的信号
                           // 滤除固定频率干扰，通常设为 8-32

typedef struct 
{
    float weights[NUM_TAPS];
    float state[NUM_TAPS];
    uint32_t state_idx;
    float mu;
    
    // 延迟线缓冲区，用于创造“伪参考信号”
    float delay_line[DELAY_SAMPLES];
    uint32_t delay_idx;
} LMS_SelfReference;

void LMS_SelfRef_Init(LMS_SelfReference *f, float mu) 
{
    memset(f->weights, 0, sizeof(f->weights));
    memset(f->state, 0, sizeof(f->state));
    memset(f->delay_line, 0, sizeof(f->delay_line));
    f->state_idx = 0;
    f->delay_idx = 0;
    f->mu = mu;
}

/**
 * 只有输入，没有参考的滤波函数
 */
float LMS_SelfRef_Update(LMS_SelfReference *f, float input_sample) 
{
    // 1. 获取伪参考信号：从延迟线中取出一个“过去”的样本
    float x_pseudo_ref = f->delay_line[f->delay_idx];
    
    // 2. 将当前样本存入延迟线，供未来使用
    f->delay_line[f->delay_idx] = input_sample;
    f->delay_idx = (f->delay_idx + 1) % DELAY_SAMPLES;

    // --- 以下进入标准 LMS 流程 ---
    // 3. 更新滤波器状态（循环缓冲区）
    f->state[f->state_idx] = x_pseudo_ref;

    // 4. 计算预测输出 y
    float y = 0.0f;
    uint32_t curr_idx = f->state_idx;
    for (uint32_t i = 0; i < NUM_TAPS; i++) {
        y += f->weights[i] * f->state[curr_idx];
        if (curr_idx == 0) curr_idx = NUM_TAPS - 1;
        else curr_idx--;
    }

    // 5. 计算误差 e
    // 如果目的是“消除周期干扰”，则输出 e (干净信号)
    // 如果目的是“提取周期信号”，则输出 y (干扰信号)
    float e = input_sample - y;

    // 6. 更新权重
    curr_idx = f->state_idx;
    for (uint32_t i = 0; i < NUM_TAPS; i++) {
        f->weights[i] += 2.0f * f->mu * e * f->state[curr_idx];
        if (curr_idx == 0) curr_idx = NUM_TAPS - 1;
        else curr_idx--;
    }

    f->state_idx = (f->state_idx + 1) % NUM_TAPS;

    return e; 
}

//*********************************************************
//@auto 	: 	Li
//@brief	: 	AI task function
//@param	: 	none
//@return	: 	none
//*********************************************************
LMS_SelfReference h_ale;
float tick = 0;

void AI_task(void *pvParameters)
{
    uint16_t n=0;

	LMS_SelfRef_Init(&h_ale, 0.005f); // 步长 0.01


    printf("Index\tInput\tDesired\tOutput\tError\n");
    printf("--------------------------------------\n");
	while(1)
	{
		// 2. 严格对齐采样率
        // 假设 vTaskDelay(10) 是准的 100Hz
        float fs_real = 500.0f; 
        
        // 降低干扰频率确保在 100Hz 采样下不混叠 (比如用 20Hz)
        float signal = arm_sin_f32(2.0f * 3.1415f * 2.0f * tick / fs_real); // 2Hz 有用信号
        float noise  = 0.5f * arm_sin_f32(2.0f * 3.1415f * 20.0f * tick / fs_real); // 20Hz 干扰
        
        float input = signal + noise;

        // 3. 执行滤波
        float output = LMS_SelfRef_Update(&h_ale, input);

        tick++;


		// 每隔100点打印一次收敛情况				
		my_printf("%.3f %.3f %.3f\n", signal, input, output);

	
		vTaskDelay(2);
	}
}

//*********************************************************
//@auto: Li
//@brief: Create AI task
//@param: none
//@return: none
//*********************************************************
void AI_task_create(void)
{
    xTaskCreate((TaskFunction_t )AI_task,(const char*)"AI_task",(uint16_t)AI_TASK_STACK_SIZE,(void*)NULL,(UBaseType_t)AI_TASK_PRIORITY,(TaskHandle_t*  )&AI_task_Handler);

}
