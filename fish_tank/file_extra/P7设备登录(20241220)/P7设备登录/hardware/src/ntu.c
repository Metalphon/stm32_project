#include "ds18b20.h"
#include "ntu.h"
#include "AD.h"

float TU=0.0;
short TU_value=0.0;
float TU_calibration=0.0;//�¶�У׼ϵ��
float temp_data=0.0;


float TU_Value_Conversion()
{

	  temp_data=DS18B20_Get_Temp();
	    TU =(float) AD_Value[3]/4095*3.3; // ��ȡת����ADֵ
		TU_value =100 - (TU*100/3.3);

	
		return TU_value;
		
}



