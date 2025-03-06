#ifndef __KEY_H
#define __KEY_H

extern u8 state;

void Key_Init(void);
uint8_t Key_GetNum(void);
//uint8_t Key_Turn1(void);
//uint8_t Key_Turn2(void);
//uint8_t Key_Turn3(void);
//uint8_t Key_Turn4(void);
uint8_t Next_page(void);
//void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);

#endif
