#include "usart4.h"
#include "lcd.h"
#include "SysTick.h"
#include "stdio.h"




int fputc(int ch,FILE *p)  //����Ĭ�ϵģ���ʹ��printf����ʱ�Զ�����
{
	USART_SendData(UART4,(u8)ch);	
	while(USART_GetFlagStatus(UART4,USART_FLAG_TXE)==RESET);
	return ch;
}

/**
 * ���ڳ�ʼ������
 */
void LTE_uart4_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;  //GPIO���ýṹ��
	USART_InitTypeDef USART_InitStructure;//�������ýṹ��
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//1.����ʱ�Ӻ�GPIOʱ��ʹ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOBʱ�� 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE); //ʹ��USART4ʱ�� 

	//2.�������Ÿ�����ӳ��
 	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_UART4);  //GPIOA0����ΪUART4
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_UART4);  //GPIOA1����ΪUART4
	
	//3.GPIO�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //GPIOA0��GPIOA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;      //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //����
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//4.���ڲ�����ʼ���������ʵ�
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(UART4, &USART_InitStructure); //��ʼ������4
  USART_Cmd(UART4, ENABLE);  //ʹ�ܴ���4
	
	//����жϱ�־λ
	USART_ClearFlag(UART4, USART_FLAG_TC);
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE); //��������ж�
	
	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;      //����3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		   //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			   //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	  //����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}



// �������ݽ��ջ�������
u8  USART4_RX_BUF[USART4_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
u16 USART4_RX_STA = 0;         		    //����״̬��� 
u8 VO7670_flag;

void UART4_IRQHandler(void)
{	
	u8  i;
	LCD_Clear(YELLOW);
  LCD_DisplayString(30,80,24,"wyf");	
	//��Ϊ���ڵ��ж������кܶ࣬����ڽ����ж�֮��������Ҫ�жϴ˴��ж����������͡�
	//USART_GetITStatus(USART1,USART_IT_RXNE) ����������ȡ�ж�״̬��־λ���˺����������ж� USARTx ���ж����� USART_IT �Ƿ�����жϣ�
	if(USART_GetITStatus(UART4,USART_IT_RXNE)!=RESET)
	{
		USART_ClearITPendingBit(UART4,USART_IT_RXNE);           //����жϱ�־λ
		
		USART4_RX_BUF[USART4_RX_STA]=USART_ReceiveData(UART4);  //��ȡһλ���յ�������
    USART4_RX_STA++;
		
		//�ж��Ƿ��������������������������������
		if(USART4_RX_BUF[USART4_RX_STA-1] == 0x0a || USART4_RX_BUF[USART4_RX_STA-1] == '#'|| USART4_RX_STA == USART4_REC_LEN)   //������յ����з����߽��յ���������
		{
			  LCD_Clear(WHITE);
			  if(USART4_RX_BUF[USART4_RX_STA-1]=='#') // �ж�ǰһ�����յ����ַ��Ƿ�Ϊ�����ַ�
				{
					  USART4_RX_BUF[USART4_RX_STA-1]='\0';
					  if(USART4_RX_BUF[0]=='s'){
							VO7670_flag=1;
						}
				}
				LCD_DisplayString(30,80,24,USART4_RX_BUF);	
				delay_ms(2000);
				
				//�ȴ����ݷ������
	    	while(USART_GetFlagStatus(UART4,USART_FLAG_TC) != SET);   //��ȡ����״̬��־λ  USART_FLAG_TC������ɱ�־λ
				
			  //���ݷ������֮��������ݽ������飬һ������µ����ݡ�
			  for(i=0;i<USART4_RX_STA;i++)
				{
					USART4_RX_BUF[i]='\0';
				}
				USART4_RX_STA=0;
		}
	}

}

////�����ַ���
void Send_data_4(u8 *s)
{
	while(*s!='\0')
	{ 
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC )==RESET);	
		USART_SendData(UART4,*s);
		s++;  //ָ����һ���ֽڡ�
	}
}


