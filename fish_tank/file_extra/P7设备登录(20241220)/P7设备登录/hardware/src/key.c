
//��Ƭ��ͷ�ļ�
#include "stm32f10x.h"

//Ӳ������
#include "key.h"
#include "delay.h"
#include "led.h"
#include "heater.h"
#include "bool.h"		
#include "Oxygen.h"
#include "servo.h"
#include "beep.h"
#include "mode.h"
u8 state = 0;
//float Angle =60;
/*
************************************************************
*	�������ƣ�	Led_Init
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
void Key_Init(void)
{
    GPIO_InitTypeDef gpio_initstruct;
    EXTI_InitTypeDef exti_initstruct;
    NVIC_InitTypeDef nvic_initstruct;

    // ʹ��GPIOB��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    // ʹ�ܸ��ù���ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

		gpio_initstruct.GPIO_Mode = GPIO_Mode_IPU;
    gpio_initstruct.GPIO_Pin = GPIO_Pin_4;
    gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio_initstruct);
	
		gpio_initstruct.GPIO_Mode = GPIO_Mode_IPU;
    gpio_initstruct.GPIO_Pin = GPIO_Pin_3;
    gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio_initstruct);
    // ����PB10Ϊ��������ģʽ
    gpio_initstruct.GPIO_Mode = GPIO_Mode_IPU;
    gpio_initstruct.GPIO_Pin = GPIO_Pin_10;
    gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio_initstruct);

    // ����PB11Ϊ��������ģʽ
    gpio_initstruct.GPIO_Mode = GPIO_Mode_IPU;
    gpio_initstruct.GPIO_Pin = GPIO_Pin_11;
    gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio_initstruct);

    // ����PB12Ϊ��������ģʽ
    gpio_initstruct.GPIO_Mode = GPIO_Mode_IPU;
    gpio_initstruct.GPIO_Pin = GPIO_Pin_12;
    gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio_initstruct);

    // ����PB13Ϊ��������ģʽ
    gpio_initstruct.GPIO_Mode = GPIO_Mode_IPU;
    gpio_initstruct.GPIO_Pin = GPIO_Pin_13;
    gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio_initstruct);

   gpio_initstruct.GPIO_Mode = GPIO_Mode_IPU;
    gpio_initstruct.GPIO_Pin = GPIO_Pin_14;
    gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio_initstruct);
		
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource4);

    exti_initstruct.EXTI_Line = EXTI_Line4;
    exti_initstruct.EXTI_Mode = EXTI_Mode_Interrupt;
    exti_initstruct.EXTI_Trigger = EXTI_Trigger_Falling;
    exti_initstruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti_initstruct);

GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource3);

    exti_initstruct.EXTI_Line = EXTI_Line3;
    exti_initstruct.EXTI_Mode = EXTI_Mode_Interrupt;
    exti_initstruct.EXTI_Trigger = EXTI_Trigger_Falling;
    exti_initstruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti_initstruct);
    // ����PB10���ⲿ�ж���
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10);

    exti_initstruct.EXTI_Line = EXTI_Line10;
    exti_initstruct.EXTI_Mode = EXTI_Mode_Interrupt;
    exti_initstruct.EXTI_Trigger = EXTI_Trigger_Falling;
    exti_initstruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti_initstruct);

    // ����PB11���ⲿ�ж���
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);

    exti_initstruct.EXTI_Line = EXTI_Line11;
    exti_initstruct.EXTI_Mode = EXTI_Mode_Interrupt;
    exti_initstruct.EXTI_Trigger = EXTI_Trigger_Falling;
    exti_initstruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti_initstruct);

    // ����PB12���ⲿ�ж���
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);

    exti_initstruct.EXTI_Line = EXTI_Line12;
    exti_initstruct.EXTI_Mode = EXTI_Mode_Interrupt;
    exti_initstruct.EXTI_Trigger = EXTI_Trigger_Falling;
    exti_initstruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti_initstruct);

    // ����PB13���ⲿ�ж���
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);

    exti_initstruct.EXTI_Line = EXTI_Line13;
    exti_initstruct.EXTI_Mode = EXTI_Mode_Interrupt;
    exti_initstruct.EXTI_Trigger = EXTI_Trigger_Falling;
    exti_initstruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&exti_initstruct);
		


	nvic_initstruct.NVIC_IRQChannel = EXTI4_IRQn ;
    nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 2;
    nvic_initstruct.NVIC_IRQChannelSubPriority = 2;
    nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_initstruct);
		
		nvic_initstruct.NVIC_IRQChannel = EXTI3_IRQn ;
    nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 2;
    nvic_initstruct.NVIC_IRQChannelSubPriority = 2;
    nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_initstruct);

    // ����PB10���ж����ȼ���ʹ���ж�
    nvic_initstruct.NVIC_IRQChannel = EXTI15_10_IRQn;
    nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 2;
    nvic_initstruct.NVIC_IRQChannelSubPriority = 2;
    nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_initstruct);

    // ����PB11���ж����ȼ���ʹ���ж�
    nvic_initstruct.NVIC_IRQChannel = EXTI15_10_IRQn;
    nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 2;
    nvic_initstruct.NVIC_IRQChannelSubPriority = 2;
    nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_initstruct);

    // ����PB12���ж����ȼ���ʹ���ж�
    nvic_initstruct.NVIC_IRQChannel = EXTI15_10_IRQn;
    nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 2;
    nvic_initstruct.NVIC_IRQChannelSubPriority = 2;
    nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_initstruct);

    // ����PB13���ж����ȼ���ʹ���ж�
    nvic_initstruct.NVIC_IRQChannel = EXTI15_10_IRQn;
    nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 2;
    nvic_initstruct.NVIC_IRQChannelSubPriority = 2;
    nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic_initstruct);
		
//    nvic_initstruct.NVIC_IRQChannel = EXTI15_10_IRQn;
//    nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 2;
//    nvic_initstruct.NVIC_IRQChannelSubPriority = 2;
//    nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&nvic_initstruct);
}

// PB10���ⲿ�жϷ�����
void EXTI15_10_IRQHandler(void)
{
    DelayXms(10);
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) == 0)
    {
        // ����PB10�İ����������PA8
         	if (led1_info.Led1_Status == LED1_ON)
            Led1_Set(LED1_OFF);
        else
            Led1_Set(LED1_ON);
//				DelayMs(3000);
//            Led1_Set(LED1_OFF);
    }
    EXTI_ClearITPendingBit(EXTI_Line10);
		 
		DelayXms(10);
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)
    {
        // ����PB11�İ����������PA9
			if (led_info.Led_Status == LED_ON)
            Led_Set(LED_OFF);
			
        else
            Led_Set(LED_ON);
    
    }
    EXTI_ClearITPendingBit(EXTI_Line11);
		
		 DelayXms(10);
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 0)
    {
        // ����PB12�İ����������PA10
         	if (led2_info.Led2_Status == LED2_ON)
            Led2_Set(LED2_OFF);
        else
            Led2_Set(LED2_ON);
    }
    EXTI_ClearITPendingBit(EXTI_Line12);
		
		DelayXms(10);
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0)
    {

			if(pump_info.Pump_Status==Pump_ON) Pump_Set(Pump_OFF);
			
					else Pump_Set(Pump_ON);
    }
    EXTI_ClearITPendingBit(EXTI_Line13);

}
void EXTI3_IRQHandler(void)
{
		DelayXms(10);
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == 0)
    {
        // ����PB13�İ����������PA11
			if(beep_info.Beep_Status==BEEP_ON) Beep_Set(BEEP_OFF);
        else
           Beep_Set(BEEP_ON);
	
    }
    EXTI_ClearITPendingBit(EXTI_Line3);
}
void EXTI4_IRQHandler(void)
{
		DelayXms(10);
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0)
    {
        state = !state;
					if (led3_info.Led3_Status == LED3_ON)
            Led3_Set(LED3_OFF);
        else
            Led3_Set(LED3_ON);
    }
    EXTI_ClearITPendingBit(EXTI_Line4);
}
uint8_t Next_page(void){
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0)
    {
        DelayXms(60);
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0);
        DelayXms(60);
		return 1;
    }
	else return 0;	
}

//void EXTI9_5_IRQHandler(void)
//{
//		DelayXms(10);
//	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)==0)
//	{
//		if(Beep_Status==BEEP_ON) Beep_Set(BEEP_OFF);
//		else Beep_Set(BEEP_ON);
//			}
//	EXTI_ClearITPendingBit(EXTI_Line8);
//		
//}

// PA0���ⲿ�жϷ�����
//void EXTI0_IRQHandler(void)
//{
//	uint16_t i;
//    DelayXms(10);
//    if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)
//    {
//          if (pwm_info.PWM_Status == PWM_ON)
//						 Angle = 90;
//            //Servo_Set(PWM_OFF);
//        else
//            Angle += 90;
//			if (Angle > 90){
//				Angle = 0;				//�Ƕȱ�������
//			}
//				Servo_SetAngle1(Angle);	
//    }
//    EXTI_ClearITPendingBit(EXTI_Line0);
//}

//// PB12���ⲿ�жϷ�����
//void EXTI15_10_IRQHandler(void)
//{
//    DelayXms(10);
//    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 0)
//    {
//        // ����PB12�İ����������PA10
//        if (led_info.Led_Status == LED_ON)
//            Led_Set(LED_OFF);
//        else
//            Led_Set(LED_ON);
//    }
//    EXTI_ClearITPendingBit(EXTI_Line12);
//}

//// PB13���ⲿ�жϷ�����
//void EXTI15_10_IRQHandler(void)
//{
//    DelayXms(10);
//    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0)
//    {
//        // ����PB13�İ����������PA11
//        if (led_info.Led_Status == LED_ON)
//            Led_Set(LED_OFF);
//        else
//            Led_Set(LED_ON);
//    }
//    EXTI_ClearITPendingBit(EXTI_Line13);
//}
