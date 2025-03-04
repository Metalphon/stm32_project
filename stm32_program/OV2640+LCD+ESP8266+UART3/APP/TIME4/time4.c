#include "time4.h"
#include "SysTick.h"
#include "beep.h"

/**
 * per:定时器数值
 * psc:分频系数
 */
void TIM4_Init(u16 per,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//1.使能定时器时钟 TIM4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);   //使能
	
	//2.初始化定时器参数，包含自动重装载、分频系数、计数方式等
	TIM_TimeBaseInitStructure.TIM_Period=per;    //自动装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //分频系数
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;  //时钟分频因子
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //设置向上计数模式
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	//3.设置定时器中断类型
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);  //开启定时器中断
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	
	//4.设置定时器中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//定时器中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	
	
	//5.开启定时器
	TIM_Cmd(TIM4,ENABLE); //使能定时器	
}

/*******************************************************************************
* 函 数 名         : TIM4_IRQHandler
* 函数功能		   : TIM4中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update))
	{
    GPIO_SetBits(GPIOG,GPIO_Pin_7);//复位F9 点亮D1
		delay_ms(500);  //精确延时500ms
		GPIO_ResetBits(GPIOG,GPIO_Pin_7);
		delay_ms(500);  //精确延时500ms
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);	
}

