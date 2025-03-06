



//Ӳ������
#include "beep.h"

//uint8_t Beep_Status;


Beep_INFO beep_info = {0};
/*
************************************************************
*	�������ƣ�	Beep_Init
*
*	�������ܣ�	��������ʼ��
*
*	��ڲ�����	��
*
*	���ز�����	��
*
*	˵����		
************************************************************
*/
void Beep_Init(void)
{

	GPIO_InitTypeDef gpio_initstruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);		//��GPIOB��ʱ��
	
	gpio_initstruct.GPIO_Mode = GPIO_Mode_Out_PP;				//����Ϊ���
	gpio_initstruct.GPIO_Pin = GPIO_Pin_1;						//����ʼ����Pin��
	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;				//�ɳ��ص����Ƶ��
	
	GPIO_Init(GPIOC, &gpio_initstruct);							//��ʼ��GPIO
	
	Beep_Set(BEEP_OFF);											//��ʼ����ɺ󣬹رշ�����

}

/*
************************************************************
*	�������ƣ�	Beep_Set
*
*	�������ܣ�	����������
*
*	��ڲ�����	status�����ط�����
*
*	���ز�����	��
*
*	˵����		��-BEEP_ON		��-BEEP_OFF
************************************************************
*/
void Beep_Set(_Bool status)
{
	
	GPIO_WriteBit(GPIOC, GPIO_Pin_14, status == BEEP_ON ? Bit_RESET : Bit_SET);		//���status����BEEP_ON���򷵻�Bit_SET�����򷵻�Bit_RESET
	
	beep_info.Beep_Status = status;

}
