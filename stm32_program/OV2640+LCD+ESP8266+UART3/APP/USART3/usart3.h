#ifndef _usart3_H
#define _usart3_H

#include "system.h"

#define LTE_connect PFin(13)     //LTE���ӱ�־  Ϊ1���ֻ�APP���������豸���ӳɹ�

#define USART3_REC_LEN	200  	//�����������ֽ��� 200

extern u8  USART3_RX_BUF[USART3_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART3_RX_STA;         		//����״̬��� 

void LTE_uart3_init(u32 bound);
void Send_data_3(u8 *s);

#endif


