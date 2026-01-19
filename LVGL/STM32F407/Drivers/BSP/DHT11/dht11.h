#ifndef __DHT11_H
#define __DHT11_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************
 /* TIMX 输入捕获定义 
 * 这里的输入捕获使用定时器TIM5_CH1,捕获WK_UP按键的输入
 * 默认是针对TIM2~TIM5. 
 * 注意: 通过修改这几个宏定义,可以支持TIM1~TIM8任意一个定时器,任意一个IO口做输入捕获
 *       特别要注意:默认用的PA0,设置的是下拉输入!如果改其他IO,对应的上下拉方式也得改!
 */
#define GTIM_TIMX_CAP_CHY_GPIO_PORT         GPIOA
#define GTIM_TIMX_CAP_CHY_GPIO_PIN          GPIO_PIN_0
#define GTIM_TIMX_CAP_CHY_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */
#define GTIM_TIMX_CAP_CHY_GPIO_AF           GPIO_AF2_TIM5                                 /* AF功能选择 */

#define GTIM_TIMX_CAP                       TIM5                       
#define GTIM_TIMX_CAP_IRQn                  TIM5_IRQn
#define GTIM_TIMX_CAP_IRQHandler            TIM5_IRQHandler
#define GTIM_TIMX_CAP_CHY                   TIM_CHANNEL_1                                 /* 通道Y,  1<= Y <=4 */
#define GTIM_TIMX_CAP_CHY_CCRX              TIM5->CCR1                                    /* 通道Y的输出比较寄存器 */
#define GTIM_TIMX_CAP_CHY_CLK_ENABLE()      do{ __HAL_RCC_TIM5_CLK_ENABLE(); }while(0)    /* TIM5 时钟使能 */

/******************************************************************************************/

#define  MAX_NUM 85


void DHT11_cap_init(uint32_t arr, uint16_t psc);    /* 通用定时器 输入捕获初始化函数 */

void DHT11_OUT_RE(void);
void DHT11_IN_RE(void);;

#endif

