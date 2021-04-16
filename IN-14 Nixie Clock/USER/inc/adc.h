#ifndef ADC_H
#define ADC_H

#include "stm32f10x.h"

#define ADC_Count 	60		//ADC测量的次数
#define ADC_Th 		Set_ADC_Th()	//Boost关闭阈值
#define Rg 			Get_ADC_Data(9)

void ADC1_Init(void);
u16 Get_ADC_Data(u8 ch);
u16 Get_ADC_Average(u8 ch,u8 times);
u16 Set_ADC_Th(void);

#endif


