#ifndef TIMER2_H
#define TIMER2_H

#include "stm32f10x.h"

#define TIM2_ON		TIM_Cmd(TIM2,ENABLE)
#define TIM2_OFF	TIM_Cmd(TIM2,DISABLE)

void TIM2_Init(u16 ms);

#endif

