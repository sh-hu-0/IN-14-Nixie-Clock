#include "display.h"
#include "hv57708.h"
#include "max1771.h"
#include "ds3231.h"
#include "led.h"
#include "adc.h"
#include "systick.h"
#include "timer2.h"
#include "timer3.h"
#include "timer4.h"
#include "oled.h"
#include "stdlib.h"
#include "stdbool.h"

bool IN_14_ON = true;
u8 LED_Flag = 0;
u8 Tmp_Sec = 0;
u8 cycle_times = 0;
u8 loading_i = 0;
u8 pattern1,pattern2,pattern3 = 1;
u8 ADC_Times = 0;
u16 ADC_Data[ADC_Count];
u32 ADC_Average = 3000;

void Nixie_Run(void)
{
		if(IN_14_ON)//显示时间
		{
			Boost_ON;Dms(50);Display_Time();
		}

		if(!IN_14_ON && Boosted)//显示日期、熄灭
		{
			Dms(500);
			Display_Year();Dms(750);
			Display_Date();Dms(750);
			Boost_OFF;Dms(1000);
			OLED_OFF;
		}

		if(td.sec == 30 && Boosted)//定时刷新
		{
			Dms(200);Fresh;
		}

		ADC_Data[td.sec] = Get_ADC_Data(9);
		if(Rg < ADC_Th)//判断开启、熄灭
		{
			if(cycle_times == ADC_Count)
			{
				u8 i;
				ADC_Average = 0;
				for(i = 0; i < ADC_Count; i ++)
					ADC_Average += ADC_Data[i];
				
				ADC_Average /= ADC_Count;
				if(ADC_Average <= ADC_Th)
					IN_14_ON = false;
			}
		}
		else IN_14_ON = true;
}

//全数字刷新
void Fresh_Num(void)
{
	All_LED_OFF;
	Display(     9);LED6_XOR;Dms(70);
	Display(    98);LED5_XOR;Dms(70);
	Display(   987);LED4_XOR;Dms(70);
	Display(  9876);LED3_XOR;Dms(70);
	Display( 98765);LED2_XOR;Dms(70);
	Display(987654);LED1_XOR;Dms(70);
	Display(876543);Dms(70);
	Display(765432);Dms(70);
	Display(654321);LED6_XOR;Dms(70);
	Display(543210);LED5_XOR;Dms(70);
	Display(432100);LED4_XOR;Dms(70);
	Display(321000);LED3_XOR;Dms(70);
	Display(210000);LED2_XOR;Dms(70);
	Display(100000);LED1_XOR;Dms(70);
	Display(000000);
	OLED_Clear();
}

//初次开机显示
void OLED_Init_Display(void)
{
	u8 i;
	LED1_ON;
	for(i = 0; i < 10; i ++)
	{
		OLED_DrawLine(i,15,i,16,Inv);
		OLED_Refresh();LED2_ON;
	}
	for(i = 10; i < 24; i ++)
		OLED_DrawLine(i,15,i,16,Inv);
	OLED_Refresh();LED3_ON;
	for(i = 24; i < 48; i ++)
		OLED_DrawLine(i,15,i,16,Inv);
	OLED_Refresh();LED4_ON;
	for(i = 48; i < 72; i ++)
		OLED_DrawLine(i,15,i,16,Inv);
	OLED_Refresh();LED5_ON;
	for(i = 72; i < 96; i ++)
		OLED_DrawLine(i,15,i,16,Inv);
	OLED_Refresh();LED6_ON;
	for(i = 96; i < 128; i ++)
		OLED_DrawLine(i,15,i,16,Inv);
	OLED_Refresh();
	OLED_Clear();
}

//OLED显示pattern序号
void OLED_Pattern(u8 pattern)
{
	if(IN_14_ON)
	{
		switch(pattern)
		{
			case 1:
			{
				OLED_ON;OLED_Clear();
				OLED_ShowString(40,0,"IN-14",16,1);
				OLED_ShowString(16,16,"Nixie Clock",16,1);
				OLED_Refresh();
			}break;
			case 2:
			{
				OLED_ON;OLED_Clear();
				OLED_ShowString(20,0,"Designed by",16,1);
				OLED_ShowString(40,16,"sh.hu",16,1);
				OLED_Refresh();
			}break;
			case 3:
			{
				u8 ymd[10],day[10];
				Get_Time();
				sprintf((u8 *)ymd,"20%d.%d.%d",td.year,td.month,td.date);
				sprintf((u8 *)day,"%s",Get_Day);
				OLED_ON;OLED_Clear();
				OLED_ShowString(8,0,ymd,16,1);
				OLED_ShowString(48,16,day,16,1);
				OLED_Refresh();
			}break;
			
		}
	}
}

//OLED定时显示pattern
void OLED_Switch(void)
{
	if(IN_14_ON)
		switch(td.sec)
		{
			case 0:
			{
				if(pattern1)
					OLED_Pattern(1);
				pattern1 = 0;pattern2 = 1;
			}break;
			case 20:
			{
				if(pattern2)
					OLED_Pattern(2);
				pattern2 = 0;pattern3 = 1;
			}break;
			case 40:
			{
				if(pattern3)
					OLED_Pattern(3);
				pattern3 = 0;pattern1 = 1;
			}break;
		}
}

void LED_Switch(void)
{
	if(Sec_Cng)
	{
		All_LED_OFF;
		LED_Flag ++;
		if(LED_Flag > 6)
			LED_Flag = 1;
		switch(LED_Flag)
		{
			case 1:LED1_ON;break;
			case 2:LED2_ON;break;
			case 3:LED3_ON;break;
			case 4:LED4_ON;break;
			case 5:LED5_ON;break;
			case 6:LED6_ON;break;
		}
	}
}

void Num_Init(void)
{
	u8 i;
	for(i = 0; i < ADC_Count; i ++)
		ADC_Data[i] = 4000;
}

u8 Sec_Change(void)
{
	if(td.sec != Tmp_Sec)
	{
		Tmp_Sec = td.sec;
		cycle_times ++;
		if(cycle_times > ADC_Count)
			cycle_times = 0;
		return true;
	}
	else return false;
}
