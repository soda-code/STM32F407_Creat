#ifndef __DHT11_APP_H
#define __DHT11_APP_H

#include "./SYSTEM/sys/sys.h"
typedef enum 
{
    DHT11_CMD_START=0,
    DHT11_CMD_STOP,
    DHT11_CMD_SEND,
    DHT11_CMD_RECEV,

} DHT11_Command_t;


typedef struct
{
	uint16_t temp;
	uint16_t humi;
	uint16_t check;
	uint16_t reg;
}DHT11_DATA;

extern DHT11_DATA dht11_data_inf;
void DHT11_APP_Init(void);   /* lvgl???? */


#endif

