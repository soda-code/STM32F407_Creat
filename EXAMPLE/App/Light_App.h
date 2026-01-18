#ifndef __LIGHT_APP_H
#define __LIGHT_APP_H

#include "sys.h"

typedef enum
{
    LIGHT_OK = 0,
    LIGHT_ERROR = 1
} Light_StatusTypeDef;

typedef struct
{
    Light_StatusTypeDef status;      // 光照传感器状态
    uint16_t            light_value; // 光照传感器数值
} Light_TypeDef;

extern Light_TypeDef LightSensor;

//********************************************************************************************
// * @brief       光照传感器初始化函数
// * @note	      本函数用于初始化光照传感器
// * @param       无
// * @retval      无
//********************************************************************************************
void Light_Init(void);

//********************************************************************************************
// * @brief       光照传感器读取函数
// * @note	      本函数用于读取光照传感器的数值
// * @param       无
// * @retval      无
//********************************************************************************************
void Light_Read(void);


#endif
