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

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern int qyvalue;

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define high_Pin GPIO_PIN_2
#define high_GPIO_Port GPIOF
#define low_Pin GPIO_PIN_3
#define low_GPIO_Port GPIOF
#define lf1_Pin GPIO_PIN_0
#define lf1_GPIO_Port GPIOA
#define lf2_Pin GPIO_PIN_1
#define lf2_GPIO_Port GPIOA
#define lb1_Pin GPIO_PIN_2
#define lb1_GPIO_Port GPIOA
#define lb2_Pin GPIO_PIN_3
#define lb2_GPIO_Port GPIOA
#define rf1_Pin GPIO_PIN_6
#define rf1_GPIO_Port GPIOC
#define rf2_Pin GPIO_PIN_7
#define rf2_GPIO_Port GPIOC
#define rb1_Pin GPIO_PIN_8
#define rb1_GPIO_Port GPIOC
#define rb2_Pin GPIO_PIN_9
#define rb2_GPIO_Port GPIOC
#define front_Pin GPIO_PIN_11
#define front_GPIO_Port GPIOA
#define en_Pin GPIO_PIN_10
#define en_GPIO_Port GPIOG
#define dir_Pin GPIO_PIN_11
#define dir_GPIO_Port GPIOG
#define clk_Pin GPIO_PIN_12
#define clk_GPIO_Port GPIOG

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
