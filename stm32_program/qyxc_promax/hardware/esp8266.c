//单片机头文件
#include "stm32h7xx.h"                  // Device header

//网络设备驱动
#include "esp8266.h"

//硬件驱动

#include "main.h"
#include "usart.h"
//#include "ctrl.h"
//C库 
#include <string.h>
#include <stdio.h>


#define ESP8266_WIFI_INFO		"AT+CWJAP=\"yzx\",\"66666666\"\r\n"
#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"

unsigned char esp8266_buf[512];
unsigned short esp8266_cnt = 0, esp8266_cntPre = 0;
int i=1;
int b=1;
unsigned char esp8266_data[2];
uint8_t A_data[1];
uint8_t B_data[1];
uint8_t C_data[1];
uint8_t data[2]; // 创建一个长度为1的缓冲区
//==========================================================
//	函数名称：	ESP8266_Clear
//
//	函数功能：	清空缓存
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP8266_Clear(void)
{

	memset(esp8266_buf, 0, sizeof(esp8266_buf));
	esp8266_cnt = 0;

}

//==========================================================
//	函数名称：	ESP8266_WaitRecive
//
//	函数功能：	等待接收完成
//
//	入口参数：	无
//
//	返回参数：	REV_OK-接收完成		REV_WAIT-接收超时未完成
//
//	说明：		循环调用检测是否接收完成
//==========================================================
_Bool ESP8266_WaitRecive(void)
{

	if(esp8266_cnt == 0) 							//如果接收计数为0 则说明没有处于接收数据中，所以直接跳出，结束函数
		return REV_WAIT;
		
	if(esp8266_cnt == esp8266_cntPre)				//如果上一次的值和这次相同，则说明接收完毕
	{
		esp8266_cnt = 0;							//清0接收计数
			
		return REV_OK;								//返回接收完成标志
	}
		
	esp8266_cntPre = esp8266_cnt;					//置为相同
	
	return REV_WAIT;								//返回接收未完成标志
//if(i==0)
//{
//	i=1;
//return REV_OK;
//}
//	
//	return REV_WAIT;
}


//==========================================================
//	函数名称：	ESP8266_SendCmd
//
//	函数功能：	发送命令
//
//	入口参数：	cmd：命令
//				res：需要检查的返回指令
//
//	返回参数：	0-成功	1-失败
//
//	说明：		
//==========================================================
_Bool ESP8266_SendCmd(char *cmd, char *res)
{
	
	unsigned char timeOut = 200;
  b=0;
	Usart_SendString(&huart9, (unsigned char *)cmd, strlen((const char *)cmd));
//	HAL_UART_Printf(USART_DEBUG, "%d",i);
	
	while(timeOut--)
	{
		if(ESP8266_WaitRecive() == REV_OK)							//如果收到数据
		{
			if(strstr((const char *)esp8266_buf, res) != NULL)		//如果检索到关键词
			{
				ESP8266_Clear();									//清空缓存
				
				return 0;
			}
		}
		
		HAL_Delay(10);
	}
	
	return 1;

}

//==========================================================
//	函数名称：	ESP8266_SendData
//
//	函数功能：	发送数据
//
//	入口参数：	data：数据
//				len：长度
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP8266_SendData(unsigned char *data, unsigned short len)
{

	char cmdBuf[32];
	
	ESP8266_Clear();								//清空接收缓存
	sprintf(cmdBuf, "AT+CIPSEND=%d\r\n", len);		//发送命令
	if(!ESP8266_SendCmd(cmdBuf, ">"))				//收到‘>’时可以发送数据
	{
		Usart_SendString(&huart9, data, len);
	}

}

