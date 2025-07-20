#include "./BSP/DHT11/dht11.h"
#include "FreeRTOS.h"
#include "task.h"

/*********************************以下是通用定时器输入捕获实验程序*************************************/

TIM_HandleTypeDef dht11_cap_handle;   /* 定时器x句柄 */



void DHT11_OUT_RE(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    gpio_init_struct.Pin = GPIO_PIN_0;                   /* LED0引脚 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;            /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);       /* 初始化LED0引脚 */
    
}
void DHT11_IN_RE(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
	gpio_init_struct.Pin = GTIM_TIMX_CAP_CHY_GPIO_PIN;		/* 输入捕获的GPIO口 */
	gpio_init_struct.Mode = GPIO_MODE_AF_PP;				/* 复用推挽输出 */
	gpio_init_struct.Pull = GPIO_PULLDOWN;					/* 下拉 */
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH; 	/* 高速 */
	gpio_init_struct.Alternate = GTIM_TIMX_CAP_CHY_GPIO_AF; /* 复用为捕获TIM5的通道1 */
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);       /* 初始化LED0引脚 */
    
}


/**
 * @brief       通用定时器TIMX 通道Y 输入捕获 初始化函数
 * @note
 *              通用定时器的时钟来自APB1,当PPRE1 ≥ 2分频的时候
 *              通用定时器的时钟为APB1时钟的2倍, 而APB1为42M, 所以定时器时钟 = 84Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率,单位:Mhz
 *
 * @param       arr: 自动重装值
 * @param       psc: 预分频系数
 * @retval      无
 */
void DHT11_cap_init(uint32_t arr, uint16_t psc)
{
    TIM_IC_InitTypeDef timx_ic_cap_chy = {0};
    
    dht11_cap_handle.Instance = GTIM_TIMX_CAP;                /* 定时器5 */
    dht11_cap_handle.Init.Prescaler = psc;                    /* 预分频系数 */
    dht11_cap_handle.Init.CounterMode = TIM_COUNTERMODE_UP;   /* 向上计数模式 */
    dht11_cap_handle.Init.Period = arr;                       /* 自动重装载值 */
    HAL_TIM_IC_Init(&dht11_cap_handle);                       /* 初始化定时器 */
    
    timx_ic_cap_chy.ICPolarity = TIM_ICPOLARITY_BOTHEDGE;     /* 上升沿捕获 */
    timx_ic_cap_chy.ICSelection = TIM_ICSELECTION_DIRECTTI; /* 映射到TI1上 */
    timx_ic_cap_chy.ICPrescaler = TIM_ICPSC_DIV1;           /* 配置输入分频，不分频 */
    timx_ic_cap_chy.ICFilter = 0;                           /* 配置输入滤波器，不滤波 */
    HAL_TIM_IC_ConfigChannel(&dht11_cap_handle, &timx_ic_cap_chy, GTIM_TIMX_CAP_CHY); /* 配置TIM5通道1 */

    __HAL_TIM_ENABLE_IT(&dht11_cap_handle, TIM_IT_UPDATE);         /* 使能更新中断 */
	HAL_TIM_IC_Start_IT(&dht11_cap_handle, GTIM_TIMX_CAP_CHY);

	
}

/**
 * @brief       通用定时器输入捕获初始化接口
 *              HAL库调用的接口，用于配置不同的输入捕获
 * @param       htim:定时器句柄
 * @note        此函数会被HAL_TIM_IC_Init()调用
 * @retval      无
 */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == GTIM_TIMX_CAP)                        /* 输入通道捕获 */
    {
        GPIO_InitTypeDef gpio_init_struct;
        GTIM_TIMX_CAP_CHY_CLK_ENABLE();                         /* 使能TIMx时钟 */
        GTIM_TIMX_CAP_CHY_GPIO_CLK_ENABLE();                    /* 开启捕获IO的时钟 */

        gpio_init_struct.Pin = GTIM_TIMX_CAP_CHY_GPIO_PIN;      /* 输入捕获的GPIO口 */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;                /* 复用推挽输出 */
        gpio_init_struct.Pull = GPIO_PULLDOWN;                  /* 下拉 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;     /* 高速 */
        gpio_init_struct.Alternate = GTIM_TIMX_CAP_CHY_GPIO_AF; /* 复用为捕获TIM5的通道1 */
        HAL_GPIO_Init(GTIM_TIMX_CAP_CHY_GPIO_PORT, &gpio_init_struct);

        HAL_NVIC_SetPriority(GTIM_TIMX_CAP_IRQn, 1, 3);         /* 抢占1，子优先级3 */
        HAL_NVIC_EnableIRQ(GTIM_TIMX_CAP_IRQn);                 /* 开启ITMx中断 */
    }
}

/* 输入捕获状态(g_timxchy_cap_sta)
 * [7]  :0,没有成功的捕获;1,成功捕获到一次.
 * [6]  :0,还没捕获到高电平;1,已经捕获到高电平了.
 * [5:0]:捕获高电平后溢出的次数,最多溢出63次,所以最长捕获值 = 63*65536 + 65535 = 4194303
 *       注意:为了通用,我们默认ARR和CCRy都是16位寄存器,对于32位的定时器(如:TIM5),也只按16位使用
 *       按1us的计数频率,最长溢出时间为:4194303 us, 约4.19秒
 *
 *      (说明一下：正常32位定时器来说,1us计数器加1,溢出时间:4294秒)
 */


/**
 * @brief       定时器中断服务函数
 * @param       无
 * @retval      无
 */
void GTIM_TIMX_CAP_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&dht11_cap_handle);        /* 定时器共用处理函数 */
}

/**
 * @brief       定时器输入捕获中断处理回调函数
 * @param       htim:定时器句柄指针
 * @note        该函数在HAL_TIM_IRQHandler中会被调用
 * @retval      无
 */
uint8_t DHT11_tick=0;
uint16_t g_timxchy_cap_val ;   /* 输入捕获值 */
uint8_t DHT11_recev_complte=0;
uint32_t DHT11_CAP_TICK_OLD=0;
uint32_t DHT11_CAP_TICK[MAX_NUM]={0};

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
        g_timxchy_cap_val = HAL_TIM_ReadCapturedValue(&dht11_cap_handle, GTIM_TIMX_CAP_CHY);  /* 获取当前的捕获值 */
		DHT11_CAP_TICK[DHT11_tick]=g_timxchy_cap_val-DHT11_CAP_TICK_OLD;
		DHT11_CAP_TICK_OLD=g_timxchy_cap_val;
		DHT11_tick++;
		if(DHT11_tick>MAX_NUM)
		{
			DHT11_tick=0;
			DHT11_recev_complte=1;
		}
		else if(DHT11_CAP_TICK[DHT11_tick-1]==0x00)
		{
			DHT11_tick=0;
			DHT11_recev_complte=1;
		}
			

}


