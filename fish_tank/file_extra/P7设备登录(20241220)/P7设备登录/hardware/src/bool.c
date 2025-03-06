#include "bool.h"		
#include "Delay.h"
#include "stm32f10x.h"
extern int Bee,Pump,AUTO;
extern int ph,turbidity,level;

Pump_INFO pump_info = {0};
//Pump2_INFO pump2_INFO = {0};

void Pump_Init(void)        
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   //使能GPIOB的时钟
	GPIO_InitTypeDef    GPIO_InitStruct;                   //定义GPIO结构体变量
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;                   //配置PB15、12引脚
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;            //配置GPIO为通用推挽输出
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;           //配置GPIO速率
	GPIO_Init(GPIOA,&GPIO_InitStruct);                     //GPIO初始化函数
		Pump_Set(Pump_OFF);	
}

void Pump_Set(_Bool status)
{
	
	GPIO_WriteBit(GPIOA, GPIO_Pin_8, status == Pump_ON ? Bit_RESET : Bit_SET);		//如果status等于LED_ON，则返回Bit_SET，否则返回Bit_RESET
	
	pump_info.Pump_Status = status;
	

}
