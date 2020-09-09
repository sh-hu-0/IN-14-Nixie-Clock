#ifndef SYSTICK_H
#define SYSTICK_H

#include "stm32f10x.h"

#define Dms(u32) SysTick_Delay_ms(u32)
#define Dus(u32) SysTick_Delay_us(u32)

void SysTick_Delay_ms(u32 ms);
void SysTick_Delay_us(u32 us);



#endif

