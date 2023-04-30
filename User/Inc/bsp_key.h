#ifndef __BSP_KEY_H
#define	__BSP_KEY_H
#include "stdint.h"
#include "simple_gpio.h"
#include "FreeRTOS.h"
#include "task.h"
typedef enum
{
	KEY1=0,
	KEY2				
}KeyTag_Def;
typedef enum
{
	LOW=0,
	HIGH	
}KeyLevel_Def;

#define KEY1READ   PAin(0)
#define KEY2READ   PCin(13)
#define TIME_DIV_MS 100
#define VALID_LEVEL HIGH
#define TIME_DIV_MAX 10
#define KEY_DELAY_MS(_ms) vTaskDelay(_ms)

#define __KEY_LINK2(x,y)(x##y)
#define __KEY_PROCESS_(KEYx){if(key_select==KEYx)\
				{\
					while(__KEY_LINK2(KEYx,READ)==VALID_LEVEL)\
					{\
						KEY_DELAY_MS(TIME_DIV_MS);\
						if(++total_cnt>=TIME_DIV_MAX)break;\
					}\
				}\
			}


#endif

