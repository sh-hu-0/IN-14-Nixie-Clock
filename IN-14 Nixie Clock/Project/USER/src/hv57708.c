#include "hv57708.h"
#include "systick.h"
#include "ds3231.h"
#include "led.h"
#include "timer3.h"

/*
HV57708:具有推挽输出的64通道32MHz串并转换器
DIR引脚连接到GND时逆时针移位，接到VDD时顺时针移位,此处顺时针
LE为高电平时，将数据从移位寄存器传输到锁存器
LE为低电平时，锁存器中的数据被存储

已带有5V上拉电阻，以下管脚配置成开漏输出，输出速度大于8M
HV_POL:PC0	HV_CLK:PC1	HV_LE:PC2
HV_DIN1:PA3		HV_DIN2:PA2
HV_DIN3:PA1		HV_DIN4:PC3

HV57708输出模式设置
标准输出模式	Normal	HV57708_Output_Mode = 1
反相输出模式	Inv		HV57708_Output_Mode = 0
此处必须工作为反向模式
*/

u8 IN_14_Location[6][10] = 
{
	//	0  1  2  3  4  5  6  7  8  9
		55,56,57,58,59,60,51,52,53,54,	//N1
		45,46,47,48,49,50,41,42,43,44,	//N2
		35,36,37,38,39,40,31,32,33,34,	//N3
		25,26,27,28,29,30,21,22,23,24,	//N4
		15,16,17,18,19,20,11,12,13,14,	//N5
		 5, 6, 7, 8, 9, 10,1, 2, 3, 4	//N6
	//对应HV_OUT端口
};

//HV57708控制管脚初始化函数
void HV57708_Init(u8 HV57708_Output_Mode)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	//HV_POL:PC0	HV_CLK:PC1		HV_LE:PC2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	//HV_DIN1:PA3 	HV_DIN2:PA2		HV_DIN3:PA1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//HV_DIN4:PC3
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	//正相、反相输出
	if(HV57708_Output_Mode)
		HV_POL_L;
	else HV_POL_H;
	
	HV_DIN1_L;
	HV_DIN2_L;
	HV_DIN3_L;
	HV_DIN4_L;
	HV_CLK_L;
	HV_LE_L;
}


//向HV57708发送64位数据函数,分为两部分发送
void HV57708_SendData(u32 data1,u32 data2)
{
	u8 i;
	u32 tmp = data1;
	if(data1 != NoSend)
		for(i = 0; i < 8; i++)
		{
			HV_CLK_L;
			if((tmp & 1) == 1)
				HV_DIN4_H;
			else HV_DIN4_L;
			tmp >>= 1;
			if((tmp & 1) == 1)
				HV_DIN3_H;
			else HV_DIN3_L;
			tmp >>= 1;
			if((tmp & 1) == 1)
				HV_DIN2_H; 
			else HV_DIN2_L;
			tmp >>= 1;
			if((tmp & 1) == 1)
				HV_DIN1_H;
			else HV_DIN1_L;
			tmp >>= 1;
			
			HV_CLK_H;
			HV_CLK_L;
		}
	
	tmp = data2;
	if(data2 != NoSend)
		for(i = 0; i < 8; i++)
		{
			HV_CLK_L;
			if((tmp & 1) == 1)
				HV_DIN4_H;
			else HV_DIN4_L;
			tmp >>= 1;
			if((tmp & 1) == 1)
				HV_DIN3_H;
			else HV_DIN3_L;
			tmp >>= 1;
			if((tmp & 1) == 1)
				HV_DIN2_H;
			else HV_DIN2_L;
			tmp >>= 1;
			if((tmp & 1) == 1)
				HV_DIN1_H;
			else HV_DIN1_L;
			tmp >>= 1;
			
			HV_CLK_H;
			HV_CLK_L;	
		}
}

//锁存器使能，将数据输出
void HV57708_Output(void)
{
	HV_LE_H;
	HV_LE_L;
}

//清除全部数字
void HV57708_Clear(void)
{
	HV57708_SendData(0xffffffff,0xffffffff);
	Output;
}

//数字与对应管脚数值转换
u8 Convert_Num(u8 num)
{
	//显示数字：	5 4 3 2 1 0 9 8 7 6
	//移位序号：	1 2 3 4 5 6 7 8 9 0
	if(num > 0 && num <= 5)
		num = 6 - num;
	if(num >= 6 && num <= 9)
		num = 16 - num;
	if(num == 0)
		num = 6;
	return num;
}

