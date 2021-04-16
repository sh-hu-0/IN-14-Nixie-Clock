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
#include "display.h"

int main(int argc, const char * argv[])
{
	Dms(50);
	
	HV57708_Init(Inv);
	DS3231_Init();
	MAX1771_Init();
	LED_Init();
	ADC1_Init();
	Boost_ON;
	
	OLED_Init();
	OLED_ColorInv(Normal);
	OLED_Display_UpDown(Down);
	OLED_Init_Display();
	
	All_LED_OFF;
	No_Display();Boost_ON;Dms(200);
	Get_Time();
	Display_Year();Dms(750);
	Display_Date();Dms(750);
	Boost_OFF;Dms(400);Boost_ON;
	Fresh;
	OLED_Pattern(1);
	
	Num_Init();
	
	//TIM2_Init(1000);//LED中断定时器(ms)
	//TIM3_Init(1000);//OLED中断定时器(ms)
	//TIM4_Init(1000);//光感中断定时器(ms)
	
	//首次设置时间使用,格式：年、月、日、星期、时、分、秒
	//Set_Time(20,9,15,2,11,14,0);
	
	while(1)
	{
		Get_Time();
		Nixie_Run();
		LED_Switch();
		OLED_Switch();
		
//		Display(Rg * 100);Dms(200);
		
	}//
}//



