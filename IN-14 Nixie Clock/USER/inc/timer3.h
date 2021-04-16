#ifndef TIMER3_H
#define TIMER3_H

#include "stm32f10x.h"

#define TIM3_ON		TIM_Cmd(TIM3,ENABLE)
#define TIM3_OFF	TIM_Cmd(TIM3,DISABLE)

void TIM3_Init(u16 ms);

#endif

