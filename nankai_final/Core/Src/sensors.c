/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    sensors.c
  * @brief   This file provides code for the configuration
  *          and control of all sensors.
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

/* Includes ------------------------------------------------------------------*/
#include "sensors.h"
#include <string.h>  /* 添加 string.h 头文件，提供 memset, strstr, strchr, strncpy 函数 */
#include <stdlib.h>  /* 添加 stdlib.h 头文件，提供 atoi 函数 */

/* USER CODE BEGIN 0 */
// 传感器数据全局变量
static Sensors_Data_t g_sensorsData = {0};

// 发送缓冲区
static uint8_t g_txBuffer[128] = {0};

// UART接收缓冲区
static uint8_t g_uart1RxBuffer[16] = {0};  // 救援人数数据
static uint8_t g_uart2RxBuffer[128] = {0}; // GPS数据
static uint8_t g_uart2RxTemp[1] = {0};    // GPS数据临时缓冲区

// UART接收标志
static uint8_t g_uart1RxFlag = 0;
static uint8_t g_uart2RxFlag = 0;

// GPS数据接收索引
static uint16_t g_uart2RxIndex = 0;

//==============================================================================
//                              辅助函数
//==============================================================================

// 发送格式化数据到UART
static void Send_Formatted_Data(UART_HandleTypeDef *huart, const char *identifier, const char *format, ...)
{
    va_list args;
    int len;
    char formatted_data[80]; // 临时格式化数据缓冲区
    
    // 格式化数据部分
    va_start(args, format);
    len = vsprintf(formatted_data, format, args);
    va_end(args);
    
    // 组合成 "identifier:data\r\n" 格式
    len = sprintf((char*)g_txBuffer, "%s:%s\r\n", identifier, formatted_data);
    
    // 发送数据到指定UART接口
    HAL_UART_Transmit(huart, g_txBuffer, len, 100);
    
    // 如果不是USART1，且需要调试，则同时发送到USART1
    if (huart != &huart1 && huart == &huart3) {
        // 发送到USART1作为调试输出
        HAL_UART_Transmit(&huart1, g_txBuffer, len, 100);
    }
}

//==============================================================================
//                              微秒延时函数
//==============================================================================

static void delay_us(uint32_t us)
{
    // 针对STM32的优化延时
    uint32_t cycles = us * (SystemCoreClock / 1000000);
    
    // 调整循环比例系数，提高精度
    cycles = cycles / 2;  
    
    for(uint32_t i = 0; i < cycles; i++)
    {
        __NOP();
    }
}

//==============================================================================
//                              UART接收函数
//==============================================================================

// 开始接收GPS数据（字符流模式）
static void GPS_StartReceive(void)
{
    // 重置接收索引
    g_uart2RxIndex = 0;
    memset(g_uart2RxBuffer, 0, sizeof(g_uart2RxBuffer));
    
    // 开始接收第一个字符
    HAL_UART_Receive_IT(&huart2, g_uart2RxTemp, 1);
}

// GPS数据字符接收回调处理
static void GPS_CharReceived(void)
{
    // 收到结束符或缓冲区满时结束接收
    if(g_uart2RxTemp[0] == '\n' || g_uart2RxIndex >= sizeof(g_uart2RxBuffer) - 1) {
        g_uart2RxBuffer[g_uart2RxIndex] = '\0';  // 确保字符串结束
        g_uart2RxFlag = 1;  // 设置接收完成标志
    } else {
        // 保存接收到的字符
        g_uart2RxBuffer[g_uart2RxIndex++] = g_uart2RxTemp[0];
        
        // 继续接收下一个字符
        HAL_UART_Receive_IT(&huart2, g_uart2RxTemp, 1);
    }
}

//==============================================================================
//                              GPS数据处理函数
//==============================================================================

// 解析GPS NMEA数据获取经纬度
static void GPS_ParseData(void)
{
    char *p = (char*)g_uart2RxBuffer;
    
    // 寻找$GPRMC或$GNRMC语句
    p = strstr(p, "$GPRMC");
    if (p == NULL) {
        p = strstr((char*)g_uart2RxBuffer, "$GNRMC");
    }
    
    if (p != NULL) {
        // 跳过语句头
        p = strchr(p, ',');
        if (p) p++;
        
        // 跳过时间
        p = strchr(p, ',');
        if (p) p++;
        
        // 检查数据有效性
        if (*p == 'A') {
            g_sensorsData.gps.valid = 1;
            
            // 跳过A
            p = strchr(p, ',');
            if (p) p++;
            
            // 获取纬度
            char *q = strchr(p, ',');
            if (q) {
                int len = q - p;
                if (len < sizeof(g_sensorsData.gps.latitude) - 1) {
                    strncpy(g_sensorsData.gps.latitude, p, len);
                    g_sensorsData.gps.latitude[len] = '\0';
                }
                
                // 跳过纬度N/S标识
                p = strchr(q + 1, ',');
                if (p) p++;
                
                // 获取经度
                q = strchr(p, ',');
                if (q) {
                    len = q - p;
                    if (len < sizeof(g_sensorsData.gps.longitude) - 1) {
                        strncpy(g_sensorsData.gps.longitude, p, len);
                        g_sensorsData.gps.longitude[len] = '\0';
                    }
                }
            }
        } else {
            g_sensorsData.gps.valid = 0;
        }
    }
}

