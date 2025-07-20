#ifndef __LED_APP_H
#define __LED_APP_H

typedef enum 
{
    LED_CMD_ON,
    LED_CMD_OFF,
    LED_CMD_TOGGLE
} LED_Command_t;


void LED_APP_Init(void);   /* lvglÀý³Ì */

#endif
