#ifndef ADC_H
#define ADC_H

#include "stm32f10x.h"

#define RG Get_ADC_Data()

void ADC1_Init(void);
u16 Get_ADC_Data(u8 ch);
u16 Get_ADC_Average(u8 ch,u8 times);

#endif


