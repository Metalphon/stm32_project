#ifndef _MOTOR_H
#define _MOTOR_H
#include "stm32h7xx.h"                  // Device header
#include "stdint.h"

unsigned int rand1();
extern uint16_t speed;
void motortest();
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
#define rb htim1
#define lf htim3
#define rf htim4
#define lb htim23
#define sc htim15
#define rbc1 TIM_CHANNEL_2
#define rbc2 TIM_CHANNEL_3
#define lbc1 TIM_CHANNEL_2
#define lbc2 TIM_CHANNEL_3
#define rfc1 TIM_CHANNEL_1
#define rfc2 TIM_CHANNEL_2
#define lfc1 TIM_CHANNEL_3
#define lfc2 TIM_CHANNEL_4
#define scc1 TIM_CHANNEL_1
#endif

