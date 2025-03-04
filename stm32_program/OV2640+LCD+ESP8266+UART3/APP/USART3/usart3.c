#include "usart3.h"
#include "beep.h"
#include "SysTick.h"


u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u16 USART3_RX_STA=0;

u8 SEND_BUF[2]={'O','K'};
u8 SEND_BUF_UP[3]={'U','P','\0'};
u8 SEND_BUF_DOWN[5]={'D','O','W','N','\0'};


void LTE_uart3_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;  //GPIO配置结构体
	USART_InitTypeDef USART_InitStructure;//串口配置结构体
	NVIC_InitTypeDef NVIC_InitStructure;
	//1.串口时钟和GPIO时钟使能
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE); //使能GPIOB时钟 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); //使能USART3时钟 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);  
	//2.设置引脚复用器映射
 	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);  //GPIOB10复用为USART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3); //GPIOB11复用为USART3
	
	//3.GPIO端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOB10与GPIOB11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;      //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);          //初始化PB10，PB11
	
	//4.串口参数初始化：波特率等
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART3, &USART_InitStructure); //初始化串口3
  USART_Cmd(USART3, ENABLE);  //使能串口1 
	
	//清除中断标志位
	USART_ClearFlag(USART3, USART_FLAG_TC);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);         //开启相关中断
	
	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;      //串口3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		   //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			   //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	  //根据指定的参数初始化VIC寄存器、
	
	/****************************** 蓝牙状态IO口初始化**********************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;    //GPIOA7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;             //普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;       //100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;          
	GPIO_Init(GPIOF,&GPIO_InitStructure);          //初始化 
}


void USART3_IRQHandler(void)
{	
	u8  i;
	//因为串口的中断类型有很多，因此在进入中断之后，首先需要判断此次中断是那种类型。
	//USART_GetITStatus(USART1,USART_IT_RXNE) 函数用来读取中断状态标志位。此函数功能是判断 USARTx 的中断类型 USART_IT 是否产生中断，
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET)
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE); //清除中断标志位
		
		USART3_RX_BUF[USART3_RX_STA]=USART_ReceiveData(USART3);  //读取一位接收到的数据
    USART3_RX_STA++;
		
		//判断是否满足接收完成条件，否则继续接收数据
		if(USART3_RX_BUF[USART3_RX_STA-1] == 0x0a || USART3_RX_BUF[USART3_RX_STA-1] == '#'|| USART3_RX_STA == USART3_REC_LEN)   //如果接收到换行符或者接收的数据已满
		{
			  if(USART3_RX_BUF[USART3_RX_STA-1]=='#')
				{
            
						TIM_Cmd(TIM3,DISABLE);
					  Send_data_3(SEND_BUF_DOWN);
					  USART3_RX_STA=0;
				}
				//数据发送完成之后，清空数据接收数组，以便接收新的数据。
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
				//数据发送完成之后，清空数据接收数组，以便接收新的数据。
		    for(i=0;i<USART3_RX_STA;i++)
		    {
	      	USART3_RX_BUF[i]='\0';
			  }
				USART3_RX_STA=0;
		}
		
		//清除数据
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

////发送字符串
void Send_data_3(u8 *s)
{
	while(*s!='\0')
	{ 
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC )==RESET);	
		USART_SendData(USART3,*s);
		s++;  //指向下一个字节。
	}
}

