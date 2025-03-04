#ifndef _MOTOR_H
#define _MOTOR_H
#include "stm32h7xx.h"                  // Device header
#include "stdint.h"

unsigned int rand1();
	
void sportfront(uint16_t speed);
void sportback(uint16_t speed);
void sportright(uint16_t speed);
void sportleft(uint16_t speed);
void sportstop(void);
void qyup(void);
void qyright(void);
void qyleft(void);
void qy_off(void);
void qydown(void);
void qyfront(void);
void qyback(void);
void speed_low(void);
void speed_middle(void);
void speed_hight(void);


#endif

