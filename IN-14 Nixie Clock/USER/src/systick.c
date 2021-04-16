#include "systick.h"

void SysTick_Delay_ms(u32 ms)
{
	SysTick->CTRL &= ~(1 << 2);				//清零
	SysTick->CTRL |= 1 << 2;				//选择HSI 8M时钟
	SysTick->LOAD = ms * 8000;				//设置自动重装载寄存器
	SysTick->VAL = 0;						//当前计数器清零，且到零标志位同样清零
	SysTick->CTRL |= 1 << 0;				//打开定时器使能
	while(!(SysTick->CTRL & (1 << 16)));	//等待计时完成
	SysTick->CTRL &= ~(1 << 0);				//关闭定时器
}

void SysTick_Delay_us(u32 us)
{
	SysTick->CTRL &= ~(1 << 2);				//清零
	SysTick->CTRL |= 1 << 2;				//选择HSI 8M时钟
	SysTick->LOAD = us * 8;					//设置自动重装载寄存器
	SysTick->VAL = 0;						//当前计数器清零，且到零标志位同样清零
	SysTick->CTRL |= 1 << 0;				//打开定时器使能
	while(!(SysTick->CTRL & (1 << 16)));	//等待计时完成
	SysTick->CTRL &= ~(1 << 0);				//关闭定时器
}

