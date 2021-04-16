#include "timer3.h"
#include "hv57708.h"
#include "ds3231.h"
#include "led.h"

//定时器3用作LED中断定时
void TIM3_Init(u16 ms)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
    TIM_TimeBaseStructure.TIM_Period = ms - 1;			//自动重装载计数值
    TIM_TimeBaseStructure.TIM_Prescaler = 8000 - 1;				//8000分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);				//初始化TIM3
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);					//中断使能
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
    TIM_Cmd(TIM3,ENABLE);										//定时器使能
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3,TIM_IT_Update);//清中断标志位

		

    }
}


