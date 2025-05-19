#include "sensor.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

//==============================================================================
//                              微秒延时函数
//==============================================================================

void delay_us(uint32_t us)
{
    // 针对STM32F4的优化延时
    uint32_t cycles = us * (SystemCoreClock / 1000000);
    
    // 调整循环比例系数，提高精度
    cycles = cycles / 2;  
    
    for(uint32_t i = 0; i < cycles; i++)
    {
        __NOP();
    }
}

//==============================================================================
//                              DHT11 部分
//==============================================================================

// DHT11 GPIO配置函数
static void DHT11_IO_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_5;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = GPIO_PULLUP; // 强制使用内部上拉
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void DHT11_IO_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_5;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP; // 输出模式也保持上拉
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
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

uint8_t DHT11_Init(void)
{
    // 1. 先配置为输出模式并输出高电平
    DHT11_IO_OUT();
    DHT11_DQ_OUT_HIGH;
    
    // 2. 等待DHT11上电稳定（至少1秒）
    HAL_Delay(1000);
    
    // 3. 发送复位信号
    DHT11_Rst();
    
    // 4. 检查DHT11响应
    return DHT11_Check();
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

// DHT11数据读取函数
uint8_t DHT11_Read_Data(uint16_t *temp, uint16_t *humi)
{
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
        
        // 构造返回值
        *humi = ((uint16_t)data_H_humi << 8) | data_L_humi;
        *temp = ((uint16_t)data_H_temp << 8) | data_L_temp;
        return 0;
    }
    else
    {
        return 1;
    }
}

//==============================================================================
//                              对外接口函数
//==============================================================================

// 初始化所有传感器
void Sensor_Init(void)
{
    static uint8_t is_initialized = 0;
    
    if(!is_initialized)
    {
        // 确保GPIO时钟已启用
        __HAL_RCC_GPIOA_CLK_ENABLE();
        
        // 延时1秒等待DHT11上电稳定
        HAL_Delay(1000);
        
        // 初始化DHT11
        uint8_t retry_count = 0;
        uint8_t max_retries = 5;
        
        while(retry_count < max_retries)
        {
            if(DHT11_Init() == 0) break;
            
            retry_count++;
            HAL_Delay(1000);  // 失败后等待更长时间再重试
        }
        
        is_initialized = 1;
    }
}

// 获取并打印所有传感器数据
void Sensor_Get_All_Data(void)
{
    uint16_t temperature = 0, humidity = 0;
    char buffer[100];
    int len;
    
    // 获取DHT11数据
    if(DHT11_Read_Data(&temperature, &humidity) == 0)
    {
        // 提取整数部分
        uint8_t temp_int = temperature >> 8;
        uint8_t humi_int = humidity >> 8;
        
        // 发送简化格式的温度数据
        len = sprintf(buffer, "temp:%d\r\n", temp_int);
        HAL_UART_Transmit(&huart1, (uint8_t*)buffer, len, 100);
        
        // 发送简化格式的湿度数据
        len = sprintf(buffer, "humid:%d\r\n", humi_int);
        HAL_UART_Transmit(&huart1, (uint8_t*)buffer, len, 100);
    }
    else
    {
        // 读取失败时发送错误状态
        len = sprintf(buffer, "temp:0\r\nhumid:0\r\n");
        HAL_UART_Transmit(&huart1, (uint8_t*)buffer, len, 100);
    }
} 