#ifndef _usart3_H
#define _usart3_H

#include "system.h"

#define LTE_connect PFin(13)     //LTE连接标志  为1与手机APP或者其他设备连接成功

#define USART3_REC_LEN	200  	//定义最大接收字节数 200

extern u8  USART3_RX_BUF[USART3_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART3_RX_STA;         		//接收状态标记 

void LTE_uart3_init(u32 bound);
void Send_data_3(u8 *s);

#endif


