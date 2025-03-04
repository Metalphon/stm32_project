#include "l298n.h"

void L298N_Init(void)
{
	//定义结构体变量
	GPIO_InitTypeDef GPIO_InitStructure;  
	
	//使能时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
	//配置四个配置寄存器
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; //普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉模式
	
	//初始化
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOG,GPIO_Pin_4);
	GPIO_SetBits(GPIOE,GPIO_Pin_5);
}


