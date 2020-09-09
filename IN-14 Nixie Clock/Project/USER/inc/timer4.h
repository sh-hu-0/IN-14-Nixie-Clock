#ifndef TIMER4_H
#define TIMER4_H

#include "stm32f10x.h"

#define TIM4_ON		TIM_Cmd(TIM4,ENABLE)
#define TIM4_OFF	TIM_Cmd(TIM4,DISABLE)

#define Time	!YD_Flag()
#define Date	YD_Flag()

void TIM4_Init(u16 ms);
u8 YD_Flag(void);

#endif

