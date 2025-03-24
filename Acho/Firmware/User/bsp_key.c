#include "bsp_key.h"

static uint8_t KeyPressDetect(KeyTag_Def key_select,uint16_t * key_ms)
{
	uint16_t total_cnt = 0;
	uint8_t press_status =0;
	if(KEY1READ||KEY2READ)
	{
		KEY_DELAY_MS(50);
		if(KEY1READ||KEY2READ)
		{
				__KEY_PROCESS_(KEY1)
				__KEY_PROCESS_(KEY2)
				press_status=1;
		}
	}
	if(key_ms!=NULL)
		*key_ms = total_cnt * TIME_DIV_MS;
	return press_status;
}
