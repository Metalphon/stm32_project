/**
	************************************************************
	************************************************************
	************************************************************
	*	文件名： 	main.c
	*
	*	作者： 		张继瑞
	*
	*	日期： 		2017-05-08
	*
	*	版本： 		V1.0
	*
	*	说明： 		接入onenet，上传数据和命令控制
	*
	*	修改记录：	
	************************************************************
	************************************************************
	************************************************************
**/
/*
超声波HCSR04 PB5 PB6
浑浊度NTU    PA7
水质TDS     PA4
温度temperature  PB7

KEY          PB10-PB14
电机喂食led  PC13
水泵bool     PA0
增氧泵Oxygen PA11
加热棒hester PA12
切屏         PA14
*/
//单片机头文件
#include "stm32f10x.h"

//网络协议层
#include "onenet.h"

//网络设备
#include "esp8266.h"

//硬件驱动
#include "delay.h"
#include "usart.h"
//#include "led.h"
#include "LED.h"
#include "key.h"
#include "dht11.h"
#include "oled.h"
#include "timforsend.h"

#include "AD.h"
#include "HCSR04.h"
//#include "tds.h"
//#include "ntu.h"
//#include "ph.h"
#include "ds18b20.h"
#include "bool.h"
#include "heater.h"
#include "Oxygen.h"
#include "Timer.h"
#include "MyTimer.h"
//#include "fan.h"
#include "mode.h"
#include "beep.h"

#include <string.h>
#include <stdio.h>
#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"
// 定义允许的最大差值，这里假设为10，你可根据实际情况调整
#define MAX_DIFF_TDS 5 
#define MAX_DIFF_PH 0.3
// 用于存储上一次滤波后TDS的数据，初始化为一个合适的值，比如0
float lastFilteredTDSData = 0; 
float lastFilteredPHData = 0; 

	void Hardware_Init(void);
	void Math_data(void);
	void flag_data(void);
	
	void Auto1_data(void);
	void Auto2_data(void);
	void Refresh_Data(void);
//	void Math_data(void);
	float limitFilterTDS(float currentTDSData, float *lastFilteredTDS);
	float limitFilterPH(float currentPHData, float *lastFilteredPH) ;
float ph_k = 0.01;
	float TU=0.0;
float TU_value=0.0;
float TU_value1=0.0;
float TU_calibration=0.0;
float temp_data=0.0;
float K_Value=2763.19;
float ADC_ConvertedValueLocal;
float PH_Value=0;
float TDS=0.0,TDS_voltage;
float TDS_value=0.0,voltage_value;
float TDStemp_data=0.0;
float TDScompensationCoefficient=1.0;//温度校准系数
float TDScompensationVolatge;
float TDSkValue=0.9;
  u8 buff[2],p[2],t[2],l[2],n[2];
	float temperature;
  float NTU;        //浑浊度
	extern float TDS;        //TDS
	float PH;        //PH
	float Level;
	uint8_t page = 1;
 float tempflag=18; 
 float hightflag=6; //ph
 float tdsflag=100;
 float ntuflag=0;
 float tempflag_h=25; 
 float hightflag_h=8; //ph
 float tdsflag_h=350;
 float ntuflag_h=20;
	extern unsigned short timeCount = 0;	//发送间隔变量

/*
************************************************************
*	函数名称：	main
*
*	函数功能：	
*
*	入口参数：	无
*
*	返回参数：	0
*
*	说明：		
************************************************************
*/

