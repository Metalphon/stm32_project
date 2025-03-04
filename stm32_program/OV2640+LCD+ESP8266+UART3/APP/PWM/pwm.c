#include "pwm.h"

void TIM4_CH1_PWM_Init(u16 arr,u16 psc)
{
	   //�˲������ֶ��޸�IO������
   GPIO_InitTypeDef GPIO_InitStructure;
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   TIM_OCInitTypeDef  TIM_OCInitStructure;

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	//TIM14ʱ��ʹ��    
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//ʹ��PORTFʱ��	

   GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_TIM4); //GPIOF9����Ϊ��ʱ��14

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;           //GPIOF9
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
   GPIO_Init(GPIOB,&GPIO_InitStructure);              //��ʼ��PF9

   TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
   TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
   TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
   TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
   TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

   TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//��ʼ����ʱ��14

   //��ʼ��TIM14 Channel1 PWMģʽ	 
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
   TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
   TIM_OCInitStructure.TIM_Pulse = (arr + 1)/ 2 -1; 
   TIM_OC1Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1
   TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM14��CCR1�ϵ�Ԥװ�ؼĴ���
  
   TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPEʹ�� 

   TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM14
}


/**
 * ��ʱ��TIM3
 */
void TIM3_PWM_Init(u32 arr,u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	//ʱ��ʹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);    //ʹ�ܶ�ʱ��3��ʱ��
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 	//ʹ��GPIO��
	//��IO���Ÿ���ΪTIM3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3);  
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM3);  
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM3);  
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM3);  
  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;  //PC9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;          //����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;         
	GPIO_Init(GPIOC,&GPIO_InitStructure);               
  
	//��ʱ����������
	TIM_TimeBaseStructure.TIM_Prescaler=psc;   
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;  
	TIM_TimeBaseStructure.TIM_Period=arr;    
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure); 
  
	//PWMģʽ����
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;   //����ģʽ��PWM��������Ƚ�
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  //���ñȽ����ʹ�ܣ�Ҳ����ʹ��PWM������˿�
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;   //���ü����Ǹ߻��ǵ�
	
	//��ʼ��TIM3 Channel1 PWMģʽ
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);   
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);   //ʹ��Ԥװ�ؼĴ���
  
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
