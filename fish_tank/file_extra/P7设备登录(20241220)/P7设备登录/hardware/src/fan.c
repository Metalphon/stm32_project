



////硬件驱动
//#include "fan.h"


//uint8_t Fan_Status=0;




//void Fan_Init(void)
//{

//	GPIO_InitTypeDef gpio_initstruct;
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//打开GPIOB的时钟
//	
//	gpio_initstruct.GPIO_Mode = GPIO_Mode_Out_PP;				//设置为输出
//	gpio_initstruct.GPIO_Pin = GPIO_Pin_8;						//将初始化的Pin脚
//	gpio_initstruct.GPIO_Speed = GPIO_Speed_50MHz;				//可承载的最大频率
//	
//	GPIO_Init(GPIOB, &gpio_initstruct);							//初始化GPIO
//	
//	Fan_Set(FAN_OFF);											//初始化完成后，关闭蜂鸣器

//}



//void Fan_Set(uint8_t status)
//{
//	
//	GPIO_WriteBit(GPIOB, GPIO_Pin_8, status == FAN_ON ? Bit_SET : Bit_RESET);		//如果status等于BEEP_ON，则返回Bit_SET，否则返回Bit_RESET
//	
//	Fan_Status = status;

//}
