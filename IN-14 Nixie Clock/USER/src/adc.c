#include "adc.h"
#include "ds3231.h"
#include "systick.h"

/*
光敏电阻管脚PB1,对应ADC1通道9
*/

void ADC1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB,&GPIO_InitStructure); 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);		//PCLK六分频
	ADC_DeInit(ADC1);
	
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 		//单一模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;		//数据右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//软件转换
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;					//独立模式
	ADC_InitStructure.ADC_NbrOfChannel = 9;						//顺序进行规则转化的通道数目
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;				//关闭扫描模式
	ADC_Init(ADC1,&ADC_InitStructure);							//ADC1初始化

	ADC_Cmd(ADC1,ENABLE);
	ADC_GetResetCalibrationStatus(ADC1);			//校准复位ADC1
	while(ADC_GetResetCalibrationStatus(ADC1));		//是否完成复位
	ADC_StartCalibration(ADC1);						//校准
	while(ADC_GetCalibrationStatus(ADC1));			//是否完成校准
}

u16 Get_ADC_Data(u8 ch)
{		
	ADC_RegularChannelConfig(ADC1,ch,1,ADC_SampleTime_239Cycles5);//ADC1,通道9，采样时间HSI239.5个周期  
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);			//开启软件转换	
	while(ADC_GetSoftwareStartConvStatus(ADC1));	//是否完成软件转换
	return ADC_GetDualModeConversionValue();		//返回AD值
}

u16 Get_ADC_Average(u8 ch,u8 times)
{
	u32 val = 0;
	u8 t;
	for(t = 0; t < times; t++)
	{
		val += Get_ADC_Data(ch);
	}
	return val / times;
}

//设置光感ADC阈值
u16 Set_ADC_Th(void)
{
	if(td.hour >= 1 && td.hour <= 7)
		return 1000;
	else return 1800;
}

