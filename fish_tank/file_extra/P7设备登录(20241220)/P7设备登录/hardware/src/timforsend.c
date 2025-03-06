#include "stm32f10x.h"
#include "timforsend.h"


void TIM2_Configuration(void)
{
	
	  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  // ʹ��TIM2ʱ��
    
    TIM_TimeBaseStructure.TIM_Period = 29999;  // ������30000
    TIM_TimeBaseStructure.TIM_Prescaler = 7199; // Ԥ��Ƶ��7200
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);  // ʹ�ܶ�ʱ��2�����ж�
    TIM_Cmd(TIM2, ENABLE);  // ������ʱ��2
}

void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void TIM2_Iint(void)
{
	TIM2_Configuration(); // ��ʼ����ʱ��2
  NVIC_Configuration(); // ��ʼ���ж�
}

extern unsigned short timeCount;	//���ͼ������


void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  // ���TIM2�����жϱ�־
			
			  timeCount=1;
			
    }
}
