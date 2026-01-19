#ifndef __LV_PARMENTT_H
#define __LV_PARMENTT_H

#include "./SYSTEM/sys/sys.h"

typedef enum
{
	NULL_SCREEN=0,
	MAIN_SCREEN=1,
	CURVE_SCREEN=2,
}_SCREEN;

typedef struct 
{
	_SCREEN Screen_Pointer;
	_SCREEN Screen_Old_Pointer;
	uint8_t go_old_screen;
	uint8_t go_new_screen;
	
}Screen_STRUCT;
extern Screen_STRUCT Screen_INF;

#endif

