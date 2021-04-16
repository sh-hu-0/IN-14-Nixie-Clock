#ifndef DISPLAY_H
#define DISPLAY_H 

#include "stm32f10x.h"

#define All_TIM_ON	TIM2_ON,TIM3_ON,TIM4_ON
#define All_TIM_OFF	TIM2_OFF,TIM3_OFF,TIM4_OFF

#define Fresh	Fresh_Num()
#define Sec_Cng	Sec_Change()

void Nixie_Run(void);
void Fresh_Num(void);
void OLED_Init_Display(void);
void OLED_Pattern(u8 pattern);
void OLED_Switch(void);
void LED_Switch(void);
void Num_Init(void);
u8 Sec_Change(void);

#endif

