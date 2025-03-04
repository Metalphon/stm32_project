/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
extern unsigned char esp8266_buf[512];
extern unsigned char data[2];
//extern int i;
extern unsigned char esp8266_data[2];
extern uint8_t detect[1];
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/


/* USER CODE BEGIN Private defines */
#define LE_LOW_Pin GPIO_PIN_0
#define LE_LOW_GPIO_Port GPIOF
#define L_B_1_Pin GPIO_PIN_1
#define L_B_1_GPIO_Port GPIOF
#define L_B_2_Pin GPIO_PIN_2
#define L_B_2_GPIO_Port GPIOF
#define LE_HIGH_Pin GPIO_PIN_9
#define LE_HIGH_GPIO_Port GPIOF
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOH
#define BA_HIGH_Pin GPIO_PIN_9
#define BA_HIGH_GPIO_Port GPIOE
#define R_B_1_Pin GPIO_PIN_11
#define R_B_1_GPIO_Port GPIOE
#define R_B_2_Pin GPIO_PIN_13
#define R_B_2_GPIO_Port GPIOE
#define BA_LOW_Pin GPIO_PIN_14
#define BA_LOW_GPIO_Port GPIOE
#define RI_HIGH_Pin GPIO_PIN_12
#define RI_HIGH_GPIO_Port GPIOD
#define RI_LOW_Pin GPIO_PIN_13
#define RI_LOW_GPIO_Port GPIOD
#define R_F_1_Pin GPIO_PIN_14
#define R_F_1_GPIO_Port GPIOD
#define R_F_2_Pin GPIO_PIN_15
#define R_F_2_GPIO_Port GPIOD
#define FR_HIGH_Pin GPIO_PIN_6
#define FR_HIGH_GPIO_Port GPIOC
#define FR_LOW_Pin GPIO_PIN_7
#define FR_LOW_GPIO_Port GPIOC
#define L_F_1_Pin GPIO_PIN_8
#define L_F_1_GPIO_Port GPIOC
#define L_F_2_Pin GPIO_PIN_9
#define L_F_2_GPIO_Port GPIOC

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
