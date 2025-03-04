/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <string.h>

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart9;

extern UART_HandleTypeDef huart3;
/* 在 usart.h 中添加 */
#define UART3_RX_BUFFER_SIZE   256    // 接收缓冲区大小
#define UART3_TX_BUFFER_SIZE   256    // 发送缓冲区大小

extern uint8_t UART3_RxBuffer[UART3_RX_BUFFER_SIZE];
extern uint8_t UART3_TxBuffer[UART3_TX_BUFFER_SIZE];

void UART3_DMA_Start_Receive(void);
void UART3_DMA_Send_Data(uint8_t *data, uint16_t size);
/* USER CODE BEGIN Private defines */

#define USART_DEBUG		&huart3		//���Դ�ӡ��ʹ�õĴ�����

extern uint8_t detect[1];
extern uint8_t A_data[1];
extern uint8_t B_data[1];
extern uint8_t C_data[1];

void A_type(uint8_t *A_data);
void B_type(uint8_t *B_data);
void C_type(uint8_t *C_data);

/* USER CODE END Private defines */

void MX_UART9_Init(void);
void MX_USART3_UART_Init(void);

/* USER CODE BEGIN Prototypes */
void HAL_UART_Printf(UART_HandleTypeDef *huart, char *fmt, ...);
void Usart_SendString(UART_HandleTypeDef *huart, uint8_t *str, uint16_t len);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

