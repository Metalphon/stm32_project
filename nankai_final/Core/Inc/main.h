/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#define fire_1_Pin GPIO_PIN_0
#define fire_1_GPIO_Port GPIOA
#define DHT11_Pin GPIO_PIN_1
#define DHT11_GPIO_Port GPIOA
#define fire_2_Pin GPIO_PIN_4
#define fire_2_GPIO_Port GPIOA
#define fire_3_Pin GPIO_PIN_5
#define fire_3_GPIO_Port GPIOA
#define soil_Pin GPIO_PIN_6
#define soil_GPIO_Port GPIOA
#define stopauto_Pin GPIO_PIN_7
#define stopauto_GPIO_Port GPIOA
#define fog_Pin GPIO_PIN_0
#define fog_GPIO_Port GPIOB
#define ph_Pin GPIO_PIN_1
#define ph_GPIO_Port GPIOB
#define test_Pin GPIO_PIN_13
#define test_GPIO_Port GPIOB
#define fire_4_Pin GPIO_PIN_8
#define fire_4_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
