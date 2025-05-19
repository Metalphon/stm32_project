/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include "ctype.h"
// 稍后我们会重新启用sensor.h
#include "sensor.h" 
#include "core_cm4.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// 串口接收缓冲区大�?
#define UART_RX_BUFFER_SIZE 1024  // 增加缓冲区大�?

// OneNET MQTT 相关定义
#define MQTT_USERCFG "AT+MQTTUSERCFG=0,1,\"d1\",\"f04YhrRc9T\",\"version=2018-10-31&res=products%2Ff04YhrRc9T%2Fdevices%2Fd1&et=2042628885&method=md5&sign=ISybjS%2B%2F0HV3P5suvrapGQ%3D%3D\",0,0,\"\"\r\n"
#define MQTT_CONN "AT+MQTTCONN=0,\"mqtts.heclouds.com\",1883,1\r\n"
#define MQTT_TOPIC "$sys/f04YhrRc9T/d1/thing/property/post"
#define MQTT_PUBRAW_HEADER "AT+MQTTPUBRAW=0,\"$sys/f04YhrRc9T/d1/thing/property/post\","

// 状�?�定�?
#define STATUS_INIT             0
#define STATUS_WIFI_CONNECTING  1
#define STATUS_MQTT_CONFIG      2
#define STATUS_MQTT_CONNECTING  3
#define STATUS_MQTT_CONNECTED   4
#define STATUS_MQTT_PUBLISHING  5
#define STATUS_MQTT_PUBLISHED   6
#define STATUS_ERROR            7

// 数据发�?�间�? (3�?)
#define PUBLISH_INTERVAL 3000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* USER CODE BEGIN PV */
// 串口1接收缓冲�?
uint8_t uart1RxByte;
uint8_t uart1RxBuffer[UART_RX_BUFFER_SIZE];
volatile uint16_t uart1RxCount = 0;

// 串口2接收缓冲�?
uint8_t uart2RxByte;
uint8_t uart2RxBuffer[UART_RX_BUFFER_SIZE];
volatile uint16_t uart2RxCount = 0;

// 命令完成标志
volatile uint8_t cmdReady = 0;

// MQTT状�??
volatile uint8_t mqttStatus = STATUS_INIT;

// 消息ID计数�?
uint32_t messageId = 1;

// 数据上报定时�?
uint32_t lastPublishTime = 0;

// 模拟传感器数�? (递增)
int32_t tempValue = 20;  // 初始温度
int32_t humiValue = 40;  // 初始湿度

// ESP8266响应测试定时�?
uint32_t responseTimeout = 0;
const uint32_t MAX_RESPONSE_TIMEOUT = 10000; // 增加10秒超�?

// 重试计数�?
uint8_t retryCount = 0;
const uint8_t MAX_RETRY_COUNT = 3;

// 上次接收数据的时�?
uint32_t lastReceiveTime = 0;

// 发�?�后等待标志
volatile uint8_t waitAfterResponse = 0;
// 发�?�后等待时间
uint32_t waitStartTime = 0;
const uint32_t WAIT_AFTER_RESPONSE = 1000; // 获得应答后等�?1�?

