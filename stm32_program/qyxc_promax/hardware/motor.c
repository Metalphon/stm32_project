#include "motor.h"
#include "stm32h7xx.h"                  // Device header
#include "usart.h"
#include <stdint.h>
#include "tim.h"
//#include "ctrl.h"
#include "pid.h"

//htim1  左
//htim4  右


//htim1  1
uint16_t speed ;
int percent;
int qyflag;


// 伪随机数生成函数（简单的线性同余方法）
uint32_t seed = 2067713761; // 种子

unsigned int rand1() {
    seed = (seed * 1103515245 + 12345) & 0x7FFFFFFF;
    return seed;
}

void motor_init()
{ 
	//A0A1左前   A2A3左后
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);
  //C6C7右前   C8C9右后
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
	//F0F1主刷   F2F3副刷
	HAL_TIM_PWM_Start(&htim23,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim23,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim23,TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim23,TIM_CHANNEL_4);
	
	
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

void sportfront(uint16_t speed)
{
	R_target_val=speed;
  L_target_val=speed;
  sport_Flag=0;
}

void sportback(uint16_t speed)
{
	R_target_val=speed;
  L_target_val=speed;
  sport_Flag=1;
}

void sportright(uint16_t speed)
{
	R_target_val=speed;
  L_target_val=speed;
  sport_Flag=2;
}

void sportleft(uint16_t speed)
{
	R_target_val=speed;
  L_target_val=speed;
  sport_Flag=3;
}

void sportstop()
{
	R_target_val=0;
  L_target_val=0;
  sport_Flag=4;

}

void qyup()
{
TIM23->CCR1=1000; 

}

void qyright()
{
qyflag=1;
TIM23->CCR3=1000; 
TIM23->CCR4=0; 


}

void qyleft()
{
qyflag=1;
TIM23->CCR3=1000; 
TIM23->CCR4=0; 


}

void qy_off()
{
qyflag=0;
TIM23->CCR3=0; 
TIM23->CCR4=0; 
	
}

void qydown()
{
TIM23->CCR1=1000; 

}

void qyfront()	
{

qyflag=1;
pid.target_val=300;
}

void qyback()
{
//没有
	
qyflag=1;

}

void speed_low()
{
speed=100;
}

void speed_middle()
{
speed=200;
}

void speed_hight()	
{
speed=300;
}
	
