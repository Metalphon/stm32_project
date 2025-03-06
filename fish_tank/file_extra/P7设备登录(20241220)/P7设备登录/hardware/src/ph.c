#include "ds18b20.h"
#include "ph.h"
#include "AD.h"

float ADC_ConvertedValueLocal;
float PH_Value=0;




float PH_Value_Conversion()
{
	ADC_ConvertedValueLocal = (float)AD_Value[1]*(3.3/4096); // 读取转换的AD值
	
	PH_Value=-5.7541*ADC_ConvertedValueLocal+16.654;

	
		return AD_Value[1];
		
}



