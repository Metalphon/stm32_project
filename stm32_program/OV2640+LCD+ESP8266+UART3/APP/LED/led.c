#include "led.h"

/**
 * Led����˸ʵ��
 *    3��Led�Ʒֱ�������PE3\PE4\PG9
 */
void LED_Init()
{
	//����ṹ�����
	GPIO_InitTypeDef GPIO_InitStructure;  
	
	//ʹ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE); 
	
	//GPIOģʽ����
	/**
	 * STM32ÿ��ͨ��I/O�˿ڣ���GPIOA��B...������4��32λ���üĴ�����2��32λ���ݼĴ���,
	 * 1��32λ��λ�Ĵ�����1��32λ�����Ĵ����Լ�2��32λ���ù���ѡ��Ĵ�����
	 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;  //���ó�ʼ����ЩIO�ڡ�
	/**
	 * MODERΪGPIO�˿�ģʽ���ƼĴ���������32λ��STM32F4�����9��IO��ÿ��IO����16��IO��
	 * ��ˣ�MODER32λ�Ĵ���ÿ��λ����һ��IO�����У�
	 *        00�����루��λ״̬��
	 *        01��ͨ�����ģʽ
	 *        10�����ù���ģʽ
	 *        11��ģ��ģʽ
	 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //��ͨ���ģʽ
	/**
	 * OSPEEDR�Ĵ��������ڿ���GPIOx���ٶȣ��üĴ������������ģʽ��
	 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	/**
	 * OTYPER�Ĵ������ڿ���GPIOx��������͡��üĴ������������ģʽ��
	 * ��16λ��������16λÿλ��Ӧһ��I/O�ڣ����У�
	 *     0�� ������죨��λ״̬��
	 *     1�� �����©
	 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������
	/**
	 * PUPDR�Ĵ������üĴ�������λ����һ��IO�ڣ�����������������
	 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //����
	
	
	//��ʼ��
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	//��ʼ��
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	//��ʼʱ���ö˿�Ϊ�ߵ�ƽ
	GPIO_SetBits(GPIOE,GPIO_Pin_3|GPIO_Pin_4);
	GPIO_SetBits(GPIOG,GPIO_Pin_9);
}


