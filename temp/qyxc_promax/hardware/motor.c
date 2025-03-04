#include "motor.h"
#include "stm32h7xx.h"                  // Device header
#include "usart.h"
#include <stdint.h>
#include "tim.h"
#include "ctrl.h"
#include "pid.h"

//htim1  ��
//htim4  ��


//htim1  1
uint16_t speed ;
int percent;
int qyflag;


// α��������ɺ������򵥵�����ͬ�෽����
uint32_t seed = 2067713761; // ����

unsigned int rand1() {
    seed = (seed * 1103515245 + 12345) & 0x7FFFFFFF;
    return seed;
}

void motor_init()
{ 
	//A0A1��ǰ   A2A3���
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);
  //C6C7��ǰ   C8C9�Һ�
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
	//F0F1��ˢ   F2F3��ˢ
	//HAL_TIM_PWM_Start(&sc,TIM_CHANNEL_1);
 // HAL_TIM_PWM_Start(&sc,TIM_CHANNEL_2);
 // HAL_TIM_PWM_Start(&sc,TIM_CHANNEL_3);
 // HAL_TIM_PWM_Start(&sc,TIM_CHANNEL_4);
	  //HAL_TIM_PWM_Start(&htim23,TIM_CHANNEL_1);
		HAL_GPIO_WritePin(en_GPIO_Port, en_Pin, GPIO_PIN_SET);

  	TIM2->CCR1=0; 
		TIM2->CCR2=0; 
		TIM2->CCR3=0; 
		TIM2->CCR4=0; 
	
	
		TIM3->CCR1=0; 
		TIM3->CCR2=0; 
		TIM3->CCR3=0; 
		TIM3->CCR4=0; 

		TIM23->CCR1=0; 
		TIM23->CCR2=0; 
		TIM23->CCR3=0; 
		TIM23->CCR4=0; 
	
}
void zd()
{
	qydown();
		HAL_Delay(13000);
	
	qyfront();
sportfront(800);
	
	HAL_Delay(5000);
		sportstop();
	
		HAL_Delay(3000);

	qyright();
	qyleft();
	sportback(800);
	
	HAL_Delay(5000);
		sportstop();
}
/*
void motortest(){
		HAL_TIM_PWM_Start(&rf, rfc1);
		__HAL_TIM_SET_COMPARE(&rf,rfc1,speed);
			HAL_TIM_PWM_Stop(&rf, rfc2);
}
*/
void sportfront(uint16_t speed)
{
		HAL_TIM_PWM_Start(&rb, rbc1);
		__HAL_TIM_SET_COMPARE(&rb,rbc1,speed);

		HAL_TIM_PWM_Start(&rf, rfc1);
		__HAL_TIM_SET_COMPARE(&rf,rfc1,speed);

		HAL_TIM_PWM_Start(&lb, lbc2);
		__HAL_TIM_SET_COMPARE(&lb,lbc2,speed);

		HAL_TIM_PWM_Start(&lf, lfc2);
		__HAL_TIM_SET_COMPARE(&lf,lfc2,speed);

		HAL_TIM_PWM_Stop(&rb, rbc2);
		HAL_TIM_PWM_Stop(&rf, rfc2);
		HAL_TIM_PWM_Stop(&lb, lbc1);
		HAL_TIM_PWM_Stop(&lf, lfc1);
}

void sportback(uint16_t speed)
{
	HAL_TIM_PWM_Start(&rb, rbc2);
	__HAL_TIM_SET_COMPARE(&rb,rbc2,speed);

	HAL_TIM_PWM_Start(&rf, rfc2);
	__HAL_TIM_SET_COMPARE(&rf,rfc2,speed);

	HAL_TIM_PWM_Start(&lb, lbc1);
	__HAL_TIM_SET_COMPARE(&lb,lbc1,speed);

	HAL_TIM_PWM_Start(&lf, lfc1);
	__HAL_TIM_SET_COMPARE(&lf,lfc1,speed);

	HAL_TIM_PWM_Stop(&rb, rbc1);
	HAL_TIM_PWM_Stop(&rf, rfc1);
	HAL_TIM_PWM_Stop(&lb, lbc2);
	HAL_TIM_PWM_Stop(&lf, lfc2);
}

