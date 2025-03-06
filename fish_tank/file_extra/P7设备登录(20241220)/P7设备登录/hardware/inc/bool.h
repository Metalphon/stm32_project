#ifndef __BOOL_H
#define __BOOL_H	 
//#include "sys.h"

typedef struct
{

	_Bool Pump_Status;

} Pump_INFO;


#define Pump_ON	1
#define Pump_OFF	0



extern Pump_INFO pump_info;
//extern Pump2_INFO pump2_info;

void Pump_Init(void);

void Pump_Set(_Bool status);		
//void Pump2_Set(_Bool status);	

#endif
