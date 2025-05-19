#ifndef __SENSOR_H__
#define __SENSOR_H__
#include "main.h"

// DHT11 GPIO控制宏定义
#define DHT11_DQ_OUT_HIGH HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET)
#define DHT11_DQ_OUT_LOW  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET)
#define DHT11_DQ_IN       HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)

// 微秒延时函数声明
void delay_us(uint32_t us);

// 对外接口函数
void Sensor_Init(void);              // 初始化所有传感器
void Sensor_Get_All_Data(void);      // 获取并打印所有传感器数据
uint8_t DHT11_Read_Data(uint16_t *temp, uint16_t *humi); // 读取DHT11数据

#endif 