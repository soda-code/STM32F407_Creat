#ifndef __RTC_READ_APP_H
#define __RTC_READ_APP_H


void RTC_APP_Init(void);   /* lvglÀý³Ì */

typedef struct
{
	uint8_t year;
	uint8_t month;
	uint8_t date;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t reg[2];
}RTC_STRUCT;
extern RTC_STRUCT RTC_Info;

#endif
