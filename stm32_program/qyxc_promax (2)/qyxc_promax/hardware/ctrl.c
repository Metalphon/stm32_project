#include "main.h"
#include "ctrl.h"
#include "esp8266.h"
#include "motor.h"
#include "usart.h"
#include "tim.h"

uint8_t mark[4]="OK\r\n";   //
uint8_t check[4]="AT\r\n";   //
uint8_t apmode[13]="AT+CWMODE=2\r\n";
//uint8_t rst[8]="AT+RST\r\n";//AT+CIPMODE
//uint8_t cpmode[14]="AT+CIPMODE=0";
uint8_t hotspot[33]="AT+CWSAP=\"TOHKA\",\"12345688\",1,3\r\n";
uint8_t multi_connect[13]="AT+CIPMUX=1\r\n";
uint8_t tcp_set[21]="AT+CIPSERVER=1,8080\r\n";
//uint8_t ip[12]="AT+CIFSR\r\n";
//uint8_t connect[44]="AT+CIPSTART=\"TCP\",\"192.168.4.2\",8081\r\n";
//uint8_t detect[1];
//uint8_t testingccp[10]="fuckyouccp";

void SystemClock_Config(void);
static void MX_GPIO_Init(void);



uint8_t respond0[8];

uint8_t respond1[11];
uint8_t respond2[20];
uint8_t respond3[612];
uint8_t respond4[40];
uint8_t respond5[20];
uint8_t respond6[28];
uint8_t respond7[12];
uint8_t respond8[28];
//uint8_t detect[1];
uint8_t data2[9];

uint8_t received_data[12];

void cot_init()
{
	  //HAL_TIM_PWM_Start(&lf, lfc2);
	  	//	__HAL_TIM_SET_COMPARE(&lf,lfc2,65);
	HAL_Delay(1000);
  HAL_UART_Transmit(&huart3, check, sizeof(check),HAL_MAX_DELAY);
 // HAL_UART_Receive(&huart3, respond1, 11,HAL_MAX_DELAY);
HAL_Delay(1000);
  HAL_UART_Transmit(&huart3, apmode, sizeof(apmode),HAL_MAX_DELAY);
  //HAL_UART_Receive(&huart3, respond2, 20,HAL_MAX_DELAY);
HAL_Delay(1000);
  HAL_UART_Transmit(&huart3, hotspot, sizeof(hotspot),HAL_MAX_DELAY);
  //HAL_UART_Receive(&huart3, respond4,40,HAL_MAX_DELAY);
HAL_Delay(1000);
  HAL_UART_Transmit(&huart3, multi_connect, sizeof(multi_connect),HAL_MAX_DELAY);
 // HAL_UART_Receive(&huart3, respond5,20,HAL_MAX_DELAY);
HAL_Delay(1000);
  HAL_UART_Transmit(&huart3, tcp_set, sizeof(tcp_set),HAL_MAX_DELAY);
  //HAL_UART_Receive(&huart3, respond6,26,HAL_MAX_DELAY);
	//HAL_Delay(500);
	//HAL_UART_Transmit(&huart2, testingccp, 10,HAL_MAX_DELAY);



}


void A_type(uint8_t *A_data)
{
	speed_low();
if(A_data[0]=='0')//stop
	{
			//HAL_UART_Transmit(&huart2, A_data,sizeof(A_data),HAL_MAX_DELAY);	  
			sportstop();
			qy_off();
	}
	else if(A_data[0] == '1')//front
	{
		//HAL_UART_Transmit(&huart2, A_data,sizeof(A_data),HAL_MAX_DELAY);
		sportfront(speed);
	}
	else if(A_data[0] == '2')//back
	{
		//HAL_UART_Transmit(&huart2, A_data,sizeof(A_data),HAL_MAX_DELAY);
		sportback(speed);
	}
	else if(A_data[0]== '3')//left
	{
		//HAL_UART_Transmit(&huart2, A_data,sizeof(A_data),HAL_MAX_DELAY);
		sportleft(speed);
	}
	else if(A_data[0]== '4')//right
	{
		//HAL_UART_Transmit(&huart2, A_data,sizeof(A_data),HAL_MAX_DELAY);
		sportright(speed);
	}
	else if(A_data[0]=='5')//front_clean
		{
			//HAL_UART_Transmit(&huart2, A_data,sizeof(A_data),HAL_MAX_DELAY);
			qyfront();
	}
	else if(A_data[0]=='6')//left_clean&right_clean
	{
		  qyleft();
			qyright();
	}

	else if(A_data[0]=='7')
	{
		//HAL_TIM_Base_Start_IT(&htim15);
//		  qydown();
			//HAL_Delay(100);
//			sportstop();
//			HAL_Delay(1000);
//			qyfront();
//			HAL_Delay(1000);
//		sportfront(500);
//			HAL_Delay(1000);
//			sportstop();
	}

	else if(A_data[0]=='8'){
qyup();
		}

	else if(A_data[0]=='9'){
qydown();
		}
}

