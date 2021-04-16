#include "ds3231.h"
#include "systick.h"
#include "iic.h"

/*
RTC_SCL:PA11		RTC_SDA:PA12		INT/SQW:PA8
*/

td_Type td;

void DS3231_Init()
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	//SCL、SDA外部电路3.3V上拉，配置成开漏输出
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	SCL_H;
	SDA_H;
	
	DS3231_Write_Byte(0x0e,0x0);
    Dms(2);
    DS3231_Write_Byte(0x0f,0x0);
    Dms(2);
}

u8 BCDtoHEX(u8 val)
{
    u8 i;
    i = val & 0x0f;
    val >>= 4;
    val &= 0x0f;
    val *= 10;
    i += val;
    
    return i;
}

u8 HEXtoBCD(u8 val)
{
    u8 i,j,k;
    i = val / 10;
    j = val % 10;
    k = j + (i << 4);
	
    return k;
}

void DS3231_Write_Byte(u8 addr,u8 data)
{
    IIC_Start();
    IIC_SendByte(DS3231_WriteAddress);
    IIC_WaitAck();
    IIC_SendByte(addr);
    IIC_WaitAck();
    IIC_SendByte(data);
    IIC_WaitAck();
    IIC_Stop();
}

u8 DS3231_Read_Byte(u8 addr)
{
    u8 data = 0;
  
    IIC_Start();
    IIC_SendByte(DS3231_WriteAddress);
    IIC_WaitAck();
    IIC_SendByte(addr);
    IIC_WaitAck();
	
    IIC_Start();
    IIC_SendByte(DS3231_ReadAddress);
    IIC_WaitAck();
	data = IIC_RevByte();
    IIC_Stop();
    
    return data;
}

void Set_Time(u8 year,u8 month,u8 date,u8 day,u8 hour,u8 min,u8 sec)
{
    DS3231_Write_Byte(0x06,HEXtoBCD(year)); 
    DS3231_Write_Byte(0x05,HEXtoBCD(month));
    DS3231_Write_Byte(0x04,HEXtoBCD(date));
	DS3231_Write_Byte(0x03,HEXtoBCD(day));
    DS3231_Write_Byte(0x02,HEXtoBCD(hour));
    DS3231_Write_Byte(0x01,HEXtoBCD(min));
    DS3231_Write_Byte(0x00,HEXtoBCD(sec));
}

void Get_Time(void)
{
    td.year = BCDtoHEX(DS3231_Read_Byte(0x06));
    td.month = BCDtoHEX(DS3231_Read_Byte(0x05));
    td.date = BCDtoHEX(DS3231_Read_Byte(0x04));
	td.day = BCDtoHEX(DS3231_Read_Byte(0x03));
    td.hour = BCDtoHEX(DS3231_Read_Byte(0x02));	
	td.min = BCDtoHEX(DS3231_Read_Byte(0x01));	
    td.sec = BCDtoHEX(DS3231_Read_Byte(0x00));
}

u8 * Convert_Day(void)
{
	switch(td.day)
	{
		case 1:return "Monday";
		case 2:return "Tuesday";
		case 3:return "Wednesday";
		case 4:return "Thursday";
		case 5:return "Friday";
		case 6:return "Saturday";
		case 7:return "Friday";
	}
}
