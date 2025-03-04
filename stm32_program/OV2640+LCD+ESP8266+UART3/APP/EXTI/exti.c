#include "exti.h"
#include "led.h"
#include "SysTick.h"
#include "beep.h"

/**
 * IO口外部中断
 */
void KEYExti_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	
	//1.使能IO
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体变量	
	//时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=KEY0_PIN|KEY1_PIN|KEY2_PIN|KEY3_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;     //GPIO模式为普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;       //100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             //上拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);                   //初始化GPIOF6,7,8,9
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //普通输出模式
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//2.开启SYSCFC时钟，以便设置IO口与中断线之间的映射关系。
	//只要使用外部中断，就必须打开SYSCFG时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	//配置IO口与中断线之间的映射关系
	//配置IO口与中断线之间的映射关系
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF,EXTI_PinSource9);//将中断线9与GPIOF映射
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF,EXTI_PinSource8);//将中断线8与GPIOF映射
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF,EXTI_PinSource7);//将中断线7与GPIOF映射
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF,EXTI_PinSource6);//将中断线6与GPIOF映射
	
	//3.配置中断分组，使能中断
	NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn;    //中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2; //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  //中断通道使能
	
	//中断优先级分组初始化
	NVIC_Init(&NVIC_InitStructure);
	
	//4.初始化EXTI，设置中断触发条件
	EXTI_InitStructure.EXTI_Line=EXTI_Line9|EXTI_Line8|EXTI_Line7|EXTI_Line6;  //中断线标号，
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;      //中断模式，可选的值为中断和事件
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;  //触发方式：下降沿触发或上升沿触发
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}


//中断函数
void EXTI9_5_IRQHandler()
{
	if(EXTI_GetITStatus(EXTI_Line9) != RESET)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
	if(EXTI_GetITStatus(EXTI_Line8) != RESET)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		EXTI_ClearITPendingBit(EXTI_Line8);
	}
	if(EXTI_GetITStatus(EXTI_Line7) != RESET)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
	if(EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
}


