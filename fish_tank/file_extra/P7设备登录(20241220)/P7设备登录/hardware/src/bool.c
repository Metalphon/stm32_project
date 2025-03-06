#include "bool.h"		
#include "Delay.h"
#include "stm32f10x.h"
extern int Bee,Pump,AUTO;
extern int ph,turbidity,level;

Pump_INFO pump_info = {0};
//Pump2_INFO pump2_INFO = {0};

void Pump_Init(void)        
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //ʹ��GPIOB��ʱ��
	GPIO_InitTypeDef    GPIO_InitStruct;                   //����GPIO�ṹ�����
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;                   //����PB15��12����
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;            //����GPIOΪͨ���������
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;           //����GPIO����
	GPIO_Init(GPIOA,&GPIO_InitStruct);                     //GPIO��ʼ������
		Pump_Set(Pump_OFF);	
}

void Pump_Set(_Bool status)
{
	
	GPIO_WriteBit(GPIOA, GPIO_Pin_8, status == Pump_ON ? Bit_RESET : Bit_SET);		//���status����LED_ON���򷵻�Bit_SET�����򷵻�Bit_RESET
	
	pump_info.Pump_Status = status;
	

}
