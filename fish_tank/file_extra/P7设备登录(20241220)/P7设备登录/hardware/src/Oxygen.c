//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//Ӳ������
#include "Oxygen.h"


LED2_INFO led2_info = {0};


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
void Led2_Init(void)
{

	GPIO_InitTypeDef gpio_initstruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//��GPIOA��ʱ��
	
	gpio_initstruct.GPIO_Mode = GPIO_Mode_Out_PP;				//����Ϊ���
	gpio_initstruct.GPIO_Pin = GPIO_Pin_11;						//����ʼ����Pin��
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;				//�ɳ��ص����Ƶ��
	
	GPIO_Init(GPIOA, &gpio_initstruct);							//��ʼ��GPIO
	
	Led2_Set(LED2_OFF);											//��ʼ����ɺ󣬹رշ�����

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
void Led2_Set(_Bool status)
{
	
	GPIO_WriteBit(GPIOA, GPIO_Pin_11, status == LED2_ON ? Bit_RESET : Bit_SET);		//���status����LED_ON���򷵻�Bit_SET�����򷵻�Bit_RESET
	
	led2_info.Led2_Status = status;

}
