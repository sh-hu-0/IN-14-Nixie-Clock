#ifndef HV57708_H
#define HV57708_H

#include "stm32f10x.h"

#define Normal 0
#define Inv 1
#define NoSend 0x0

#define HV_POL_H    GPIO_SetBits(GPIOC,GPIO_Pin_0)
#define HV_CLK_H    GPIO_SetBits(GPIOC,GPIO_Pin_1)
#define HV_LE_H     GPIO_SetBits(GPIOC,GPIO_Pin_2)
#define HV_DIN1_H   GPIO_SetBits(GPIOA,GPIO_Pin_3)
#define HV_DIN2_H   GPIO_SetBits(GPIOA,GPIO_Pin_2)
#define HV_DIN3_H   GPIO_SetBits(GPIOA,GPIO_Pin_1)
#define HV_DIN4_H   GPIO_SetBits(GPIOC,GPIO_Pin_3)

#define HV_POL_L    GPIO_ResetBits(GPIOC,GPIO_Pin_0)
#define HV_CLK_L    GPIO_ResetBits(GPIOC,GPIO_Pin_1)
#define HV_LE_L     GPIO_ResetBits(GPIOC,GPIO_Pin_2)
#define HV_DIN1_L   GPIO_ResetBits(GPIOA,GPIO_Pin_3)
#define HV_DIN2_L   GPIO_ResetBits(GPIOA,GPIO_Pin_2)
#define HV_DIN3_L   GPIO_ResetBits(GPIOA,GPIO_Pin_1)
#define HV_DIN4_L   GPIO_ResetBits(GPIOC,GPIO_Pin_3)

#define Output	HV57708_Output()
#define Clear	HV57708_Clear()

void HV57708_Init(u8 HV57708_Output_Mode);
void HV57708_SendData(u32 data1,u32 data2);
void HV57708_Output(void);
void HV57708_Clear(void);
u8 Convert_Num(u8 num);
void Display(u32 num);
void Display_Time(void);
void Display_Year(void);
void Display_Date(void);
void No_Display(void);


#endif

