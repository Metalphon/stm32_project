#ifndef __SENSOR_H__
#define __SENSOR_H__
#include "main.h"

// DHT11 GPIO控制宏定义
#define DHT11_DQ_OUT_HIGH HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_SET)
#define DHT11_DQ_OUT_LOW  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET)
#define DHT11_DQ_IN       HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7)

// 对外接口函数
void Sensor_Init(void);              // 初始化所有传感器
void Sensor_Get_All_Data(void);      // 获取并打印所有传感器数据

#endif 