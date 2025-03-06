#ifndef _OXYGEN_H_
#define _OXYGEN_H_ 

#include "stm32f10x.h"


typedef struct
{

	_Bool Led2_Status;

} LED2_INFO;

#define LED2_ON	1

#define LED2_OFF	0

extern LED2_INFO led2_info;


void Led2_Init(void);

void Led2_Set(_Bool status);


#endif
