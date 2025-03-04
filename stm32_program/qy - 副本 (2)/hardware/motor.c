#include "motor.h"
#include "stm32h7xx.h"                  // Device header
#include "usart.h"
#include <stdint.h>


uint16_t speed;
extern int percent;
extern int qyflag;


// 伪随机数生成函数（简单的线性同余方法）
uint32_t seed = 2067713761; // 种子

unsigned int rand1() {
    seed = (seed * 1103515245 + 12345) & 0x7FFFFFFF;
    return seed;
}


void sportfront(uint16_t speed)
{
		HAL_UART_Printf(USART_DEBUG, "%d\r\n",speed);

}

void sportback(uint16_t speed)
{
			HAL_UART_Printf(USART_DEBUG, "%d\r\n",speed);

}

void sportright(uint16_t speed)
{

}

void sportleft(uint16_t speed)
{

}

void sportstop()
{

}

void qyup()
{
	
}

void qyright()
{
qyflag=1;

}

void qyleft()
{
qyflag=1;

}

void qy_off()
{
qyflag=0;

}

void qydown()
{

}

void qyfront()	
{
qyflag=1;

}

void qyback()
{
qyflag=1;

}

void speed_low()
{
speed=30;
}

void speed_middle()
{
speed=60;
}

void speed_hight()	
{
speed=90;
}
	
