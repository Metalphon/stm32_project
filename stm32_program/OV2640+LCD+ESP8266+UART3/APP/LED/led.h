#ifndef _led_H
#define _led_H

#include "stm32f4xx.h"

//ʱ�Ӷ˿ڶ���
#define LED0 PEout(3)
#define LED1 PEout(4)
#define LED2 PGout(9)

void LED_Init(void);  //LED��ʼ������

#endif




