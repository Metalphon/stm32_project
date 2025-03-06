//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//Ӳ������
#include "heater.h"


LED1_INFO led1_info = {0};


/*
************************************************************
*	�������ƣ�	Led_Init
*
*	�������ܣ�	���ݳ�ʼ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void Led1_Init(void)
{

	GPIO_InitTypeDef gpio_initstruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//��GPIOA��ʱ��
	
	gpio_initstruct.GPIO_Mode = GPIO_Mode_Out_PP;				//����Ϊ���
	gpio_initstruct.GPIO_Pin = GPIO_Pin_12;						//����ʼ����Pin��
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;				//�ɳ��ص����Ƶ��
	
	GPIO_Init(GPIOA, &gpio_initstruct);							//��ʼ��GPIO
	
	Led1_Set(LED1_OFF);											//��ʼ����ɺ󣬹رշ�����

}

/*
************************************************************
*	�������ƣ�	Led_Set
*
*	�������ܣ�	���ݿ���
*
*	��ڲ�����	status�����ط�����
*
*	���ز�����	��
*
*	˵����		��-LED_ON		��-LED_OFF
************************************************************
*/
void Led1_Set(_Bool status)
{
	
	GPIO_WriteBit(GPIOA, GPIO_Pin_12, status == LED1_ON ? Bit_RESET : Bit_SET);		//���status����LED_ON���򷵻�Bit_SET�����򷵻�Bit_RESET
	
	led1_info.Led1_Status = status;

}