//==========================================================
//	函数名称：	ESP8266_GetIPD
//
//	函数功能：	获取平台返回的数据
//
//	入口参数：	等待的时间(乘以10ms)
//
//	返回参数：	平台返回的原始数据
//
//	说明：		不同网络设备返回的格式不同，需要去调试
//				如ESP8266的返回格式为	"+IPD,x:yyy"	x代表数据长度，yyy是数据内容
//==========================================================
unsigned char *ESP8266_GetIPD(unsigned short timeOut)
{

	char *ptrIPD = NULL;
	
	do
	{
		if(ESP8266_WaitRecive() == REV_OK)								//如果接收完成
		{
			ptrIPD = strstr((char *)esp8266_buf, "IPD,");				//搜索“IPD”头
			if(ptrIPD == NULL)											//如果没找到，可能是IPD头的延迟，还是需要等待一会，但不会超过设定的时间
			{
				//UsartPrintf(USART_DEBUG, "\"IPD\" not found\r\n");
			}
			else
			{
				ptrIPD = strchr(ptrIPD, ':');							//找到':'
				if(ptrIPD != NULL)
				{
					ptrIPD++;
					return (unsigned char *)(ptrIPD);
				}
				else
					return NULL;
				
			}
		}
		
		HAL_Delay(10);													//延时等待
	} while(timeOut--);
	
	return NULL;														//超时还未找到，返回空指针

}


//==========================================================
//	函数名称：	ESP8266_Init
//
//	函数功能：	初始化ESP8266
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void ESP8266_Init(void)
{
	
	ESP8266_Clear();
	
	HAL_UART_Printf(USART_DEBUG, "1. AT\r\n");
	while(ESP8266_SendCmd("AT\r\n", "OK"))
		HAL_Delay(500);
	
	HAL_UART_Printf(USART_DEBUG, "2. CWMODE\r\n");
	while(ESP8266_SendCmd("AT+CWMODE=1\r\n", "OK"))
		HAL_Delay(500);
	
	HAL_UART_Printf(USART_DEBUG, "3. AT+CWDHCP\r\n");
	while(ESP8266_SendCmd("AT+CWDHCP=1,1\r\n", "OK"))
		HAL_Delay(500);
	
	HAL_UART_Printf(USART_DEBUG, "4. CWJAP\r\n");
	while(ESP8266_SendCmd(ESP8266_WIFI_INFO, "GOT IP"))
		HAL_Delay(500);
		HAL_UART_Printf(USART_DEBUG, "5. CWJAP\r\n");
			while(ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
		HAL_Delay(500);
			
	HAL_UART_Printf(USART_DEBUG, "5. ESP8266 Init OK\r\n");
			


}

//==========================================================
//	函数名称：	USART2_IRQHandler
//
//	函数功能：	串口2收发中断
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		  HAL_UART_IRQHandler(&huart9);

//==========================================================

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart9) // 判断是串口2发出的中断
	{
		if(esp8266_cnt >= sizeof(esp8266_buf))
		{
			esp8266_cnt = 0; // 防止缓冲区溢出
		}
		esp8266_buf[esp8266_cnt++] = esp8266_data[0]; // 类型转换并将数据存储到缓冲区中
		
		HAL_UART_Receive_IT(&huart9, &esp8266_data[0], 1); // 继续接收下一个字节
	}
//	if(huart == &huart3)
//	{//HAL_UART_Printf(USART_DEBUG, "bbbbbbbbbbbbb\r\n");
//		if(detect[0] =='A' )
//	{
//		  HAL_UART_Receive(&huart3, A_data,1,HAL_MAX_DELAY);
//			A_type(&A_data[0]);
//		  //HAL_UART_Transmit_IT(&huart2, A_data, 1);
//		  HAL_UART_Receive_IT(&huart3,detect,1);
//	}
//	else if(detect[0]=='B'){
//		HAL_UART_Receive(&huart3, B_data,1,HAL_MAX_DELAY);
//			B_type(&B_data[0]);
//	    HAL_UART_Receive_IT(&huart3,detect,1);	
//	}
//	else if (detect[0]=='C'){
//		HAL_UART_Receive(&huart3, C_data,1,HAL_MAX_DELAY);
//			C_type(&C_data[0]);
//	    HAL_UART_Receive_IT(&huart3,detect,1);	
//	
//	}
//	else{
//				HAL_UART_Receive_IT(&huart3,detect,1);
//		}
//	HAL_UART_Receive_IT(&huart3,detect,1);
//	}
}