int main(void)
{
	
//	unsigned short timeCount = 0;	//发送间隔变量
	unsigned char *dataPtr = NULL;
UsartPrintf(USART_DEBUG, "999\r\n");
	Hardware_Init();				//初始化外围硬件
	
	ESP8266_Init();					//初始化ESP8266	
	Timer_Init();
	//Timer4_Init();
	HC_SR04_Init();
	AD_Init();
	DS18B20_Init();
  Key_Init();	
	Led_Init();
	Led1_Init();
	Led2_Init();
	Led3_Init();
	Pump_Init();

	OLED_Clear(); 
			

	OLED_ShowString(0,0,"Connect MQTTs Server...",16); //%
	while(ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
		DelayXms(1000);
	UsartPrintf(USART_DEBUG, "Connect MQTT Server Success\r\n");
	OLED_ShowString(0,0,"Connect MQTT Server Success",16);
		DelayXms(1000);
	OLED_Clear();
	while(OneNet_DevLink())			//接入OneNET
	{
		ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT");
		DelayXms(1000);
	}
		
    OneNET_Subscribe();//订阅主题函数
	
	while(1)
	{
//			UsartPrintf(USART_DEBUG, "AD_Value[1]%f\r\n",AD_Value[1]);
//		UsartPrintf(USART_DEBUG, "AD_Value[3]%f\r\n",AD_Value[3]);
//		UsartPrintf(USART_DEBUG, "AD_Value[2]%f\r\n",AD_Value[2]);
/*//			temperature = DS18B20_Get_Temp();
//		
//		sprintf((char*)buff, "%2.1f", temperature/10);
//		  NTU=TU_Value_Conversion();
//		sprintf((char*)p, "%2.1f", NTU);
//			TDS=TDS_Value_Conversion();	
//		sprintf((char*)t, "%2.1f", TDS);
//			Level = HSCR_Value_Conversion();
//		sprintf((char*)l, "%2.1f", Level);*/

		if(++timeCount >= 50)									//发送间隔5s
		{
			Math_data();
			temperature = DS18B20_Get_Temp()/10;
		sprintf((char*)buff, "%2.1f", temperature);
		  NTU=TU_value;
		sprintf((char*)p, "%2.1f", NTU);
			TDS=TDS_value;	
		sprintf((char*)t, "%2.1f", TDS);
		  PH=PH_Value;	
		sprintf((char*)n, "%2.1f", PH);
			Level = HSCR_Value_Conversion();
		sprintf((char*)l, "%2.1f", Level);
			
			OneNet_SendData();									//发送数据，进入定义界面
		UsartPrintf(USART_DEBUG, "%2.1f\r\n",PH);
			
			timeCount = 0;
			ESP8266_Clear();
		}//上报数据
		
			dataPtr = ESP8266_GetIPD(0);//获取平台返回数据
			if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);//接收数据
			Refresh_Data();//调用
			DelayXms(10);

	}

}

/*
************************************************************
*	函数名称：	Hardware_Init
*
*	函数功能：	硬件初始化
*
*	入口参数：	无
*
*	返回参数：	无
*
*	说明：		初始化单片机功能以及外接设备
************************************************************
*/
void Hardware_Init(void)
{
	
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//中断控制器分组设置

		Delay_Init();									//systick初始化
		
		Usart1_Init(115200);							//串口1，打印信息用
		
		Usart2_Init(115200);							//串口2，驱动ESP8266用
		
		Key_Init();
	
		Led_Init();									//蜂鸣器初始化
	  //Oxygen_Init();
	 // Heater_Init();
//	  Pump_Init();
//		Servo_Init();
		OLED_Init();			//初始化OLED  

	  OLED_Clear(); 
		OLED_ShowString(0,0,"Hardware init OK",16); 
	DelayMs(1000);
}



//采集参数
void Refresh_Data(void)
{
		
			if(state == 0)
			{
				Auto1_data();
				Auto2_data();
				
			}
flag_data();
//三个页面切换，第三面未写，可拓展
if(Next_page() == 1){
			page++;
			OLED_Clear();
		}

		if(page>3){
				page = 1;
		}
		switch(page){
			case 1 :
	if(led3_info.Led3_Status)	
			{
			OLED_ShowCHinese(57,2,21);//手
				OLED_ShowCHinese(72,2,20);//动
			}
			
			else 
			{
					OLED_ShowCHinese(57,2,19);//自
				OLED_ShowCHinese(72,2,20);//动
			}
			OLED_ShowCHinese(0,6,1);//温
		  OLED_ShowCHinese(18,6,2);//度
      OLED_ShowCHinese(36,6,0);//:
			
		OLED_ShowCHinese(0,4,6);
			OLED_ShowCHinese(18,4,7);
		OLED_ShowCHinese(36,4,0);//:
			
				OLED_ShowCHinese(0,2,17);//模
		  OLED_ShowCHinese(18,2,18);//式
      OLED_ShowCHinese(36,2,0);//:
			

			OLED_ShowCHinese(26,0,27);//:
		OLED_ShowCHinese(44,0,28);//:	
		OLED_ShowCHinese(62,0,29);//:
		OLED_ShowCHinese(80,0,30);//:	

			temperature = DS18B20_Get_Temp()/10;
			sprintf((char*)buff, "%2.1f", temperature);
				OLED_ShowString(62,6,buff,16); 
	
				Level = HSCR_Value_Conversion();	
	DelayXms(100);
			sprintf((char*)l, "%2.1f",Level);
				OLED_ShowString(62,4,l,16);

				break;
				case 2 :

				OLED_ShowChar(8,0,'P',16);
					OLED_ShowChar(18,0,'H',16);

		OLED_ShowCHinese(36,0,0);//:	
				
		OLED_ShowCHinese(0,4,4);//浊
		OLED_ShowCHinese(18,4,5);//度	
		OLED_ShowCHinese(36,4,0);//:
		
    OLED_ShowCHinese(0,2,6);//水	
			OLED_ShowCHinese(18,2,8);//质
		OLED_ShowCHinese(36,2,0);//:	
				
				PH=PH_Value;	
	DelayXms(100);
			sprintf((char*)n, "%2.1f",PH);
				OLED_ShowString(62,0,n,16); 
				
		TDS=TDS_value;	
	DelayXms(100);
			sprintf((char*)t, "%2.1f",TDS);
				OLED_ShowString(62,2,t,16);			
			
					break;
			case 3 :
	OLED_ShowCHinese(0,6,10);//氧
		  OLED_ShowCHinese(18,6,9);//泵
      OLED_ShowCHinese(36,6,0);//:
		
		OLED_ShowCHinese(0,4,15);//加
		OLED_ShowCHinese(18,4,16);//热	
		OLED_ShowCHinese(36,4,0);//:
		
    OLED_ShowCHinese(0,2,11);//喂	
			OLED_ShowCHinese(18,2,12);//食
		OLED_ShowCHinese(36,2,0);//:	

		OLED_ShowCHinese(0,0,6);
			OLED_ShowCHinese(18,0,9);
		OLED_ShowCHinese(36,0,0);//:
				
			if(pump_info.Pump_Status)	OLED_ShowCHinese(57,0,13);//关
			else OLED_ShowCHinese(57,0,14);//开
		
      if(led_info.Led_Status)OLED_ShowCHinese(57,2,13);//关
			else 
			OLED_ShowCHinese(57,2,14);//开
			
			if(led1_info.Led1_Status)OLED_ShowCHinese(57,4,13);//关
			else 
	  	OLED_ShowCHinese(57,4,14);//开
			
			if(led2_info.Led2_Status)OLED_ShowCHinese(57,6,13);//关;
			else 		
			OLED_ShowCHinese(57,6,14);//开;	

				break;
				
		}

}

