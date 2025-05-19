/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    sensors.h
  * @brief   This file contains all the function prototypes for
  *          the sensors.c file
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
#ifndef __SENSORS_H__
#define __SENSORS_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include <stdio.h>  // 添加stdio.h以使用sprintf函数
#include <stdarg.h> // 添加stdarg.h以支持可变参数函数
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
// DHT11引脚定义
#define DHT11_DQ_OUT_HIGH   HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET)
#define DHT11_DQ_OUT_LOW    HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_RESET)
#define DHT11_DQ_IN         HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin)

// DHT11传感器数据结构
typedef struct {
    uint8_t temperature;  // 温度
    uint8_t humidity;     // 湿度
} DHT11_Data_t;

// GPS数据结构
typedef struct {
    char data[100];       // GPS原始数据
    char latitude[15];    // 纬度
    char longitude[15];   // 经度
    uint8_t valid;        // 数据有效标志
} GPS_Data_t;

// 所有传感器数据结构
typedef struct {
    uint8_t fire_1;       // 火光传感器1状态 (0:无火光, 1:检测到火光)
    uint8_t fire_2;       // 火光传感器2状态
    uint8_t fire_3;       // 火光传感器3状态
    uint8_t fire_4;       // 火光传感器4状态
    DHT11_Data_t dht11;   // DHT11温湿度传感器数据
    uint16_t soil;        // 土壤湿度值 (ADC值)
    uint16_t fog;         // 烟雾浓度值 (ADC值)
    uint16_t ph;          // PH值 (ADC值)
    GPS_Data_t gps;       // GPS数据
    uint16_t rescue_count; // 救援人数
} Sensors_Data_t;
/* USER CODE END Private defines */

/* 传感器初始化函数 */
void Sensors_Init(void);

/* 获取所有传感器数据 */
Sensors_Data_t Sensors_GetAllData(void);

/* 发送所有传感器数据到USART3 */
void Sensors_SendDataToUSART3(void);

/* 传感器定时处理函数 (每1秒调用一次) */
void Sensors_TimerHandler(void);

/* DHT11相关函数 */
uint8_t DHT11_ReadData(DHT11_Data_t* data);

/* ADC传感器读取函数 */
uint16_t Sensors_ReadADC(uint32_t channel);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __SENSORS_H__ */ 