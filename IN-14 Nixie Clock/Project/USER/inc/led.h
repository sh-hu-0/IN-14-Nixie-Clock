#ifndef LED_H
#define LED_H

#include "stm32f10x.h"

#define LED1_ON    	GPIO_ResetBits(GPIOC,GPIO_Pin_5)
#define LED2_ON   	GPIO_ResetBits(GPIOC,GPIO_Pin_4)
#define LED3_ON   	GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define LED4_ON    	GPIO_ResetBits(GPIOA,GPIO_Pin_6)
#define LED5_ON   	GPIO_ResetBits(GPIOA,GPIO_Pin_5)
#define LED6_ON   	GPIO_ResetBits(GPIOA,GPIO_Pin_4)

#define LED1_OFF    GPIO_SetBits(GPIOC,GPIO_Pin_5)
#define LED2_OFF    GPIO_SetBits(GPIOC,GPIO_Pin_4)
#define LED3_OFF    GPIO_SetBits(GPIOA,GPIO_Pin_7)
#define LED4_OFF    GPIO_SetBits(GPIOA,GPIO_Pin_6)
#define LED5_OFF    GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define LED6_OFF    GPIO_SetBits(GPIOA,GPIO_Pin_4)

#define LED1_XOR	GPIOC->ODR ^= (1 << 5)
#define LED2_XOR	GPIOC->ODR ^= (1 << 4)
#define LED3_XOR	GPIOA->ODR ^= (1 << 7)
#define LED4_XOR	GPIOA->ODR ^= (1 << 6)
#define LED5_XOR	GPIOA->ODR ^= (1 << 5)
#define LED6_XOR	GPIOA->ODR ^= (1 << 4)

#define All_LED_ON 	LED1_ON,LED2_ON,LED3_ON,LED4_ON,LED5_ON,LED6_ON
#define All_LED_OFF	LED1_OFF,LED2_OFF,LED3_OFF,LED4_OFF,LED5_OFF,LED6_OFF
#define All_LED_XOR	LED1_XOR,LED2_XOR,LED3_XOR,LED4_XOR,LED5_XOR,LED6_XOR

void LED_Init(void);

#endif