void flag_data(void)//阈值标志
{
		if(temperature<tempflag || PH<hightflag || TDS<tdsflag || temperature>tempflag_h || PH>hightflag_h || TDS>tdsflag_h)
		{
				DelayXms(200);
			Beep_Set(BEEP_ON);	
		}
		if(temperature>tempflag && PH<=hightflag && TDS<=tdsflag)

		{		DelayXms(200);
				Beep_Set(BEEP_OFF);
		}
}

void Auto1_data(void)//温度过低开启加热棒
{
	if(temperature<tempflag)Led2_Set(LED2_ON);			
	
	else Led2_Set(LED2_OFF);	
	//Led1_Set(LED1_ON);//
}

void Auto2_data(void)//浑浊度或水质过高，进行水循环
{
	if(TDS>tdsflag_h || PH>hightflag_h)
	{Pump_Set(Pump_ON);}		
	
	else {Pump_Set(Pump_OFF);	}
}

void Math_data(void)
{
	 TDS_voltage =(float) AD_Value[2]/4096*3.3; // 读取转换的AD值
    TDScompensationCoefficient=1.0+0.02*(temperature - 25.0); 
    TDScompensationVolatge=TDS_voltage/TDScompensationCoefficient;
    TDS_value=(66.71*TDScompensationVolatge*TDScompensationVolatge*TDScompensationVolatge - 
               127.93*TDScompensationVolatge*TDScompensationVolatge + 428.7*TDScompensationVolatge)*0.5*TDSkValue;

    TDS_value = limitFilterTDS(TDS_value, &lastFilteredTDSData);
    ADC_ConvertedValueLocal = (float)AD_Value[1]*(3.3/4095); 
    PH_Value=-6.5*ADC_ConvertedValueLocal+17.5;
    PH_Value = PH_Value + ph_k * (temperature - 25.0);
    PH_Value = limitFilterPH(PH_Value, &lastFilteredPHData);
    if(PH_Value>=7.5)
    {
        PH_Value = PH_Value-0.5;
    }
}
// 限幅滤波函数，针对TDS数据进行限幅滤波处理
float limitFilterTDS(float currentTDSData, float *lastFilteredTDS) {
    if (fabs(currentTDSData - *lastFilteredTDS) > MAX_DIFF_TDS) {
        return *lastFilteredTDS;
    } else {
        *lastFilteredTDS = currentTDSData;
        return currentTDSData;
    }
}
float limitFilterPH(float currentPHData, float *lastFilteredPH) {
    if (fabs(currentPHData - *lastFilteredPH) > MAX_DIFF_PH) {
        return *lastFilteredPH;
    } else {
        *lastFilteredPH = currentPHData;
        return currentPHData;
    }
}