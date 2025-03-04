#ifndef INC_CONTROLLER_INTI_C_
#define INC_CONTROLLER_INTI_C_
#include "main.h"

void cot_init();
//void HAL_UART_RxCpltCallback();
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void A_type(uint8_t *A_data);
void B_type(uint8_t *B_data);
void C_type(uint8_t *C_data);


extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim23;
extern uint8_t detect[1];
extern uint8_t data2[9];

#endif /* INC_CONTROLLER_INTI_C_ */
