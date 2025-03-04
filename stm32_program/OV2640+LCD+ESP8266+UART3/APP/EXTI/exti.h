#ifndef _exti_H
#define _exti_H

#include "stm32f4xx.h"

//定义引脚端口
#define KEY0_PIN GPIO_Pin_9
#define KEY1_PIN GPIO_Pin_8
#define KEY2_PIN GPIO_Pin_7
#define KEY3_PIN GPIO_Pin_6

void KEYExti_Init(void);

#endif

