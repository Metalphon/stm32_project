#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "bump.h"


/**
  * 函    数：LED初始化
  * 参    数：无
  * 返 回 值：无
  */
void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA, GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_10);
}


void LED1_fanzhuan()
{
	static u8 k = 0;
	if(k){
		GPIO_SetBits(GPIOA, GPIO_Pin_11);
		k = !k;
	}
	else{
		GPIO_ResetBits(GPIOA, GPIO_Pin_11);
		k = !k;
	}
}


void LED2_fanzhuan()
{
	static u8 i = 0;
	if(i){
		GPIO_SetBits(GPIOA, GPIO_Pin_12);
		i = !i;
	}
	else{
		GPIO_ResetBits(GPIOA, GPIO_Pin_12);
		i = !i;
	}
}

void LED3_fanzhuan()
{
	static u8 j = 0;
	if(j){
		GPIO_SetBits(GPIOA, GPIO_Pin_8);
		j = !j;
	}
	else{
		GPIO_ResetBits(GPIOA, GPIO_Pin_8);
		j = !j;
	}
}

