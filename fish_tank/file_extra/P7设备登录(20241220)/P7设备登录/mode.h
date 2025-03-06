

#ifndef _MODE_H_
#define _MODE_H_



#include "stm32f10x.h"


typedef struct
{

	_Bool Led3_Status;

} LED3_INFO;

#define LED3_ON		1

#define LED3_OFF	0

extern LED3_INFO led3_info;


void Led3_Init(void);

void Led3_Set(_Bool status);


#endif
