//单片机头文件
#include "stm32f10x.h"

//硬件驱动
#include "mode.h"


LED3_INFO led3_info = {0};


/*
************************************************************
*	函数名称：	Led_Init
*
*	函数功能：	灯泡初始化
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		
************************************************************
*/
void Led3_Init(void)
{

	GPIO_InitTypeDef gpio_initstruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//打开GPIOA的时钟
	
	gpio_initstruct.GPIO_Mode = GPIO_Mode_Out_PP;				//设置为输出
	gpio_initstruct.GPIO_Pin = GPIO_Pin_0;						//将初始化的Pin脚
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;				//可承载的最大频率
	
	GPIO_Init(GPIOA, &gpio_initstruct);							//初始化GPIO
	
	Led3_Set(LED3_OFF);											//初始化完成后，关闭蜂鸣器

}

/*
************************************************************
*	函数名称：	Led_Set
*
*	函数功能：	灯泡控制
*
*	入口参数：	status：开关蜂鸣器
*
*	返回参数：	无
*
*	说明：		开-LED_ON		关-LED_OFF
************************************************************
*/
void Led3_Set(_Bool status)
{
	
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, status == LED3_ON ? Bit_RESET : Bit_SET);		//如果status等于LED_ON，则返回Bit_SET，否则返回Bit_RESET
	
	led3_info.Led3_Status = status;

}
