#ifndef _MOTOR_H
#define _MOTOR_H
#include "stm32h7xx.h"                  // Device header
#include "stdint.h"
extern int percent;
extern int qyflag;
void motor_init();
unsigned int rand1();
extern uint16_t speed;
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
void zd();
//tim2
#define lfc1 TIM_CHANNEL_1
#define lfc2 TIM_CHANNEL_2
#define lbc1 TIM_CHANNEL_3
#define lbc2 TIM_CHANNEL_4
//tim3
#define rfc1 TIM_CHANNEL_1
#define rfc2 TIM_CHANNEL_2
#define rbc1 TIM_CHANNEL_3
#define rbc2 TIM_CHANNEL_4
//tim23
#define one TIM_CHANNEL_1
#define high TIM_CHANNEL_1
#define low TIM_CHANNEL_1

#define rf htim3
#define rb htim3

#define lf htim2
#define lb htim2

#define clk_ud htim23


#endif