void B_type(uint8_t *B_data)
{
		speed_middle();
if(B_data[0]=='0')//stop
	{
		//	HAL_UART_Transmit(&huart2, B_data,sizeof(B_data),HAL_MAX_DELAY);	  
			sportstop();
	}
	else if(B_data[0] == '1')//front
	{
//		HAL_UART_Transmit(&huart2, B_data,sizeof(B_data),HAL_MAX_DELAY);
		sportfront(speed);
	}
	else if(B_data[0] == '2')//back
	{
	//	HAL_UART_Transmit(&huart2, B_data,sizeof(B_data),HAL_MAX_DELAY);
		sportback(speed);
	}
	else if(B_data[0]== '3')//left
	{
		//HAL_UART_Transmit(&huart2, B_data,sizeof(B_data),HAL_MAX_DELAY);
		sportleft(speed);
	}
	else if(B_data[0]== '4')//right
	{
//		HAL_UART_Transmit(&huart2, B_data,sizeof(B_data),HAL_MAX_DELAY);
		sportright(speed);
	}
	else if(B_data[0]=='5')//front_clean
		{
//			HAL_UART_Transmit(&huart2, B_data,sizeof(B_data),HAL_MAX_DELAY);
			qyfront();
	}
	else if(B_data[0]=='6')//left_clean&right_clean
	{
		  qyleft();
			qyright();
	}

	else if(B_data[0]=='7')//auto
	{
			qydown();
			HAL_Delay(1000);
			sportstop();
			HAL_Delay(1000);
			qyfront();
			
	}

	else if(B_data[0]=='8'){
qyup();
		}

	else if(B_data[0]=='9'){
qydown();
		}
}


void C_type(uint8_t *C_data)
{
		speed_hight();
if(C_data[0]=='0')//stop
	{
	//		HAL_UART_Transmit(&huart2, C_data,sizeof(C_data),HAL_MAX_DELAY);	  
			sportstop();
	}
	else if(C_data[0] == '1')//front
	{
	//	HAL_UART_Transmit(&huart2, C_data,sizeof(C_data),HAL_MAX_DELAY);
		sportfront(speed);
	}
	else if(C_data[0] == '2')//back
	{
	//	HAL_UART_Transmit(&huart2, C_data,sizeof(C_data),HAL_MAX_DELAY);
		sportback(speed);
	}
	else if(C_data[0]== '3')//left
	{
	//	HAL_UART_Transmit(&huart2, C_data,sizeof(C_data),HAL_MAX_DELAY);
		sportleft(speed);
	}
	else if(C_data[0]== '4')//right
	{
		//HAL_UART_Transmit(&huart2, C_data,sizeof(C_data),HAL_MAX_DELAY);
		sportright(speed);
	}
	else if(C_data[0]=='5')//front_clean
		{
	//		HAL_UART_Transmit(&huart2, C_data,sizeof(C_data),HAL_MAX_DELAY);
			qyfront();
	}
	else if(C_data[0]=='6')//left_clean&right_clean
	{
		  qyleft();
			qyright();
	}

	else if(C_data[0]=='7')//auto
	{
			qydown();
			HAL_Delay(1000);
			sportstop();
			HAL_Delay(1000);
			qyfront();
			
	}

	else if(C_data[0]=='8'){
qyup();
		}

	else if(C_data[0]=='9'){
qydown();
		}
}


/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
//static void MX_GPIO_Init(void)
//{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
///* USER CODE BEGIN MX_GPIO_Init_1 */
///* USER CODE END MX_GPIO_Init_1 */

//  /* GPIO Ports Clock Enable */
//  __HAL_RCC_GPIOF_CLK_ENABLE();
//  __HAL_RCC_GPIOH_CLK_ENABLE();
//  __HAL_RCC_GPIOA_CLK_ENABLE();
//  __HAL_RCC_GPIOE_CLK_ENABLE();
//  __HAL_RCC_GPIOB_CLK_ENABLE();
//  __HAL_RCC_GPIOD_CLK_ENABLE();
//  __HAL_RCC_GPIOC_CLK_ENABLE();

//  /*Configure GPIO pin Output Level */
//  HAL_GPIO_WritePin(GPIOF, LE_LOW_Pin|LE_HIGH_Pin, GPIO_PIN_RESET);

//  /*Configure GPIO pin Output Level */
//  HAL_GPIO_WritePin(GPIOE, BA_HIGH_Pin|BA_LOW_Pin, GPIO_PIN_RESET);

//  /*Configure GPIO pin Output Level */
//  HAL_GPIO_WritePin(GPIOD, RI_HIGH_Pin|RI_LOW_Pin, GPIO_PIN_RESET);

//  /*Configure GPIO pin Output Level */
//  HAL_GPIO_WritePin(GPIOC, FR_HIGH_Pin|FR_LOW_Pin, GPIO_PIN_RESET);

//  /*Configure GPIO pins : LE_LOW_Pin LE_HIGH_Pin */
//  GPIO_InitStruct.Pin = LE_LOW_Pin|LE_HIGH_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

//  /*Configure GPIO pins : BA_HIGH_Pin BA_LOW_Pin */
//  GPIO_InitStruct.Pin = BA_HIGH_Pin|BA_LOW_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

//  /*Configure GPIO pins : RI_HIGH_Pin RI_LOW_Pin */
//  GPIO_InitStruct.Pin = RI_HIGH_Pin|RI_LOW_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

//  /*Configure GPIO pins : FR_HIGH_Pin FR_LOW_Pin */
//  GPIO_InitStruct.Pin = FR_HIGH_Pin|FR_LOW_Pin;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

///* USER CODE BEGIN MX_GPIO_Init_2 */
///* USER CODE END MX_GPIO_Init_2 */
//}
