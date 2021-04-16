#include "iic.h"
#include "systick.h"

/*
RTC_SCL:PA11	RTC_SDA:PA12	INT/SQW:PA8
*/

void IIC_Start(void)
{
	SCL_H;Dus(5);
	SDA_H;Dus(5);
	SDA_L;Dus(5);
	SCL_L;Dus(5);
}

void IIC_Stop(void)
{
	SDA_L;Dus(5);
	SCL_H;Dus(5);
	SDA_H;Dus(5);
}

void IIC_Ack(void)
{
    SCL_L;Dus(5); 
    SDA_L;Dus(5); 
    SCL_H;Dus(5); 
    SCL_L;Dus(5); 
}

void IIC_NoAck(void)
{
    SCL_L;Dus(5);
    SDA_H;Dus(5); 
    SCL_H;Dus(5);
    SCL_L;Dus(5); 
}  

u8 IIC_WaitAck(void) 
{
    u8 ErrTime = 0;
    SCL_L;Dus(5);
    SDA_H;Dus(5);
    SCL_H;Dus(5);
    while(SDA_Read)
    {
        ErrTime ++;
        if(ErrTime > 250)
        {
            IIC_Stop();
            return 0;
        }
    }
    SCL_L;  
    return 1;  
}  

void IIC_SendByte(u8 data)
{
    u8 i;
    for(i = 0; i < 8; i ++)
    { 
        SCL_L;Dus(5);
		
        if(data & 0x80)
            SDA_H;
        else  SDA_L;
        data <<= 1;
        Dus(5);

        SCL_H;Dus(5);
    }
    SCL_L;
}

u8 IIC_RevByte(void)
{
    u8 i;
    u8 data = 0;
	
    SDA_H;
    for(i = 0; i < 8; i ++)
    {
        SCL_L;Dus(5);
        SCL_H;Dus(5);
        if(SDA_Read)
        {
            data |= (0x80 >> i);
        }
		else  data &= ~(0x80 >> i);
    }
	SCL_L;Dus(5);
    return data;
}


