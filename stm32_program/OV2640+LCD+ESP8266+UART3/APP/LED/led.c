#include "led.h"

/**
 * Led灯闪烁实验
 *    3个Led灯分别链接在PE3\PE4\PG9
 */
void LED_Init()
{
	//定义结构体变量
	GPIO_InitTypeDef GPIO_InitStructure;  
	
	//使能时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE); 
	
	//GPIO模式配置
	/**
	 * STM32每组通用I/O端口（如GPIOA、B...）包括4个32位配置寄存器，2个32位数据寄存器,
	 * 1个32位复位寄存器，1个32位锁定寄存器以及2个32位复用功能选择寄存器。
	 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;  //设置初始化哪些IO口。
	/**
	 * MODER为GPIO端口模式控制寄存器，共有32位。STM32F4最多有9组IO，每组IO下有16个IO口
	 * 因此，MODER32位寄存器每两位控制一个IO。其中：
	 *        00：输入（复位状态）
	 *        01：通用输出模式
	 *        10：复用功能模式
	 *        11：模拟模式
	 */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //普通输出模式
	/**
	 * OSPEEDR寄存器：用于控制GPIOx的速度，该寄存器仅用于输出模式。
	 */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	/**
	 * OTYPER寄存器用于控制GPIOx的输出类型。该寄存器仅用于输出模式。
	 * 高16位保留，低16位每位对应一个I/O口，其中：
	 *     0： 输出推挽（复位状态）
	 *     1： 输出开漏
	 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽输出
	/**
	 * PUPDR寄存器：该寄存器两个位控制一个IO口，用于设置上下拉。
	 */
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   //上拉
	
	
	//初始化
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	//初始化
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	//开始时设置端口为高电平
	GPIO_SetBits(GPIOE,GPIO_Pin_3|GPIO_Pin_4);
	GPIO_SetBits(GPIOG,GPIO_Pin_9);
}


