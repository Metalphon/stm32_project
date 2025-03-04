#include "usart4.h"
#include "lcd.h"
#include "SysTick.h"
#include "stdio.h"




int fputc(int ch,FILE *p)  //函数默认的，在使用printf函数时自动调用
{
	USART_SendData(UART4,(u8)ch);	
	while(USART_GetFlagStatus(UART4,USART_FLAG_TXE)==RESET);
	return ch;
}

/**
 * 串口初始化函数
 */
void LTE_uart4_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;  //GPIO配置结构体
	USART_InitTypeDef USART_InitStructure;//串口配置结构体
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//1.串口时钟和GPIO时钟使能
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOB时钟 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE); //使能USART4时钟 

	//2.设置引脚复用器映射
 	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_UART4);  //GPIOA0复用为UART4
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_UART4);  //GPIOA1复用为UART4
	
	//3.GPIO端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //GPIOA0与GPIOA1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;      //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//4.串口参数初始化：波特率等
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(UART4, &USART_InitStructure); //初始化串口4
  USART_Cmd(UART4, ENABLE);  //使能串口4
	
	//清除中断标志位
	USART_ClearFlag(UART4, USART_FLAG_TC);
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE); //开启相关中断
	
	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;      //串口3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		   //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			   //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	  //根据指定的参数初始化VIC寄存器、
}



// 定义数据接收缓冲区域
u8  USART4_RX_BUF[USART4_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
u16 USART4_RX_STA = 0;         		    //接收状态标记 
u8 VO7670_flag;

void UART4_IRQHandler(void)
{	
	u8  i;
	LCD_Clear(YELLOW);
  LCD_DisplayString(30,80,24,"wyf");	
	//因为串口的中断类型有很多，因此在进入中断之后，首先需要判断此次中断是那种类型。
	//USART_GetITStatus(USART1,USART_IT_RXNE) 函数用来读取中断状态标志位。此函数功能是判断 USARTx 的中断类型 USART_IT 是否产生中断，
	if(USART_GetITStatus(UART4,USART_IT_RXNE)!=RESET)
	{
		USART_ClearITPendingBit(UART4,USART_IT_RXNE);           //清除中断标志位
		
		USART4_RX_BUF[USART4_RX_STA]=USART_ReceiveData(UART4);  //读取一位接收到的数据
    USART4_RX_STA++;
		
		//判断是否满足接收完成条件，否则继续接收数据
		if(USART4_RX_BUF[USART4_RX_STA-1] == 0x0a || USART4_RX_BUF[USART4_RX_STA-1] == '#'|| USART4_RX_STA == USART4_REC_LEN)   //如果接收到换行符或者接收的数据已满
		{
			  LCD_Clear(WHITE);
			  if(USART4_RX_BUF[USART4_RX_STA-1]=='#') // 判断前一个接收到的字符是否为特殊字符
				{
					  USART4_RX_BUF[USART4_RX_STA-1]='\0';
					  if(USART4_RX_BUF[0]=='s'){
							VO7670_flag=1;
						}
				}
				LCD_DisplayString(30,80,24,USART4_RX_BUF);	
				delay_ms(2000);
				
				//等待数据发送完成
	    	while(USART_GetFlagStatus(UART4,USART_FLAG_TC) != SET);   //读取串口状态标志位  USART_FLAG_TC发送完成标志位
				
			  //数据发送完成之后，清空数据接收数组，一遍接收新的数据。
			  for(i=0;i<USART4_RX_STA;i++)
				{
					USART4_RX_BUF[i]='\0';
				}
				USART4_RX_STA=0;
		}
	}

}

////发送字符串
void Send_data_4(u8 *s)
{
	while(*s!='\0')
	{ 
		while(USART_GetFlagStatus(UART4,USART_FLAG_TC )==RESET);	
		USART_SendData(UART4,*s);
		s++;  //指向下一个字节。
	}
}


