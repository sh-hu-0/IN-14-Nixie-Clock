//
//  main.c
//  IN-14 Nixie Clock
//
//  Created by Samlany on 2018/6/1.
//  Copyright @ 2020 Pineapple. All rights reserved.
//

#include "stm32f10x.h"
#include "hv57708.h"
#include "ds3231.h"
#include "max1771.h"
#include "systick.h"
#include "led.h"
#include "iic.h"
#include "adc.h"
#include "timer2.h"
#include "timer3.h"
#include "timer4.h"
#include "oled.h"

#define All_TIM_ON	TIM2_ON,TIM3_ON,TIM4_ON
#define All_TIM_OFF	TIM2_OFF,TIM3_OFF,TIM4_OFF

int main(int argc, const char * argv[])
{
	Dms(50);
	
	HV57708_Init(Inv);
	DS3231_Init();
	MAX1771_Init();
	LED_Init();
	ADC1_Init();
	
	All_LED_OFF;
	No_Display();Boost_ON;Dms(50);Get;
	Display_Year();Dms(500);
	Display_Date();Dms(500);
	Boost_OFF;Dms(300);Boost_ON;
	Fresh;
	
	TIM3_Init(1000);//LED中断定时器(ms)
	TIM4_Init(100);//光感中断定时器(ms)
	
	//首次设置时间使用,格式：年、月、日、星期、时、分、秒
	//Set_Time(20,8,22,6,17,57,0);
	
	while(1)
	{
		if(Time)//显示时间
		{
			Boost_ON;Get;Run;
		}
		
		if(Date)//显示日期
		{
			Dms(500);
			Display_Year();Dms(500);
			Display_Date();Dms(500);
			Boost_OFF;Dms(1000);
		}
		
		if(td.sec == 30 && Boost_Status())//定时刷新
		{
			Dms(200);Fresh;
		}
		
		
//		Display(Get_ADC_Data(9) * 100);Dms(200);
		
	}//
}//



