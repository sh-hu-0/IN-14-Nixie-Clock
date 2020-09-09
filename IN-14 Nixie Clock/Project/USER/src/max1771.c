#include "max1771.h"

//PB0控制Boost开关，高电平关闭，低电平打开,配置成开漏/推挽输出,速度随意
void MAX1771_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;														//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;														//速度50MHZ
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	Boost_OFF;
}

//升压状态检测
u8 Boost_Status(void)
{
	if(GPIOB->IDR & (1 << 0))
		return 0;
	else return 1;
}