//显示任意数字函数,如显示“123456”则为Display(123456);
void Display(u32 num)
{
	u32 data1,data2;
	u8 n1,n2,n3,n4,n5,n6;	//管1-6对应时高，时低，分高，分低，秒高，秒低
	n1 = Convert_Num(num / 100000 % 10) + (10 * 0) + 3;
	n2 = Convert_Num(num / 10000 % 10) 	+ (10 * 1) + 3;
	n3 = Convert_Num(num / 1000 % 10) 	+ (10 * 2) + 3;
	n4 = Convert_Num(num / 100 % 10) 	+ (10 * 0) + 1;
	n5 = Convert_Num(num / 10 % 10) 	+ (10 * 1) + 1;
	n6 = Convert_Num(num % 10) 			+ (10 * 2) + 1;
	
	if(n3 <= 8 + (10 * 2) + 3)//第三管数字6、7在第二部分数据
	{
		data1 = 0xffffffff ^ (1 << n1) ^ (1 << n2) ^ (1 << n3);
		data2 = 0xffffffff ^ (1 << n4) ^ (1 << n5) ^ (1 << n6);
	}
	else
	{
		data1 = 0xffffffff ^ (1 << n1) ^ (1 << n2);
		data2 = 0xffffffff ^ (1 << (n3 - (10 * 3) - 2)) ^ (1 << n4) ^ (1 << n5) ^ (1 << n6);
	}
	
	HV57708_SendData(data1,data2);
	Output;
}

//显示时间
void Display_Time(void)
{
	u32 data1,data2;
	u8 n1,n2,n3,n4,n5,n6;	//管1-6对应时高，时低，分高，分低，秒高，秒低
	n1 = Convert_Num(td.hour / 10 % 10) + (10 * 0) + 3;
	n2 = Convert_Num(td.hour % 10) 		+ (10 * 1) + 3;
	n3 = Convert_Num(td.min / 10 % 10) 	+ (10 * 2) + 3;
	n4 = Convert_Num(td.min % 10) 		+ (10 * 0) + 1;
	n5 = Convert_Num(td.sec / 10 % 10) 	+ (10 * 1) + 1;
	n6 = Convert_Num(td.sec % 10) 		+ (10 * 2) + 1;
	
	if(n3 <= 8 + (10 * 2) + 3)//第三管数字6、7在第二部分数据
	{
		data1 = 0xffffffff ^ (1 << n1) ^ (1 << n2) ^ (1 << n3);
		data2 = 0xffffffff ^ (1 << n4) ^ (1 << n5) ^ (1 << n6);
	}
	else
	{
		data1 = 0xffffffff ^ (1 << n1) ^ (1 << n2);
		data2 = 0xffffffff ^ (1 << (n3 - (10 * 3) - 2)) ^ (1 << n4) ^ (1 << n5) ^ (1 << n6);
	}
	
	HV57708_SendData(data1,data2);
	Output;
}

//显示年
void Display_Year(void)
{
	u32 data1,data2;
	u8 n1,n2,n3,n4,n5,n6;
	n1 = Convert_Num(2) 				+ (10 * 0) + 3;
	n2 = Convert_Num(0) 				+ (10 * 1) + 3;
	n3 = Convert_Num(td.year / 10 % 10) + (10 * 2) + 3;
	n4 = Convert_Num(td.year % 10) 		+ (10 * 0) + 1;
	n5 = 0;
	n6 = 0;
	
	if(n3 <= 8 + (10 * 2) + 3)//第三管数字6、7在第二部分数据
	{
		data1 = 0xffffffff ^ (1 << n1) ^ (1 << n2) ^ (1 << n3);
		data2 = 0xffffffff ^ (1 << n4) ^ (1 << n5) ^ (1 << n6);
	}
	else
	{
		data1 = 0xffffffff ^ (1 << n1) ^ (1 << n2);
		data2 = 0xffffffff ^ (1 << (n3 - (10 * 3) - 2)) ^ (1 << n4) ^ (1 << n5) ^ (1 << n6);
	}
	
	HV57708_SendData(data1,data2);
	Output;
}

//显示日期
void Display_Date(void)
{
	u32 data1,data2;
	u8 n1,n2,n3,n4,n5,n6;
	n1 = 0;
	n2 = 0;
	n3 = Convert_Num(td.month / 10 % 10) + (10 * 2) + 3;
	n4 = Convert_Num(td.month % 10) 	 + (10 * 0) + 1;
	n5 = Convert_Num(td.date / 10 % 10)  + (10 * 1) + 1;
	n6 = Convert_Num(td.date % 10) 		 + (10 * 2) + 1;
	
	if(n3 <= 8 + (10 * 2) + 3)//第三管数字6、7在第二部分数据
	{
		data1 = 0xffffffff ^ (1 << n1) ^ (1 << n2) ^ (1 << n3);
		data2 = 0xffffffff ^ (1 << n4) ^ (1 << n5) ^ (1 << n6);
	}
	else
	{
		data1 = 0xffffffff ^ (1 << n1) ^ (1 << n2);
		data2 = 0xffffffff ^ (1 << (n3 - (10 * 3) - 2)) ^ (1 << n4) ^ (1 << n5) ^ (1 << n6);
	}
	
	HV57708_SendData(data1,data2);
	Output;
}

void No_Display(void)
{
	u32 data1,data2;
	data1 = 0xffffffff;
	data2 = 0xffffffff;
	
	HV57708_SendData(data1,data2);
	Output;
}

//全数字刷新
void Fresh_Num(void)
{
	TIM3_OFF;All_LED_OFF;
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
	TIM3_ON;
	
}


