#ifndef DS3231_H
#define DS3231_H

#include "stm32f10x.h"

#define DS3231_WriteAddress	0xd0  
#define DS3231_ReadAddress	0xd1

#define INT_SQW_H	GPIO_SetBits(GPIOA,GPIO_Pin_8);
#define INT_SQW_L	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
#define Get_Day	Convert_Day()

typedef struct
{
    u8	year;
    u8	month;
    u8	date;
    u8	day; 
	u8	hour;
    u8	min;
    u8	sec;        
}td_Type;

extern td_Type td;

void DS3231_Init(void);
void DS3231_Send_Data(u8 addr,u8 data);
void DS3231_Write_Byte(u8 addr,u8 data);
u8 DS3231_Read_Byte(u8 addr);
void Set_Time(u8 year,u8 month,u8 day,u8 date,u8 hour,u8 min,u8 sec);
void Get_Time(void);

u8 BCDtoHEX(u8 val);
u8 HEXtoBCD(u8 val);
u8 * Convert_Day(void);


#endif