// AT命令发�?�时间记录器
volatile uint32_t lastCommandTime = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void printDebug(char* message);
void sendATCommand(char* command);
void publishSensorData(void);
void connectToMQTT(void);
uint8_t checkResponse(char* expectedResponse);
void processESP8266Response(void);
void clearBuffer(void);
void resetESP8266(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// �?单的调试打印函数
void printDebug(char* message) {
    HAL_UART_Transmit(&huart1, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);
}

// 连接到MQTT服务�?
void connectToMQTT(void) {
    // 打印调试信息
    printDebug("[MQTT] Starting MQTT connection process...\r\n");
    
    // 发�?�MQTT配置命令
    sendATCommand(MQTT_USERCFG);
    mqttStatus = STATUS_MQTT_CONFIG;
    
    // 等待2�?
    HAL_Delay(2000);
    
    // 发�?�MQTT连接命令
    sendATCommand(MQTT_CONN);
    mqttStatus = STATUS_MQTT_CONNECTING;
}

// 向ESP8266发�?�AT命令
void sendATCommand(char* command) {
    // 清空接收缓冲�?
    memset(uart2RxBuffer, 0, UART_RX_BUFFER_SIZE);
    uart2RxCount = 0;
    
    // 发�?�AT命令到ESP8266
    HAL_UART_Transmit(&huart2, (uint8_t*)command, strlen(command), HAL_MAX_DELAY);
    
    // 记录发�?�时�?
    lastCommandTime = HAL_GetTick();
}

// �?查ESP8266的响应中是否包含指定字符�?
uint8_t checkResponse(char* expectedResponse) {
    // 忽略大小写搜索，提高兼容�?
    char lowerBuffer[UART_RX_BUFFER_SIZE];
    char lowerExpected[100];
    
    // 复制并转换为小写
    strncpy(lowerBuffer, (char*)uart2RxBuffer, UART_RX_BUFFER_SIZE-1);
    lowerBuffer[UART_RX_BUFFER_SIZE-1] = '\0';
    
    strncpy(lowerExpected, expectedResponse, 99);
    lowerExpected[99] = '\0';
    
    // 转换为小�?
    for(int i = 0; lowerBuffer[i]; i++) {
        lowerBuffer[i] = tolower(lowerBuffer[i]);
    }
    
    for(int i = 0; lowerExpected[i]; i++) {
        lowerExpected[i] = tolower(lowerExpected[i]);
    }
    
    return (strstr(lowerBuffer, lowerExpected) != NULL);
}

// 清空缓冲�?
void clearBuffer(void) {
    memset(uart2RxBuffer, 0, UART_RX_BUFFER_SIZE);
    uart2RxCount = 0;
}

// 重置ESP8266模块 - 更可靠的版本
void resetESP8266(void) {
    printDebug("\r\n[RESET] Resetting ESP8266...\r\n");
    
    // 清空缓冲�?
    clearBuffer();
    
    // 发�?�重置命�?
    HAL_UART_Transmit(&huart2, (uint8_t*)"AT+RST\r\n", 8, HAL_MAX_DELAY);
    
    // 等待ESP8266重启
    printDebug("[RESET] Waiting for ESP8266 to fully restart (10 seconds)...\r\n");
    HAL_Delay(10000);
    
    // 清空缓冲�?
    clearBuffer();
    
    // 发�?�几个AT命令确保通信同步
    for (int i = 0; i < 3; i++) {
        HAL_UART_Transmit(&huart2, (uint8_t*)"AT\r\n", 4, HAL_MAX_DELAY);
        HAL_Delay(500);
    }
    
    printDebug("[RESET] ESP8266 reset complete, restarting connection process\r\n");
    
    // 重置�?有状�?
    mqttStatus = STATUS_INIT;
    retryCount = 0;
}

// 处理ESP8266的响�?
void processESP8266Response(void) {
    // 更新上次接收时间
    lastReceiveTime = HAL_GetTick();
    
    // 在初始状态收到AT的OK响应, 直接发�?�MQTT配置命令
    if(mqttStatus == STATUS_INIT && checkResponse("OK")) {
        // 清空缓冲�?
        clearBuffer();
        
        // 直接发�?�MQTT用户配置命令
        sendATCommand(MQTT_USERCFG);
        
        // 更新状�??
        mqttStatus = STATUS_MQTT_CONFIG;
        return;
    }
    
    // �?查MQTT配置成功
    if(mqttStatus == STATUS_MQTT_CONFIG && checkResponse("OK")) {
        // 清空缓冲�?
        clearBuffer();
        
        // 更新状�??
        mqttStatus = STATUS_MQTT_CONNECTING;
        
        // 发�?�连接命�?
        sendATCommand(MQTT_CONN);
        return;
    }
    
    // 测试MQTT连接成功
    if(checkResponse("+MQTTCONNECTED") || checkResponse("+MQTTCNNECTD") || 
       checkResponse("+MQTTCNNECT") || checkResponse("CONNECTED") || 
       checkResponse("MQTT CONNECTED")) {
        // 清空缓冲�?
        clearBuffer();
        
        // 更新状�??
        mqttStatus = STATUS_MQTT_CONNECTED;
        retryCount = 0;
        
        // 连接成功后，设置首次发布时间
        lastPublishTime = HAL_GetTick();
        return;
    }
    
    // 测试发布就绪标志">"
    if(checkResponse(">")) {
        // 构建JSON数据
        char jsonData[256];
        sprintf(jsonData, "{\"id\": \"%u\",\"version\": \"1.0\",\"params\": {\"temp\":{\"value\": %d},\"humi\":{\"value\": %d}}}", 
                messageId++, 
                tempValue, 
                humiValue);
        
        // 通过串口2发�?�JSON数据到ESP8266
        HAL_UART_Transmit(&huart2, (uint8_t*)jsonData, strlen(jsonData), HAL_MAX_DELAY);
        
        // 每次发�?�后递增数�??
        tempValue++;
        humiValue++;
        
        mqttStatus = STATUS_MQTT_PUBLISHED;
        clearBuffer();
        
        // 设置响应超时
        responseTimeout = HAL_GetTick();
        return;
    }
    
    // 发�?�确�?
    if(checkResponse("+MQTTPUB:OK") || checkResponse("+MQTTPUB:") || 
       checkResponse("+MQTTPUB") || checkResponse("PUB:OK")) {
        mqttStatus = STATUS_MQTT_CONNECTED; // 返回连接状�?�，准备下一次发�?
        
        // 设置等待标志和时�?
        waitAfterResponse = 1;
        waitStartTime = HAL_GetTick();
        
        retryCount = 0;
        return;
    }
    
    // �?查错误响�?
    if(checkResponse("ERROR") || checkResponse("FAIL") || checkResponse("+MQTTDISCONNECTED") || 
       checkResponse("DISCONNECTED") || checkResponse("DISCONNECT")) {
        mqttStatus = STATUS_ERROR;
    }
    
    // 错误状�?�处�?
    if(mqttStatus == STATUS_ERROR) {
        if(retryCount < MAX_RETRY_COUNT) {
            retryCount++;
            connectToMQTT();
        } else {
            resetESP8266();
        }
    }
}

// 处理串口2数据
void processUart2Data() {
    // 直接转发每个收到的字符到串口1（调试输出）
    HAL_UART_Transmit(&huart1, &uart2RxByte, 1, HAL_MAX_DELAY);
    
    // 存储字符到缓冲区
    if(uart2RxCount < UART_RX_BUFFER_SIZE - 1) {
        uart2RxBuffer[uart2RxCount++] = uart2RxByte;
        uart2RxBuffer[uart2RxCount] = '\0';  // 确保字符串始终以null结尾
    }
    
    // �?测到换行符，处理完整的响�?
    if(uart2RxByte == '\n' || uart2RxByte == '\r' || uart2RxCount >= UART_RX_BUFFER_SIZE - 1) {
        // 打印调试信息
        char debugMsg[32];
        sprintf(debugMsg, "[DEBUG] Status: %d\r\n", mqttStatus);
        printDebug(debugMsg);
        
        // 根据当前状�?�处理数�?
        switch(mqttStatus) {
            case STATUS_MQTT_CONFIG:
                // 等待2�?
                HAL_Delay(2000);
                // 发�?�MQTT连接命令
                printDebug("[MQTT] Sending connection command...\r\n");
                sendATCommand(MQTT_CONN);
                mqttStatus = STATUS_MQTT_CONNECTING;
                break;
                
            case STATUS_MQTT_CONNECTING:
                if(strstr((char*)uart2RxBuffer, "+MQTTCONNECTED") || 
                   strstr((char*)uart2RxBuffer, "CONNECTED")) {
                    printDebug("[MQTT] Connected successfully\r\n");
                    mqttStatus = STATUS_MQTT_CONNECTED;
                    lastPublishTime = HAL_GetTick();  // 准备首次发布
                }
                break;
                
            case STATUS_MQTT_PUBLISHING:
                if(strstr((char*)uart2RxBuffer, ">")) {
                    // 准备发�?�数�?
                    char jsonData[256];
                    sprintf(jsonData, "{\"id\":\"%u\",\"version\":\"1.0\",\"params\":{\"temp\":{\"value\":%d},\"humi\":{\"value\":%d}}}", 
                            messageId++, tempValue++, humiValue++);
                    HAL_UART_Transmit(&huart2, (uint8_t*)jsonData, strlen(jsonData), HAL_MAX_DELAY);
                    mqttStatus = STATUS_MQTT_PUBLISHED;
                }
                break;
                
            case STATUS_MQTT_PUBLISHED:
                if(strstr((char*)uart2RxBuffer, "+MQTTPUB:OK") || 
                   strstr((char*)uart2RxBuffer, "PUB:OK")) {
                    printDebug("[MQTT] Data published successfully\r\n");
                    mqttStatus = STATUS_MQTT_CONNECTED;
                    lastPublishTime = HAL_GetTick();
                }
                break;
        }
        
        // �?查错误响�?
        if(strstr((char*)uart2RxBuffer, "ERROR") || 
           strstr((char*)uart2RxBuffer, "FAIL") || 
           strstr((char*)uart2RxBuffer, "DISCONNECT")) {
            printDebug("[ERROR] Command failed\r\n");
            mqttStatus = STATUS_ERROR;
        }
        
        // 清空缓冲�?
        memset(uart2RxBuffer, 0, UART_RX_BUFFER_SIZE);
        uart2RxCount = 0;
    }
    
    // 继续接收下一个字�?
    HAL_UART_Receive_IT(&huart2, &uart2RxByte, 1);
}

// 串口接收回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        // 继续接收
        HAL_UART_Receive_IT(&huart1, &uart1RxByte, 1);
    }
    else if(huart->Instance == USART2)
    {
        // 处理ESP8266返回的数�?
        processUart2Data();
        
        // 继续接收
        HAL_UART_Receive_IT(&huart2, &uart2RxByte, 1);
    }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	printf("1111111111111111");
  // 初始化串口接�?


  // 初始化传感器
  Sensor_Init();

  // 等待系统稳定
  //HAL_Delay(2000);

  // 发�?�MQTT配置命令
  printDebug("[MQTT] Sending MQTT configuration command...\r\n");
  sendATCommand(MQTT_USERCFG);
  mqttStatus = STATUS_MQTT_CONFIG;

  // 等待2�?
  //HAL_Delay(2000);

  // 发�?�MQTT连接命令
  printDebug("[MQTT] Sending MQTT connection command...\r\n");
  sendATCommand(MQTT_CONN);
  mqttStatus = STATUS_MQTT_CONNECTING;
  HAL_UART_Receive_IT(&huart1, &uart1RxByte, 1);
  HAL_UART_Receive_IT(&huart2, &uart2RxByte, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // 如果MQTT已连接且距离上次发布超过3�?
    if(mqttStatus == STATUS_MQTT_CONNECTED && 
       (HAL_GetTick() - lastPublishTime) >= PUBLISH_INTERVAL)
    {
        // 获取传感器数据并通过MQTT发布
        char pubCommand[128];
        sprintf(pubCommand, "%s%d\r\n", MQTT_PUBRAW_HEADER, messageId);
        sendATCommand(pubCommand);
        mqttStatus = STATUS_MQTT_PUBLISHING;
        
        // 更新发布时间
        lastPublishTime = HAL_GetTick();
    }
    
    // �?查是否需要重置ESP8266
    if(mqttStatus == STATUS_ERROR)
    {
        if(retryCount < MAX_RETRY_COUNT)
        {
            retryCount++;
            printDebug("[ERROR] Connection failed, retrying...\r\n");
            HAL_Delay(1000);
            sendATCommand(MQTT_USERCFG);
            mqttStatus = STATUS_MQTT_CONFIG;
        }
        else
        {
            printDebug("[ERROR] Max retry count reached, resetting ESP8266...\r\n");
            resetESP8266();
            retryCount = 0;
        }
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
