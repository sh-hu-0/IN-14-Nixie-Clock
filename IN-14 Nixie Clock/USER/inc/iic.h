#ifndef IIC_H
#define IIC_H

#include "stm32f10x.h"

#define SCL_H GPIO_SetBits(GPIOA,GPIO_Pin_11)
#define SCL_L GPIO_ResetBits(GPIOA,GPIO_Pin_11)	

#define SDA_H GPIO_SetBits(GPIOA,GPIO_Pin_12)
#define SDA_L GPIO_ResetBits(GPIOA,GPIO_Pin_12)

#define SDA_Read  GPIOA->IDR & (1 << 12)

void IIC_Start(void);
void IIC_Stop(void);
void IIC_Ack(void);
void IIC_NoAck(void);
u8 IIC_WaitAck(void);
void IIC_SendByte(u8 data);
u8 IIC_RevByte(void);


#endif



