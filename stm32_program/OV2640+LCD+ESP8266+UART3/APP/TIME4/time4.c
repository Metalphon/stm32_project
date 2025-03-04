#include "time4.h"
#include "SysTick.h"
#include "beep.h"

/**
 * per:��ʱ����ֵ
 * psc:��Ƶϵ��
 */
void TIM4_Init(u16 per,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//1.ʹ�ܶ�ʱ��ʱ�� TIM4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);   //ʹ��
	
	//2.��ʼ����ʱ�������������Զ���װ�ء���Ƶϵ����������ʽ��
	TIM_TimeBaseInitStructure.TIM_Period=per;    //�Զ�װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; //��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;  //ʱ�ӷ�Ƶ����
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //�������ϼ���ģʽ
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	//3.���ö�ʱ���ж�����
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);  //������ʱ���ж�
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	
	//4.���ö�ʱ���ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;//��ʱ���ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	
	
	//5.������ʱ��
	TIM_Cmd(TIM4,ENABLE); //ʹ�ܶ�ʱ��	
}

/*******************************************************************************
* �� �� ��         : TIM4_IRQHandler
* ��������		   : TIM4�жϺ���
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update))
	{
    GPIO_SetBits(GPIOG,GPIO_Pin_7);//��λF9 ����D1
		delay_ms(500);  //��ȷ��ʱ500ms
		GPIO_ResetBits(GPIOG,GPIO_Pin_7);
		delay_ms(500);  //��ȷ��ʱ500ms
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);	
}

