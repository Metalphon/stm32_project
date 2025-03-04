#include "exti.h"
#include "led.h"
#include "SysTick.h"
#include "beep.h"

/**
 * IO���ⲿ�ж�
 */
void KEYExti_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef  EXTI_InitStructure;
	
	//1.ʹ��IO
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ�����	
	//ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=KEY0_PIN|KEY1_PIN|KEY2_PIN|KEY3_PIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;     //GPIOģʽΪ��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;       //100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;             //����
  GPIO_Init(GPIOF, &GPIO_InitStructure);                   //��ʼ��GPIOF6,7,8,9
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //��ͨ���ģʽ
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//2.����SYSCFCʱ�ӣ��Ա�����IO�����ж���֮���ӳ���ϵ��
	//ֻҪʹ���ⲿ�жϣ��ͱ����SYSCFGʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
	//����IO�����ж���֮���ӳ���ϵ
	//����IO�����ж���֮���ӳ���ϵ
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF,EXTI_PinSource9);//���ж���9��GPIOFӳ��
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF,EXTI_PinSource8);//���ж���8��GPIOFӳ��
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF,EXTI_PinSource7);//���ж���7��GPIOFӳ��
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF,EXTI_PinSource6);//���ж���6��GPIOFӳ��
	
	//3.�����жϷ��飬ʹ���ж�
	NVIC_InitStructure.NVIC_IRQChannel=EXTI9_5_IRQn;    //�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2; //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;  //�ж�ͨ��ʹ��
	
	//�ж����ȼ������ʼ��
	NVIC_Init(&NVIC_InitStructure);
	
	//4.��ʼ��EXTI�������жϴ�������
	EXTI_InitStructure.EXTI_Line=EXTI_Line9|EXTI_Line8|EXTI_Line7|EXTI_Line6;  //�ж��߱�ţ�
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;      //�ж�ģʽ����ѡ��ֵΪ�жϺ��¼�
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;  //������ʽ���½��ش����������ش���
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}


//�жϺ���
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


