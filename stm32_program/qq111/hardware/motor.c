#include "motor.h"
#include "stm32h7xx.h"                  // Device header
#include "usart.h"
#include <stdint.h>
#include "tim.h"
#include "ctrl.h"
//htim1  左
//htim4  右


//htim1  1
uint16_t speed ;
extern int percent;
extern int qyflag;


// 伪随机数生成函数（简单的线性同余方法）
uint32_t seed = 2067713761; // 种子

unsigned int rand1() {
    seed = (seed * 1103515245 + 12345) & 0x7FFFFFFF;
    return seed;
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

void motortest(){
		HAL_TIM_PWM_Start(&rf, rfc1);
		__HAL_TIM_SET_COMPARE(&rf,rfc1,speed);
			HAL_TIM_PWM_Stop(&rf, rfc2);
}

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
	HAL_TIM_PWM_Stop(&sc, scc1);
	HAL_GPIO_WritePin(SC_HIGH_GPIO_Port, SC_HIGH_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LE_LOW_GPIO_Port, LE_LOW_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LE_HIGH_GPIO_Port, LE_HIGH_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RI_LOW_GPIO_Port, RI_LOW_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RI_HIGH_GPIO_Port, RI_HIGH_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(FR_LOW_GPIO_Port, FR_LOW_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(FR_HIGH_GPIO_Port, FR_HIGH_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BA_LOW_GPIO_Port, BA_LOW_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BA_HIGH_GPIO_Port, BA_HIGH_Pin, GPIO_PIN_RESET);
	qyflag=0;

}

void qyup()
{
	HAL_TIM_PWM_Start(&sc, scc1);
	__HAL_TIM_SET_COMPARE(&sc,scc1,70);
	HAL_GPIO_WritePin(SC_HIGH_GPIO_Port, SC_HIGH_Pin, GPIO_PIN_SET);
}

void qyright()
{
	HAL_GPIO_WritePin(RI_HIGH_GPIO_Port, RI_HIGH_Pin, GPIO_PIN_SET);
qyflag=1;

}

void qyleft()
{
	HAL_GPIO_WritePin(LE_HIGH_GPIO_Port, LE_HIGH_Pin, GPIO_PIN_SET);
qyflag=1;

}

void qy_off()
{
		HAL_GPIO_WritePin(RI_HIGH_GPIO_Port, RI_HIGH_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LE_HIGH_GPIO_Port, LE_HIGH_Pin, GPIO_PIN_RESET);

qyflag=0;

}

void qydown()
{
HAL_TIM_PWM_Start(&sc, scc1);
	__HAL_TIM_SET_COMPARE(&sc,scc1,70);
	HAL_GPIO_WritePin(SC_HIGH_GPIO_Port, SC_HIGH_Pin, GPIO_PIN_RESET);
}

void qyfront()	
{

	HAL_GPIO_WritePin(FR_HIGH_GPIO_Port, FR_HIGH_Pin, GPIO_PIN_SET);
qyflag=1;

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
	
