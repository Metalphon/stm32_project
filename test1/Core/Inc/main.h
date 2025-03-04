/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
#define L298N_Pin GPIO_PIN_0
#define L298N_GPIO_Port GPIOA
#define LN1_Pin GPIO_PIN_0
#define LN1_GPIO_Port GPIOB
#define LN2_Pin GPIO_PIN_1
#define LN2_GPIO_Port GPIOB
#define LN3_Pin GPIO_PIN_2
#define LN3_GPIO_Port GPIOB
#define LN4_Pin GPIO_PIN_10
#define LN4_GPIO_Port GPIOB
#define LN11_Pin GPIO_PIN_11
#define LN11_GPIO_Port GPIOB
#define LN22_Pin GPIO_PIN_12
#define LN22_GPIO_Port GPIOB
#define LN33_Pin GPIO_PIN_13
#define LN33_GPIO_Port GPIOB
#define LN44_Pin GPIO_PIN_14
#define LN44_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
