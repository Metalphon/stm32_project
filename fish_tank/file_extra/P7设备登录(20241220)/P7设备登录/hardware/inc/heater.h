#ifndef __HEATER_H
#define __HEATER_H	 

#include "stm32f10x.h"


typedef struct
{

	_Bool Led1_Status;

} LED1_INFO;

#define LED1_ON	1

#define LED1_OFF	0

extern LED1_INFO led1_info;


void Led1_Init(void);

void Led1_Set(_Bool status);


#endif
