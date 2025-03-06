#ifndef _BEEP_H_
#define _BEEP_H_

//单片机头文件
#include "stm32f10x.h"
typedef struct
{

	_Bool Beep_Status;

} Beep_INFO;

#define  BEEP_ON	1
#define BEEP_OFF	0

extern Beep_INFO beep_info;

void Beep_Init(void);

void Beep_Set(_Bool status);


#endif
