#include "timer2.h"

//定时器2用作
void TIM2_Init(u16 ms)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
    TIM_TimeBaseStructure.TIM_Period = ms - 1;			//自动重装载计数值
    TIM_TimeBaseStructure.TIM_Prescaler = 8000 - 1;				//8000分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//向上计数
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);				//初始化TIM2
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);					//中断使能
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
    TIM_Cmd(TIM2,ENABLE);										//定时器使能
}

//定时器2中断服务函数
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2,TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清中断标志位
		
		
    }
}


