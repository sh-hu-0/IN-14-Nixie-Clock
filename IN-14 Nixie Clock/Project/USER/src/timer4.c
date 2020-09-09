#include "timer4.h"
#include "max1771.h"
#include "adc.h"
#include "led.h"

u8 TIM4_Flag = 0;
u8 TD = 1;			//1显示时间
u8 RG_Th = 150;
u16 ADC_Th = 1800;

//定时器4用作光感中断定时
void TIM4_Init(u16 ms)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
    TIM_TimeBaseStructure.TIM_Period = ms - 1;			//自动重装载计数值
    TIM_TimeBaseStructure.TIM_Prescaler = 8000 - 1;				//8000分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);				//初始化TIM4
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);					//中断使能
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
    TIM_Cmd(TIM4,ENABLE);										//定时器使能
}

//定时器4中断服务函数
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4,TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//清中断标志位

		if(TIM4_Flag < RG_Th)//RG_Th次中断，即RG_Th个定时器3设定时间后开始启动光感开关
		{	
			TIM4_Flag ++;
		}
		if(Get_ADC_Average(9,10) < ADC_Th && TIM4_Flag >= 150)
		{
			TD = 0;
		}
		else
		{
			TD = 1;
		}
    }
}

//显示时间/日期标志位
u8 YD_Flag(void)
{
	if(TD)
		return 0;
	else return 1;
}