// 解析救援人数数据
static void Rescue_ParseData(void)
{
    // 假设接收到的是一个数字字符串
    g_sensorsData.rescue_count = g_uart1RxBuffer[0] - '0';

}

//==============================================================================
//                              DHT11 部分
//==============================================================================

// DHT11 GPIO配置函数
static void DHT11_IO_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = DHT11_Pin;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = GPIO_PULLUP; // 强制使用内部上拉
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStructure);
}

static void DHT11_IO_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = DHT11_Pin;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP; // 输出模式也保持上拉
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStructure);
}

// DHT11基础函数
static void DHT11_Rst(void)
{
    // 复位序列
    DHT11_IO_OUT();      // 设置为输出
    DHT11_DQ_OUT_LOW;    // 拉低信号线
    HAL_Delay(20);       // 保持至少18ms (我们用20ms)
    DHT11_DQ_OUT_HIGH;   // 释放总线
    delay_us(40);        // 主机释放总线 20-40us
    DHT11_IO_IN();       // 设置为输入，准备接收DHT11响应
}

static uint8_t DHT11_Check(void) 	   
{   
    uint8_t retry = 0;
    
    // 第一步：检测DHT11是否响应，应该先出现低电平
    while (DHT11_DQ_IN && retry < 100)
    {
        retry++;
        delay_us(1);
    }
    if(retry >= 100) return 1;
    
    // 第二步：等待低电平结束
    retry = 0;
    while (!DHT11_DQ_IN && retry < 100)
    {
        retry++;
        delay_us(1);
    }
    if(retry >= 100) return 1;
    
    // 第三步：等待高电平结束
    retry = 0;
    while (DHT11_DQ_IN && retry < 100)
    {
        retry++;
        delay_us(1);
    }
    if(retry >= 100) return 1;
    
    return 0;  // 响应正确
}

static uint8_t DHT11_Read_Bit(void)
{
    uint8_t retry = 0;
    
    // 等待数据位开始（低电平）
    while(DHT11_DQ_IN && retry < 100)
    {
        retry++;
        delay_us(1);
    }
    
    // 等待低电平结束
    retry = 0;
    while(!DHT11_DQ_IN && retry < 100)
    {
        retry++;
        delay_us(1);
    }
    
    // 延时30us后判断电平
    delay_us(40); // 读取延迟时间增加到40us
    
    if(DHT11_DQ_IN)
        return 1;
    else
        return 0;
}

static uint8_t DHT11_Read_Byte(void)
{
    uint8_t i, dat = 0;
    
    for(i = 0; i < 8; i++)
    {
        dat <<= 1;
        dat |= DHT11_Read_Bit();
    }
    
    return dat;
}
/* USER CODE END 0 */

/**
  * @brief  初始化所有传感器
  * @retval None
  */
void Sensors_Init(void)
{
    /* USER CODE BEGIN Sensors_Init */
    // ADC已在main.c中初始化
    
    // 初始化DHT11
    // 1. 先配置为输出模式并输出高电平
    DHT11_IO_OUT();
    DHT11_DQ_OUT_HIGH;
    
    // 2. 等待DHT11上电稳定（至少1秒）
    HAL_Delay(1000);
    
    // 3. 发送复位信号
    DHT11_Rst();
    
    // 4. 检查DHT11响应
    uint8_t retry_count = 0;
    uint8_t max_retries = 5;
    
    while(retry_count < max_retries)
    {
        if(DHT11_Check() == 0) break;
        
        retry_count++;
        HAL_Delay(1000);  // 失败后等待更长时间再重试
    }
    
    // 5. 初始化UART接收
    // 启动UART1接收（救援人数数据）
    HAL_UART_Receive_IT(&huart1, g_uart1RxBuffer, 1);
    
    // 启动UART2接收（GPS数据）
    GPS_StartReceive();
    
    // 6. 初始化GPS和救援人数数据
    g_sensorsData.gps.valid = 0;
    memset(g_sensorsData.gps.latitude, 0, sizeof(g_sensorsData.gps.latitude));
    memset(g_sensorsData.gps.longitude, 0, sizeof(g_sensorsData.gps.longitude));
    g_sensorsData.rescue_count = 0;
    /* USER CODE END Sensors_Init */
}

