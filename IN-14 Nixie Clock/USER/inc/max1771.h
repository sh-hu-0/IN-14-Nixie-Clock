#ifndef MAX1771_H
#define MAX1771_H

#include "stm32f10x.h"

#define Boost_ON	GPIO_ResetBits(GPIOB,GPIO_Pin_0)
#define Boost_OFF	GPIO_SetBits(GPIOB,GPIO_Pin_0)
#define Boost_XOR	GPIOB->ODR ^= (1 << 0)
#define Boosted	Boost_Status()

void MAX1771_Init(void);
u8 Boost_Status(void);


#endif

