#ifndef __AD_H
#define __AD_H

extern uint16_t AD_Value[4];

void AD_Init(void);
uint16_t limitFilter(uint16_t currentData);
#endif
