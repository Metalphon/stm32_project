



////Ӳ������
//#include "fan.h"


//uint8_t Fan_Status=0;




//void Fan_Init(void)
//{

//	GPIO_InitTypeDef gpio_initstruct;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//��GPIOB��ʱ��
//	
//	gpio_initstruct.GPIO_Mode = GPIO_Mode_Out_PP;				//����Ϊ���
//	gpio_initstruct.GPIO_Pin = GPIO_Pin_8;						//����ʼ����Pin��
//	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;				//�ɳ��ص����Ƶ��
//	
//	GPIO_Init(GPIOB, &gpio_initstruct);							//��ʼ��GPIO
//	
//	Fan_Set(FAN_OFF);											//��ʼ����ɺ󣬹رշ�����

//}



//void Fan_Set(uint8_t status)
//{
//	
//	GPIO_WriteBit(GPIOB, GPIO_Pin_8, status == FAN_ON ? Bit_SET : Bit_RESET);		//���status����BEEP_ON���򷵻�Bit_SET�����򷵻�Bit_RESET
//	
//	Fan_Status = status;

//}
