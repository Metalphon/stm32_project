#include "pwm.h"

void TIM4_CH1_PWM_Init(u16 arr,u16 psc)
{
	   //此部分需手动修改IO口设置
   GPIO_InitTypeDef GPIO_InitStructure;
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   TIM_OCInitTypeDef  TIM_OCInitStructure;

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	//TIM14时钟使能    
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTF时钟	

   GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4); //GPIOF9复用为定时器14

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIOF9
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
   GPIO_Init(GPIOB,&GPIO_InitStructure);              //初始化PF9

   TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
   TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
   TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
   TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
   TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

   TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//初始化定时器14

   //初始化TIM14 Channel1 PWM模式	 
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
   TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
   TIM_OCInitStructure.TIM_Pulse = (arr + 1)/ 2 -1; 
   TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1 4OC1
   TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM14在CCR1上的预装载寄存器
  
   TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPE使能 

   TIM_Cmd(TIM4, ENABLE);  //使能TIM14
}


/**
 * 定时器TIM3
 */
void TIM3_PWM_Init(u32 arr,u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	//时钟使能
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);    //使能定时器3的时钟
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//使能GPIO口
	//将IO引脚复用为TIM3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3);  
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM3);  
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM3);  
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM3);  
  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;  //PC9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;          //复用模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;         
	GPIO_Init(GPIOC,&GPIO_InitStructure);               
  
	//定时器参数配置
	TIM_TimeBaseStructure.TIM_Prescaler=psc;   
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  
	TIM_TimeBaseStructure.TIM_Period=arr;    
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure); 
  
	//PWM模式配置
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;   //设置模式是PWM还是输出比较
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //设置比较输出使能，也就是使能PWM输出到端口
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;   //设置极性是高还是低
	
	//初始化TIM3 Channel1 PWM模式
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);   
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);   //使能预装载寄存器
  
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);   
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);   

	 
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);   
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);   

	TIM_OC4Init(TIM3, &TIM_OCInitStructure);   
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);   
	
  TIM_ARRPreloadConfig(TIM3,ENABLE); 
	//TIM_Cmd(TIM3, ENABLE);   
}

//TIM2 PWM????? 
//PWM?????
//arr:?????
//psc:??????
void TIM2_PWM_Init(u32 arr,u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	//TIM2????  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//??PORTA??
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM2); //GPIOA0??????2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;           //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //??
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//??100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //??????
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //??
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //???PA0
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //?????
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //??????
	TIM_TimeBaseStructure.TIM_Period=arr;   //??????
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//??????2
	
	//???TIM2 Channel PWM?? 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //???????:TIM????????1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //??????
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //????:TIM???????
	TIM_OCInitStructure.TIM_Pulse = 0;//?????
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //??T??????????TIM2 OC1

	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //??TIM2?CCR1????????
 
  TIM_ARRPreloadConfig(TIM2,ENABLE);//ARPE??
	
	//TIM_Cmd(TIM2, ENABLE);  //??TIM2										  
}  
