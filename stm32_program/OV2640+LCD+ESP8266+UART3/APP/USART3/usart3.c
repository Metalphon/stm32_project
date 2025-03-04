#include "usart3.h"
#include "beep.h"
#include "SysTick.h"


u8 USART3_RX_BUF[USART3_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u16 USART3_RX_STA=0;

u8 SEND_BUF[2]={'O','K'};
u8 SEND_BUF_UP[3]={'U','P','\0'};
u8 SEND_BUF_DOWN[5]={'D','O','W','N','\0'};


void LTE_uart3_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;  //GPIO���ýṹ��
	USART_InitTypeDef USART_InitStructure;//�������ýṹ��
	NVIC_InitTypeDef NVIC_InitStructure;
	//1.����ʱ�Ӻ�GPIOʱ��ʹ��
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //ʹ��GPIOBʱ�� 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //ʹ��USART3ʱ�� 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);  
	//2.�������Ÿ�����ӳ��
 	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);  //GPIOB10����ΪUSART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11����ΪUSART3
	
	//3.GPIO�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOB10��GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;      //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);          //��ʼ��PB10��PB11
	
	//4.���ڲ�����ʼ���������ʵ�
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART3, &USART_InitStructure); //��ʼ������3
  USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���1 
	
	//����жϱ�־λ
	USART_ClearFlag(USART3, USART_FLAG_TC);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);         //��������ж�
	
	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;      //����3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		   //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			   //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	  //����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
	/****************************** ����״̬IO�ڳ�ʼ��**********************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;    //GPIOA7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;             //��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;       //100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;          
	GPIO_Init(GPIOF,&GPIO_InitStructure);          //��ʼ�� 
}


void USART3_IRQHandler(void)
{	
	u8  i;
	//��Ϊ���ڵ��ж������кܶ࣬����ڽ����ж�֮��������Ҫ�жϴ˴��ж����������͡�
	//USART_GetITStatus(USART1,USART_IT_RXNE) ����������ȡ�ж�״̬��־λ���˺����������ж� USARTx ���ж����� USART_IT �Ƿ�����жϣ�
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE); //����жϱ�־λ
		
		USART3_RX_BUF[USART3_RX_STA]=USART_ReceiveData(USART3);  //��ȡһλ���յ�������
    USART3_RX_STA++;
		
		//�ж��Ƿ��������������������������������
		if(USART3_RX_BUF[USART3_RX_STA-1] == 0x0a || USART3_RX_BUF[USART3_RX_STA-1] == '#'|| USART3_RX_STA == USART3_REC_LEN)   //������յ����з����߽��յ���������
		{
			  if(USART3_RX_BUF[USART3_RX_STA-1]=='#')
				{
            
						TIM_Cmd(TIM3,DISABLE);
					  Send_data_3(SEND_BUF_DOWN);
					  USART3_RX_STA=0;
				}
				//���ݷ������֮��������ݽ������飬�Ա�����µ����ݡ�
		    for(i=0;i<USART3_RX_STA;i++)
		    {
	      	USART3_RX_BUF[i]='\0';
			  }
				USART3_RX_STA=0;
		}
		else if(USART3_RX_BUF[USART3_RX_STA-1] == 0x0a ||USART3_RX_BUF[USART3_RX_STA-1] == '@'|| USART3_RX_STA == USART3_REC_LEN)
	  {
			  if(USART3_RX_BUF[USART3_RX_STA-1]=='@')
				{
					
					TIM_Cmd(TIM4,DISABLE);
					Send_data_3(SEND_BUF_UP);
					USART3_RX_STA=0;
				}
				//���ݷ������֮��������ݽ������飬�Ա�����µ����ݡ�
		    for(i=0;i<USART3_RX_STA;i++)
		    {
	      	USART3_RX_BUF[i]='\0';
			  }
				USART3_RX_STA=0;
		}
		
		//�������
		if(USART3_RX_STA>2)
		{
				for(i=0;i<USART3_RX_STA;i++)
		    {
	      	USART3_RX_BUF[i]='\0';
			  }
				USART3_RX_STA=0;
		}
		
	}

}

////�����ַ���
void Send_data_3(u8 *s)
{
	while(*s!='\0')
	{ 
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC )==RESET);	
		USART_SendData(USART3,*s);
		s++;  //ָ����һ���ֽڡ�
	}
}

