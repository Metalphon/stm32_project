#ifndef _usart4_H
#define _usart4_H

#include "system.h"

#define USART4_REC_LEN		200  	//�����������ֽ��� 200
extern u8  USART4_RX_BUF[USART4_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART4_RX_STA;         		    //����״̬��� 

void LTE_uart4_init(u32 bound);
void Send_data_4(u8 *s);

#endif
