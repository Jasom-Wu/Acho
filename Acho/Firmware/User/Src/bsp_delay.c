#include "bsp_delay.h"
#include "main.h"

static uint8_t fac_us = 0;
static uint32_t fac_ms = 0;

void delay_init(void)
{
    fac_us = 1;//SystemCoreClock / 1000000;
    fac_ms = 1;//SystemCoreClock / 1000;
}

void delay_us(uint16_t nus)  //Be Sure Maximum Value Is Under 1000 !!
{
    uint32_t ticks = 0;
    uint32_t told = 0;
    uint32_t tnow = 0;
    uint32_t tcnt = 0;
    uint32_t reload = 0;
    reload = TIM1->ARR + 1;//SysTick->LOAD;
    ticks = nus * fac_us;
    told = TIM1->CNT;//SysTick->VAL;  // 'TIM1->CNT' Measurement is 1Us per-tick
    while (1)
    {
        tnow = TIM1->CNT;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;
            }
        }
    }
}

void delay_ms(uint16_t nms)
{
    HAL_Delay(nms);
//    uint32_t ticks = 0;
//    uint32_t told = 0;
//    uint32_t tnow = 0;
//    uint32_t tcnt = 0;
//    uint32_t reload = 0;
//    reload = 65535;//SysTick->LOAD;  //Truly Maximum Value Is 'pow(2,32)', But This is Enough
//    ticks = nms * fac_ms;
//    told = HAL_GetTick();//SysTick->VAL;  //Function 'HAL_GetTick()' Whose Result Measurement is 1Ms per-tick
//    while (1)
//    {
//        tnow = HAL_GetTick();//SysTick->VAL;
//        if (tnow != told)
//        {
//            if (tnow < told)
//            {
//                tcnt += told - tnow;
//            }
//            else
//            {
//                tcnt += reload - tnow + told;
//            }
//            told = tnow;
//            if (tcnt >= ticks)
//            {
//                break;
//            }
//        }
//    }
}