/**
  * @brief  读取DHT11传感器数据
  * @param  data: 指向DHT11数据结构的指针
  * @retval 0: 成功, 1: 失败
  */
uint8_t DHT11_ReadData(DHT11_Data_t* data)
{
    /* USER CODE BEGIN DHT11_ReadData */
    uint8_t data_H_humi, data_L_humi, data_H_temp, data_L_temp, checksum;
    
    // 0. 初始状态检查
    if(DHT11_DQ_IN != GPIO_PIN_SET) return 1;
    
    // 1. 发送起始信号
    DHT11_Rst();
    
    // 2. 等待并检查DHT11响应
    if(DHT11_Check() != 0) return 1;
    
    // 3. 开始接收数据
    data_H_humi = DHT11_Read_Byte();  // 湿度高字节
    data_L_humi = DHT11_Read_Byte();  // 湿度低字节
    data_H_temp = DHT11_Read_Byte();  // 温度高字节
    data_L_temp = DHT11_Read_Byte();  // 温度低字节
    checksum = DHT11_Read_Byte();     // 校验和
    
    // 4. 释放总线
    DHT11_IO_OUT();
    DHT11_DQ_OUT_HIGH;
    
    // 5. 校验数据
    uint8_t sum = data_H_humi + data_L_humi + data_H_temp + data_L_temp;
    
    if(sum == checksum)
    {
        // 检查数据是否合理范围
        if(data_H_humi > 100 || data_H_temp > 80) return 1;
        
        // 保存数据 - 只使用整数部分
        data->humidity = data_H_humi;
        data->temperature = data_H_temp;
        
        return 0;
    }
    else
    {
        return 1;
    }
    /* USER CODE END DHT11_ReadData */
}

/**
  * @brief  读取指定通道的ADC值
  * @param  channel: ADC通道
  * @retval ADC转换结果
  */