void sportright(uint16_t speed)
{
HAL_TIM_PWM_Start(&rb, rbc2);
		__HAL_TIM_SET_COMPARE(&rb,rbc2,speed);

		HAL_TIM_PWM_Start(&rf, rfc2);
		__HAL_TIM_SET_COMPARE(&rf,rfc2,speed);

		HAL_TIM_PWM_Start(&lb, lbc2);
		__HAL_TIM_SET_COMPARE(&lb,lbc2,speed);

		HAL_TIM_PWM_Start(&lf, lfc2);
		__HAL_TIM_SET_COMPARE(&lf,lfc2,speed);

		HAL_TIM_PWM_Stop(&rb, rbc1);
		HAL_TIM_PWM_Stop(&rf, rfc1);
		HAL_TIM_PWM_Stop(&lb, lbc1);
		HAL_TIM_PWM_Stop(&lf, lfc1);	
}

void sportleft(uint16_t speed)
{
	HAL_TIM_PWM_Start(&rb, rbc1);
		__HAL_TIM_SET_COMPARE(&rb,rbc1,speed);

		HAL_TIM_PWM_Start(&rf, rfc1);
		__HAL_TIM_SET_COMPARE(&rf,rfc1,speed);

		HAL_TIM_PWM_Start(&lb, lbc1);
		__HAL_TIM_SET_COMPARE(&lb,lbc1,speed);

		HAL_TIM_PWM_Start(&lf, lfc1);
		__HAL_TIM_SET_COMPARE(&lf,lfc1,speed);

		HAL_TIM_PWM_Stop(&rb, rbc2);
		HAL_TIM_PWM_Stop(&rf, rfc2);
		HAL_TIM_PWM_Stop(&lb, lbc2);
		HAL_TIM_PWM_Stop(&lf, lfc2);
}

void sportstop()
{
	HAL_TIM_PWM_Stop(&rb, rbc1);
	HAL_TIM_PWM_Stop(&rb, rbc2);
	HAL_TIM_PWM_Stop(&lb, lbc1);
	HAL_TIM_PWM_Stop(&lb, lbc2);
	HAL_TIM_PWM_Stop(&rf, rfc1);
	HAL_TIM_PWM_Stop(&rf, rfc2);
	HAL_TIM_PWM_Stop(&lf, lfc1);
	HAL_TIM_PWM_Stop(&lf, lfc2);
	//HAL_TIM_PWM_Stop(&sc, one);

	qyflag=0;

}

void qyup()
{
	HAL_TIM_PWM_Start(&htim23, TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE(&htim23,TIM_CHANNEL_1,70);
	HAL_GPIO_WritePin(dir_GPIO_Port, dir_Pin, GPIO_PIN_SET);

}

void qyright()
{
	HAL_GPIO_WritePin(high, high_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(low, low_Pin, GPIO_PIN_RESET);
//qyflag=1;

}

void qyleft()
{
	HAL_GPIO_WritePin(high, high_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(low, low_Pin, GPIO_PIN_RESET);
//qyflag=1;

}

void qy_off()
{
	HAL_TIM_PWM_Stop(&htim23, TIM_CHANNEL_1);
	HAL_GPIO_WritePin(front, front_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(high, high_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(low, low_Pin, GPIO_PIN_RESET);
//qyflag=0;

}

void qydown()
{
	HAL_TIM_PWM_Start(&htim23, TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE(&htim23,TIM_CHANNEL_1,70);
	HAL_GPIO_WritePin(dir_GPIO_Port, dir_Pin, GPIO_PIN_RESET);
//HAL_TIM_PWM_Start(&sc, one);
//	__HAL_TIM_SET_COMPARE(&sc,one,70);
	//HAL_GPIO_WritePin(SC_HIGH_GPIO_Port, SC_HIGH_Pin, GPIO_PIN_RESET);
}

void qyfront()	
{
	HAL_GPIO_WritePin(front, front_Pin, GPIO_PIN_SET);
	//HAL_GPIO_WritePin(FR_HIGH_GPIO_Port, FR_HIGH_Pin, GPIO_PIN_SET);
//qyflag=1;

}

void qyback()
{

	
qyflag=1;

}

void speed_low()
{
speed=600;
}

void speed_middle()
{
speed=800;
}

void speed_hight()	
{
speed=999;
}
	