#include "ds18b20.h"
#include "tds.h"
#include "AD.h"

float TDS=0.0,TDS_voltage;
float TDS_value=0.0,voltage_value;
float TDStemp_data=0.0;
float TDScompensationCoefficient=1.0;//温度校准系数
float TDScompensationVolatge;
float TDSkValue=1.67;

//extern u16 bufadc[];

float TDS_Value_Conversion()
{
	
	TDStemp_data=DS18B20_Get_Temp()/10;
	
	TDS_voltage =(float) AD_Value[2]/4096*3.3; // 读取转换的AD值
	
	TDScompensationCoefficient=1.0+0.02*(TDStemp_data - 25.0); 
	
	TDScompensationVolatge=TDS_voltage/TDScompensationCoefficient;
	
	TDS_value=(66.71*TDScompensationVolatge*TDScompensationVolatge*TDScompensationVolatge - 
	127.93*TDScompensationVolatge*TDScompensationVolatge + 428.7*TDScompensationVolatge)*0.5*TDSkValue;
	
	if((TDS_value<=0)){TDS_value=0;}
	if((TDS_value>1000)){TDS_value=1000;}
		
	return TDS_value;

}