uint16_t Sensors_ReadADC(uint32_t channel)
{
    /* USER CODE BEGIN Sensors_ReadADC */
    ADC_ChannelConfTypeDef sConfig = {0};
    uint16_t adcValue = 0;
    
    // 配置ADC通道
    sConfig.Channel = channel;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
    
    // 启动ADC转换
    HAL_ADC_Start(&hadc1);
    
    // 等待转换完成
    if (HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
    {
        adcValue = HAL_ADC_GetValue(&hadc1);
    }
    
    HAL_ADC_Stop(&hadc1);
    
    return adcValue;
    /* USER CODE END Sensors_ReadADC */
}

/**
  * @brief  获取所有传感器数据
  * @retval 传感器数据结构
  */
Sensors_Data_t Sensors_GetAllData(void)
{
    /* USER CODE BEGIN Sensors_GetAllData */
    // 读取火光传感器状态 (数字输入)
    g_sensorsData.fire_1 = HAL_GPIO_ReadPin(fire_1_GPIO_Port, fire_1_Pin);
    g_sensorsData.fire_2 = HAL_GPIO_ReadPin(fire_2_GPIO_Port, fire_2_Pin);
    g_sensorsData.fire_3 = HAL_GPIO_ReadPin(fire_3_GPIO_Port, fire_3_Pin);
    g_sensorsData.fire_4 = HAL_GPIO_ReadPin(fire_4_GPIO_Port, fire_4_Pin);
    
    // 读取DHT11温湿度数据(如果读取失败，保持上次的有效值)
    if(DHT11_ReadData(&g_sensorsData.dht11) != 0) {
        // 读取失败时保留原值，确保不会发送无效数据
    }
    
    // 读取ADC传感器数据
    g_sensorsData.soil = Sensors_ReadADC(ADC_CHANNEL_6);  // PA6 - soil
    g_sensorsData.fog = Sensors_ReadADC(ADC_CHANNEL_8);   // PB0 - fog
    g_sensorsData.ph = Sensors_ReadADC(ADC_CHANNEL_9);    // PB1 - ph
    
    return g_sensorsData;
    /* USER CODE END Sensors_GetAllData */
}

/**
  * @brief  发送所有传感器数据到USART3
  * @retval None
  */
void Sensors_SendDataToUSART3(void)
{
    /* USER CODE BEGIN Sensors_SendDataToUSART3 */
    // 获取最新的传感器数据
    
    // 读取火光传感器状态 (数字输入)
    g_sensorsData.fire_1 = HAL_GPIO_ReadPin(fire_1_GPIO_Port, fire_1_Pin);
    g_sensorsData.fire_2 = HAL_GPIO_ReadPin(fire_2_GPIO_Port, fire_2_Pin);
    g_sensorsData.fire_3 = HAL_GPIO_ReadPin(fire_3_GPIO_Port, fire_3_Pin);
    g_sensorsData.fire_4 = HAL_GPIO_ReadPin(fire_4_GPIO_Port, fire_4_Pin);
    
    // 读取DHT11温湿度数据(如果读取失败，保持上次的有效值)
    if(DHT11_ReadData(&g_sensorsData.dht11) != 0) {
        // 读取失败时保留原值，确保不会发送无效数据
    }
    
    // 读取ADC传感器数据
    g_sensorsData.soil = Sensors_ReadADC(ADC_CHANNEL_6);  // PA6 - soil
    g_sensorsData.fog = Sensors_ReadADC(ADC_CHANNEL_8);   // PB0 - fog
    g_sensorsData.ph = Sensors_ReadADC(ADC_CHANNEL_9);    // PB1 - ph
    
    // 处理GPS数据和救援人数数据
    if(g_uart2RxFlag) {
        GPS_ParseData();
        g_uart2RxFlag = 0;
        // 重新启动GPS数据接收
        GPS_StartReceive();
    }
    
    if(g_uart1RxFlag) {
        Rescue_ParseData();
        g_uart1RxFlag = 0;
        // 重新启动UART1接收
        HAL_UART_Receive_IT(&huart1, g_uart1RxBuffer, 1);
    }
    
    // 以"标识符:数据\r\n"格式发送各个传感器数据
    
    // 数据包开始标记
    Send_Formatted_Data(&huart3, "start", "1");
    
    // 1. 发送火光传感器状态
    // 合并四个火光传感器状态为一个值(有任一传感器检测到火光则为1)
    uint8_t fire_detected = 
    (g_sensorsData.fire_1 != 1) + 
    (g_sensorsData.fire_2 != 1) + 
    (g_sensorsData.fire_3 != 1) + 
    (g_sensorsData.fire_4 != 1);

    Send_Formatted_Data(&huart3, "fire", "%d", fire_detected);
    
    // 2. 发送温度数据
    Send_Formatted_Data(&huart3, "temp", "%d", g_sensorsData.dht11.temperature);
    
    // 3. 发送湿度数据
    Send_Formatted_Data(&huart3, "humid", "%d", g_sensorsData.dht11.humidity);
    
    // 4. 发送土壤湿度数据
    Send_Formatted_Data(&huart3, "soil", "%d", g_sensorsData.soil);
    Send_Formatted_Data(&huart3, "help", "%d", g_sensorsData.rescue_count);
    // 5. 发送烟雾数据
    Send_Formatted_Data(&huart3, "fog", "%d", g_sensorsData.fog);
    
    // 6. 发送PH值数据
    Send_Formatted_Data(&huart3, "ph", "%d", g_sensorsData.ph);
    
    // 7. 发送GPS位置数据（如果有效）
    if(g_sensorsData.gps.valid) {
        Send_Formatted_Data(&huart3, "lat", "%s", g_sensorsData.gps.latitude);
        Send_Formatted_Data(&huart3, "lon", "%s", g_sensorsData.gps.longitude);
    } else {
        Send_Formatted_Data(&huart3, "lat", "0.0");
        Send_Formatted_Data(&huart3, "lon", "0.0");
    }
    
    // 8. 发送救援人数数据
    
    
    // 数据包结束标记
    Send_Formatted_Data(&huart3, "end", "1");
    //HAL_Delay(5000);
    /* USER CODE END Sensors_SendDataToUSART3 */
}

/**
  * @brief  定时处理函数，1秒调用一次
  * @retval None
  */
void Sensors_TimerHandler(void)
{
    /* USER CODE BEGIN Sensors_TimerHandler */
    // 定时发送所有传感器数据
    Sensors_SendDataToUSART3();
    /* USER CODE END Sensors_TimerHandler */
}

/* USER CODE BEGIN 1 */
/**
  * @brief  UART接收完成回调函数
  * @param  huart: UART句柄
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1) {
        // 串口1接收完成，设置标志
        g_uart1RxFlag = 1;
        // 重新启动UART1接收
        HAL_UART_Receive_IT(&huart1, g_uart1RxBuffer, 1);
    }
    else if(huart->Instance == USART2) {
        // 串口2接收完成（字符流模式）
        GPS_CharReceived();
    }
}
/* USER CODE END 1 */ 