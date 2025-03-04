#include "sensor.h"
#include "adc.h"
#include "tim.h"
#include <stdio.h>

//==============================================================================
//                              DHT11 部分
//==============================================================================

// DHT11 GPIO配置函数
static void DHT11_IO_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_7;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

static void DHT11_IO_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_7;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}

// DHT11基础函数
static void DHT11_Rst(void)
{
    DHT11_IO_OUT();
    DHT11_DQ_OUT_LOW;
    HAL_Delay(20);
    DHT11_DQ_OUT_HIGH;
    delay_us(30);
    DHT11_IO_IN();
}

uint8_t DHT11_Check(void) 	   
{   
    uint8_t retry = 0;
    
    // 等待DHT11的80us低电平响应信号
    while (DHT11_DQ_IN && retry < 100)  // 等待变低
    {
        retry++;
        delay_us(1);
    }
    if(retry >= 100) return 1;
    
    retry = 0;
    // 等待DHT11的80us高电平响应信号
    while (!DHT11_DQ_IN && retry < 100)  // 等待变高
    {
        retry++;
        delay_us(1);
    }
    if(retry >= 100) return 1;
    
    return 0;  // 响应正确
}
uint8_t DHT11_Init(void)
{
    DHT11_Rst();
    return DHT11_Check();
}
static uint8_t DHT11_Read_Byte(void)
{
    uint8_t i;
    uint8_t dat = 0;
    uint8_t retry;
    
    for(i = 0; i < 8; i++)
    {
        retry = 2;
        while((!DHT11_DQ_IN) && retry++)
        {
            delay_us(1);
        }
        
        delay_us(10);
        delay_us(10);
        delay_us(10);
        
        uint8_t temp = 0;
        if(DHT11_DQ_IN) temp = 1;
        
        retry = 2;
        while((DHT11_DQ_IN) && retry++)
        {
            delay_us(1);
        }
        
        dat <<= 1;
        dat |= temp;
        
        if(retry == 1) break;
    }
    return dat;
}

// DHT11数据读取函数
static uint8_t DHT11_Read_Data(uint16_t *temp, uint16_t *humi)
{
    uint8_t retry;
    uint8_t data_H_temp, data_L_temp, temp_H_temp, temp_L_temp, checksum_temp;
    
    // 发送起始信号
    DHT11_IO_OUT();
    DHT11_DQ_OUT_LOW;
    HAL_Delay(18);      // 至少18ms
    DHT11_DQ_OUT_HIGH;
    delay_us(40);       // 40us
    
    // 切换为输入
    DHT11_IO_IN();
    
    // 检测DHT11响应
    if(!DHT11_DQ_IN)    // 检测到低电平响应
    {
        retry = 2;
        // 等待80us低电平响应信号
        while((!DHT11_DQ_IN) && retry++)
        {
            delay_us(1);
        }
        
        retry = 2;
        // 等待80us高电平
        while(DHT11_DQ_IN && retry++)
        {
            delay_us(1);
        }
        
        // 开始接收数据
        data_H_temp = DHT11_Read_Byte();  // 湿度整数
        data_L_temp = DHT11_Read_Byte();  // 湿度小数
        temp_H_temp = DHT11_Read_Byte();  // 温度整数
        temp_L_temp = DHT11_Read_Byte();  // 温度小数
        checksum_temp = DHT11_Read_Byte(); // 校验和
        
        DHT11_IO_OUT();
        DHT11_DQ_OUT_HIGH;  // 释放总线
        
        // 校验数据
        uint8_t sum = data_H_temp + data_L_temp + temp_H_temp + temp_L_temp;
        if(sum == checksum_temp)
        {
            *humi = (data_H_temp << 8) + data_L_temp;  // 高字节左移8位加上低字节
            *temp = (temp_H_temp << 8) + temp_L_temp;  // 高字节左移8位加上低字节
            return 0;
        }
    }
    return 1;
}

//==============================================================================
//                              MQ2 部分
//==============================================================================

// MQ2基础函数
static uint16_t MQ2_Get_Value(void)
{
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 100);
    return HAL_ADC_GetValue(&hadc1);
}

static float MQ2_Get_PPM(void)
{
    uint16_t adc_value = MQ2_Get_Value();
    float voltage = (float)adc_value * (3.3 / 4096);
    float ppm = voltage * 1000;
    return ppm;
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
        printf("\r\n====== Sensor Initialization ======\r\n");
        
        // 初始化DHT11
        while(1)
        {
            DHT11_Rst();
            while(DHT11_Init())
        {
            printf("DHT11 Checked failed!!!\r\n");
            HAL_Delay(500);
        }
        printf("DHT11 Checked Success!!!\r\n");
        is_initialized = 1;
				break;
        }
        
        // 初始化MQ2
        HAL_ADC_Start(&hadc1);
        printf("MQ2 Init Success!\r\n");
        
        printf("====== Init Complete ======\r\n\n");
        is_initialized = 1;
    }
}

// 获取并打印所有传感器数据
void Sensor_Get_All_Data(void)
{
    static uint16_t temperature, humidity;
    
    // 获取DHT11数据
    if(DHT11_Read_Data(&temperature, &humidity) == 0)
    {
        printf("Temperature = %d.%d `C  Humidity = %d.%d%%\r\n",
               temperature>>8, temperature&0xff,
               humidity>>8, humidity&0xff);
    }
    
    // 获取MQ2数据
    float ppm = MQ2_Get_PPM();
    printf("Gas concentration = %.2f ppm\r\n", ppm);
}