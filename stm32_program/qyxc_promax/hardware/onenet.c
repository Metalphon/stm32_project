//单片机头文件
#include "stm32h7xx.h"                  // Device header


//网络设备
#include "esp8266.h"

//协议文件
#include "onenet.h"
#include "mqttkit.h"
#include "cJSON.h"

//算法
#include "base64.h"
#include "hmac_sha1.h"

//硬件驱动
#include "usart.h"
#include "motor.h"
#include "adc.h"

//C库
#include <string.h>
#include <stdio.h>


#define PROID			"iC8Eh1sOD5" //产品ID

#define ACCESS_KEY		"Q2l1bDNhUkR5Y0U0WGd4VEtmcjlOOTNUZzNNb21wSk8="  //设备密钥

#define DEVICE_NAME		"qyxc"  //设备名称

int led_value=1;
int smog_value=10;
int speed_value=66;
double position_x=10.20212;
double position_y=20.6523;
double position_z=15.52544;
int sport_value;


//uint16_t speed;
int speedswitch;


char devid[16];

char key[48];


extern unsigned char esp8266_buf[512];


/*
************************************************************
*	函数名称：	OTA_UrlEncode
*
*	函数功能：	sign需要进行URL编码
*
*	入口参数：	sign：加密结果
*
*	返回参数：	0-成功	其他-失败
*
*	说明：		+			%2B
*				空格		%20
*				/			%2F
*				?			%3F
*				%			%25
*				#			%23
*				&			%26
*				=			%3D
************************************************************
*/
static unsigned char OTA_UrlEncode(char *sign)
{

	char sign_t[40];
	unsigned char i = 0, j = 0;
	unsigned char sign_len = strlen(sign);
	
	if(sign == (void *)0 || sign_len < 28)
		return 1;
	
	for(; i < sign_len; i++)
	{
		sign_t[i] = sign[i];
		sign[i] = 0;
	}
	sign_t[i] = 0;
	
	for(i = 0, j = 0; i < sign_len; i++)
	{
		switch(sign_t[i])
		{
			case '+':
				strcat(sign + j, "%2B");j += 3;
			break;
			
			case ' ':
				strcat(sign + j, "%20");j += 3;
			break;
			
			case '/':
				strcat(sign + j, "%2F");j += 3;
			break;
			
			case '?':
				strcat(sign + j, "%3F");j += 3;
			break;
			
			case '%':
				strcat(sign + j, "%25");j += 3;
			break;
			
			case '#':
				strcat(sign + j, "%23");j += 3;
			break;
			
			case '&':
				strcat(sign + j, "%26");j += 3;
			break;
			
			case '=':
				strcat(sign + j, "%3D");j += 3;
			break;
			
			default:
				sign[j] = sign_t[i];j++;
			break;
		}
	}
	
	sign[j] = 0;
	
	return 0;

}

/*
************************************************************
*	函数名称：	OTA_Authorization
*
*	函数功能：	计算Authorization
*
*	入口参数：	ver：参数组版本号，日期格式，目前仅支持格式"2018-10-31"
*				res：产品id
*				et：过期时间，UTC秒值
*				access_key：访问密钥
*				dev_name：设备名
*				authorization_buf：缓存token的指针
*				authorization_buf_len：缓存区长度(字节)
*
*	返回参数：	0-成功	其他-失败
*
*	说明：		当前仅支持sha1
************************************************************
*/
#define METHOD		"sha1"
static unsigned char OneNET_Authorization(char *ver, char *res, unsigned int et, char *access_key, char *dev_name,
											char *authorization_buf, unsigned short authorization_buf_len, _Bool flag)
{
	
	size_t olen = 0;
	
	char sign_buf[64];								//保存签名的Base64编码结果 和 URL编码结果
	char hmac_sha1_buf[64];							//保存签名
	char access_key_base64[64];						//保存access_key的Base64编码结合
	char string_for_signature[72];					//保存string_for_signature，这个是加密的key

//----------------------------------------------------参数合法性--------------------------------------------------------------------
	if(ver == (void *)0 || res == (void *)0 || et < 1564562581 || access_key == (void *)0
		|| authorization_buf == (void *)0 || authorization_buf_len < 120)
		return 1;
	
//----------------------------------------------------将access_key进行Base64解码----------------------------------------------------
	memset(access_key_base64, 0, sizeof(access_key_base64));
	BASE64_Decode((unsigned char *)access_key_base64, sizeof(access_key_base64), &olen, (unsigned char *)access_key, strlen(access_key));
//	HAL_UART_Printf(USART_DEBUG, "access_key_base64: %s\r\n", access_key_base64);
	
//----------------------------------------------------计算string_for_signature-----------------------------------------------------
	memset(string_for_signature, 0, sizeof(string_for_signature));
	if(flag)
		snprintf(string_for_signature, sizeof(string_for_signature), "%d\n%s\nproducts/%s\n%s", et, METHOD, res, ver);
	else
		snprintf(string_for_signature, sizeof(string_for_signature), "%d\n%s\nproducts/%s/devices/%s\n%s", et, METHOD, res, dev_name, ver);
//	HAL_UART_Printf(USART_DEBUG, "string_for_signature: %s\r\n", string_for_signature);
	
//----------------------------------------------------加密-------------------------------------------------------------------------
	memset(hmac_sha1_buf, 0, sizeof(hmac_sha1_buf));
	
	hmac_sha1((unsigned char *)access_key_base64, strlen(access_key_base64),
				(unsigned char *)string_for_signature, strlen(string_for_signature),
				(unsigned char *)hmac_sha1_buf);
	
//	HAL_UART_Printf(USART_DEBUG, "hmac_sha1_buf: %s\r\n", hmac_sha1_buf);
	
//----------------------------------------------------将加密结果进行Base64编码------------------------------------------------------
	olen = 0;
	memset(sign_buf, 0, sizeof(sign_buf));
	BASE64_Encode((unsigned char *)sign_buf, sizeof(sign_buf), &olen, (unsigned char *)hmac_sha1_buf, strlen(hmac_sha1_buf));

//----------------------------------------------------将Base64编码结果进行URL编码---------------------------------------------------
	OTA_UrlEncode(sign_buf);
//	HAL_UART_Printf(USART_DEBUG, "sign_buf: %s\r\n", sign_buf);
	
//----------------------------------------------------计算Token--------------------------------------------------------------------
	if(flag)
		snprintf(authorization_buf, authorization_buf_len, "version=%s&res=products%%2F%s&et=%d&method=%s&sign=%s", ver, res, et, METHOD, sign_buf);
	else
		snprintf(authorization_buf, authorization_buf_len, "version=%s&res=products%%2F%s%%2Fdevices%%2F%s&et=%d&method=%s&sign=%s", ver, res, dev_name, et, METHOD, sign_buf);
//	HAL_UART_Printf(USART_DEBUG, "Token: %s\r\n", authorization_buf);
	
	return 0;

}

//==========================================================
//	函数名称：	OneNET_RegisterDevice
//
//	函数功能：	在产品中注册一个设备
//
//	入口参数：	access_key：访问密钥
//				pro_id：产品ID
//				serial：唯一设备号
//				devid：保存返回的devid
//				key：保存返回的key
//
//	返回参数：	0-成功		1-失败
//
//	说明：		
//==========================================================
_Bool OneNET_RegisterDevice(void)
{

	_Bool result = 1;
	unsigned short send_len = 11 + strlen(DEVICE_NAME);
	char *send_ptr = NULL, *data_ptr = NULL;
	
	char authorization_buf[144];													//加密的key
	
	send_ptr = malloc(send_len + 240);
	if(send_ptr == NULL)
		return result;
	
	while(ESP8266_SendCmd("AT+CIPSTART=\"TCP\",\"183.230.40.33\",80\r\n", "CONNECT"))
		HAL_Delay(500);
	
	OneNET_Authorization("2018-10-31", PROID, 1965809117, ACCESS_KEY, NULL,
							authorization_buf, sizeof(authorization_buf), 1);
	
	snprintf(send_ptr, 240 + send_len, "POST /mqtt/v1/devices/reg HTTP/1.1\r\n"
					"Authorization:%s\r\n"
					"Host:ota.heclouds.com\r\n"
					"Content-Type:application/json\r\n"
					"Content-Length:%d\r\n\r\n"
					"{\"name\":\"%s\"}",
	
					authorization_buf, 11 + strlen(DEVICE_NAME), DEVICE_NAME);
	
	ESP8266_SendData((unsigned char *)send_ptr, strlen(send_ptr));
	
	/*
	{
	  "request_id" : "f55a5a37-36e4-43a6-905c-cc8f958437b0",
	  "code" : "onenet_common_success",
	  "code_no" : "000000",
	  "message" : null,
	  "data" : {
		"device_id" : "589804481",
		"name" : "mcu_id_43057127",
		
	"pid" : 282932,
		"key" : "indu/peTFlsgQGL060Gp7GhJOn9DnuRecadrybv9/XY="
	  }
	}
	*/
	
	data_ptr = (char *)ESP8266_GetIPD(250);							//等待平台响应
	
	if(data_ptr)
	{
		data_ptr = strstr(data_ptr, "device_id");
	}
	
	if(data_ptr)
	{
		char name[16];
		int pid = 0;
		
		if(sscanf(data_ptr, "device_id\" : \"%[^\"]\",\r\n\"name\" : \"%[^\"]\",\r\n\r\n\"pid\" : %d,\r\n\"key\" : \"%[^\"]\"", devid, name, &pid, key) == 4)
		{
			//HAL_UART_Printf(USART_DEBUG, "create device: %s, %s, %d, %s\r\n", devid, name, pid, key);
			result = 0;
		}
	}
	
	free(send_ptr);
	ESP8266_SendCmd("AT+CIPCLOSE\r\n", "OK");
	
	return result;

}

//==========================================================
//	函数名称：	OneNet_DevLink
//
//	函数功能：	与onenet创建连接
//
//	入口参数：	无
//
//	返回参数：	1-成功	0-失败
//
//	说明：		与onenet平台建立连接
//==========================================================
_Bool OneNet_DevLink(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};					//协议包

	unsigned char *dataPtr;
	
	char authorization_buf[160];
	
	_Bool status = 1;
	
	OneNET_Authorization("2018-10-31", PROID, 1956499200, ACCESS_KEY, DEVICE_NAME,
								authorization_buf, sizeof(authorization_buf), 0);
	
	HAL_UART_Printf(USART_DEBUG, "OneNET_DevLink\r\n"
							"NAME: %s,	PROID: %s,	KEY:%s\r\n"
                        , DEVICE_NAME, PROID, authorization_buf);
	
	if(MQTT_PacketConnect(PROID, authorization_buf, DEVICE_NAME, 1024, 1, MQTT_QOS_LEVEL0, NULL, NULL, 0, &mqttPacket) == 0)
	{
		ESP8266_SendData(mqttPacket._data, mqttPacket._len);			//上传平台
		
		dataPtr = ESP8266_GetIPD(250);									//等待平台响应
		if(dataPtr != NULL)
		{
			if(MQTT_UnPacketRecv(dataPtr) == MQTT_PKT_CONNACK)
			{
				switch(MQTT_UnPacketConnectAck(dataPtr))
				{
					case 0:HAL_UART_Printf(USART_DEBUG, "Tips:	连接成功\r\n");status = 0;break;
					
					case 1:HAL_UART_Printf(USART_DEBUG, "WARN:	连接失败：协议错误\r\n");break;
					case 2:HAL_UART_Printf(USART_DEBUG, "WARN:	连接失败：非法的clientid\r\n");break;
					case 3:HAL_UART_Printf(USART_DEBUG, "WARN:	连接失败：服务器失败\r\n");break;
					case 4:HAL_UART_Printf(USART_DEBUG, "WARN:	连接失败：用户名或密码错误\r\n");break;
					case 5:HAL_UART_Printf(USART_DEBUG, "WARN:	连接失败：非法链接(比如token非法)\r\n");break;
					
					default:HAL_UART_Printf(USART_DEBUG, "ERR:	连接失败：未知错误\r\n");break;
				}
			}
		}
		
		MQTT_DeleteBuffer(&mqttPacket);								//删包
	}
	else
		HAL_UART_Printf(USART_DEBUG, "WARN:	MQTT_PacketConnect Failed\r\n");
	
	return status;
	
}

unsigned char OneNet_FillBuf(char *buf)
{
	
	char text[256];
	
		memset(text, 0, sizeof(text));
	//adc采集函数
//	  smog_value = alchol_meas(smog_value);
//   if(smog_value>800)
//  	smog_value=smog_value-800;
	smog_value = ADC_Read()/100;
//		HAL_UART_Printf(USART_DEBUG, "666666666666666666\r\n");
//		HAL_UART_Printf(USART_DEBUG, "%d\r\n",smog_value);
//		HAL_UART_Printf(USART_DEBUG, "%d\r\n",qyvalue);

		strcpy(buf, "{\"id\":\"123\",\"params\":{");



		memset(text, 0, sizeof(text));
		sprintf(text, "\"smog\":{\"value\":%d},", smog_value);
		strcat(buf, text);

		memset(text, 0, sizeof(text));
		sprintf(text, "\"qyvalue\":{\"value\":%d}",qyvalue);
		strcat(buf, text);
	

	
	strcat(buf, "}}");
	
	return strlen(buf);

}

//==========================================================
//	函数名称：	OneNet_SendData
//
//	函数功能：	上传数据到平台
//
//	入口参数：	type：发送数据的格式
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OneNet_SendData(void)
{
	
	MQTT_PACKET_STRUCTURE mqttPacket = {NULL, 0, 0, 0};												//协议包
	
	char buf[256];
	
	short body_len = 0, i = 0;
	
	//HAL_UART_Printf(USART_DEBUG, "Tips:	OneNet_SendData-MQTT\r\n");
	
	memset(buf, 0, sizeof(buf));
	
	body_len = OneNet_FillBuf(buf);																	//获取当前需要发送的数据流的总长度
	
	if(body_len)
	{
		if(MQTT_PacketSaveData(PROID, DEVICE_NAME, body_len, NULL, &mqttPacket) == 0)				//封包
		{
			for(; i < body_len; i++)
				mqttPacket._data[mqttPacket._len++] = buf[i];
			
			ESP8266_SendData(mqttPacket._data, mqttPacket._len);									//上传数据到平台
			//HAL_UART_Printf(USART_DEBUG, "Send %d Bytes\r\n", mqttPacket._len);
			
			MQTT_DeleteBuffer(&mqttPacket);															//删包
		}
		else
			HAL_UART_Printf(USART_DEBUG, "WARN:	EDP_NewBuffer Failed\r\n");
	}
	
}

//==========================================================
//	函数名称：	OneNET_Publish
//
//	函数功能：	发布消息
//
//	入口参数：	topic：发布的主题
//				msg：消息内容
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OneNET_Publish(const char *topic, const char *msg)
{

	MQTT_PACKET_STRUCTURE mqtt_packet = {NULL, 0, 0, 0};						//协议包
	
	HAL_UART_Printf(USART_DEBUG, "Publish Topic: %s, Msg: %s\r\n", topic, msg);
	
	if(MQTT_PacketPublish(MQTT_PUBLISH_ID, topic, msg, strlen(msg), MQTT_QOS_LEVEL0, 0, 1, &mqtt_packet) == 0)
	{
		ESP8266_SendData(mqtt_packet._data, mqtt_packet._len);					//向平台发送订阅请求
		
		MQTT_DeleteBuffer(&mqtt_packet);										//删包
	}

}

//==========================================================
//	函数名称：	OneNET_Subscribe
//
//	函数功能：	订阅
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void OneNET_Subscribe(void)
{
	
	MQTT_PACKET_STRUCTURE mqtt_packet = {NULL, 0, 0, 0};						//协议包
	
	char topic_buf[56];
	const char *topic = topic_buf;
	
	snprintf(topic_buf, sizeof(topic_buf), "$sys/%s/%s/thing/property/set", PROID, DEVICE_NAME);
	
	HAL_UART_Printf(USART_DEBUG, "Subscribe Topic: %s\r\n", topic_buf);
	
	if(MQTT_PacketSubscribe(MQTT_SUBSCRIBE_ID, MQTT_QOS_LEVEL0, &topic, 1, &mqtt_packet) == 0)
	{
		ESP8266_SendData(mqtt_packet._data, mqtt_packet._len);					//向平台发送订阅请求
			HAL_UART_Printf(USART_DEBUG, "Subscribe ok\r\n");

		MQTT_DeleteBuffer(&mqtt_packet);										//删包
	}

}

//==========================================================
//	函数名称：	OneNet_RevPro
//
//	函数功能：	平台返回数据检测
//
//	入口参数：	dataPtr：平台返回的数据
//
//	返回参数：	无
//
//	说明：		
//==========================================================


void OneNet_RevPro(unsigned char *cmd)
{
	
	char *req_payload = NULL;
	char *cmdid_topic = NULL;
	
	unsigned short topic_len = 0;
	unsigned short req_len = 0;
	
	unsigned char qos = 0;
	static unsigned short pkt_id = 0;
	
	unsigned char type = 0;
	
	short result = 0;

	char *dataPtr = NULL;
	char numBuf[10];
	int num = 0;
	
		cJSON *raw_json, *params_json, *sport_json, *speed_json, *led_json, *smog_json;

	type = MQTT_UnPacketRecv(cmd);
	
	switch(type)
	{
		case MQTT_PKT_PUBLISH:																//接收的Publish消息
		
			result = MQTT_UnPacketPublish(cmd, &cmdid_topic, &topic_len, &req_payload, &req_len, &qos, &pkt_id);
				  //  req_payload    消息体
						HAL_UART_Printf(USART_DEBUG, "Tips:  %s\r\n",req_payload);
			if(result == 0)
			{
				char *data_ptr = NULL;
				
			//	HAL_UART_Printf(USART_DEBUG, "topic: %s, topic_len: %d, payload: %s, payload_len: %d\r\n",
				//													cmdid_topic, topic_len, req_payload, req_len);
				
				 //  	HAL_UART_Printf(USART_DEBUG, "aaaaaaaaaa\r\n");

			//	data_ptr = strstr(cmdid_topic, "request/");									//查找cmdid
				   	HAL_UART_Printf(USART_DEBUG, "bbbbbbbbbbbbb\r\n");

//				if(data_ptr)
//				{
//					char topic_buf[80], cmdid[40];
//					
//					data_ptr = strchr(data_ptr, '/');
//					data_ptr++;
//					
//					memcpy(cmdid, data_ptr, 36);											//复制cmdid
//					cmdid[36] = 0;
//					
//					snprintf(topic_buf, sizeof(topic_buf), "$sys/%s/%s/cmd/response/%s",
//															PROID, DEVICE_NAME, cmdid);
//					OneNET_Publish(topic_buf, "ojbk");										//回复命令
//				}
//				else
//				{HAL_UART_Printf(USART_DEBUG, "aaaaaaaaaa\r\n");}
				//HAL_UART_Printf(USART_DEBUG, "aaaaaaaaaa\r\n");
				 //  	HAL_UART_Transmit(USART_DEBUG, "c\r\n",3,HAL_MAX_DELAY);
//if(raw_json == NULL) {
//   	HAL_UART_Printf(USART_DEBUG, "aaaaaaaaaa\r\n");

//}
//else

  	HAL_UART_Printf(USART_DEBUG, "dddddddddddd\r\n");

			enum sportState {
			sport_front = 0,
			sport_back = 1,
			sport_left = 2,
			sport_right = 3,
			sport_stop = 4,
			qy_up = 5,
			qydirection_right = 6,
			qydirection_lift = 7,
			qyoff = 8,
			qy_down = 9,
			qy_front=10,
			qy_back=11,
			speedlow=12,
			speedmiddle=13,
			speedhight=14,

};
	HAL_UART_Printf(USART_DEBUG, "1\r\n");
		    raw_json = cJSON_Parse(req_payload);
	HAL_UART_Printf(USART_DEBUG, "2\r\n");
				params_json = cJSON_GetObjectItem(raw_json,"params");
	HAL_UART_Printf(USART_DEBUG, "3\r\n");
				sport_json = cJSON_GetObjectItem(params_json,"direction");
	HAL_UART_Printf(USART_DEBUG, "4\r\n");
//  		  speed_json = cJSON_GetObjectItem(params_json,"speed");
//  		  led_json = cJSON_GetObjectItem(params_json,"led");
//  		  smog_json = cJSON_GetObjectItem(params_json,"smog");
//				led_value = led_json->valueint;    
//				smog_value = smog_json->valueint;    
//        speed_value = speed_json->valueint;                      //速度值
				sport_value = sport_json->valueint;    


//				HAL_UART_Printf(USART_DEBUG, "speed_value：%d\r\n",speed_value);
//				HAL_UART_Printf(USART_DEBUG, "smog_value：%d\r\n",smog_value);
//				HAL_UART_Printf(USART_DEBUG, "led_value：%d\r\n",led_value);

uint16_t a=10;
   // 运行方向等函数 
switch (sport_value) {
    case sport_front:
			sportfront(speed);
		
		HAL_UART_Printf(USART_DEBUG, "sport_front\r\n");

        break;
    case sport_back:
			sportback(speed);
		
     HAL_UART_Printf(USART_DEBUG, "sport_back\r\n");

        break;
    case sport_right:
     sportright(speed);

    HAL_UART_Printf(USART_DEBUG, "sport_right\r\n");
        break;
    case sport_left:
			
		sportleft(speed);
    HAL_UART_Printf(USART_DEBUG, "sport_left\r\n");
        break;
    case sport_stop:
      sportstop();

    HAL_UART_Printf(USART_DEBUG, "sport_stop\r\n");
        break;
		case qy_up:
      qyup();	
		HAL_UART_Printf(USART_DEBUG, "up\r\n");
        break;
		case qydirection_right:
    qyright();
    HAL_UART_Printf(USART_DEBUG, "qyright\r\n");
        break;
	  case qydirection_lift:
		qyleft();
    HAL_UART_Printf(USART_DEBUG, "qyleft\r\n");
        break;
		case qyoff:
	//	qy_off();
		zd();
    	HAL_UART_Printf(USART_DEBUG, "qy_off\r\n");
        break;
		case qy_down:
		qydown();
                			HAL_UART_Printf(USART_DEBUG, "qydown\r\n");
        break;
		
				case qy_front:
		qyfront();
                			HAL_UART_Printf(USART_DEBUG, "qyfront\r\n");
        break;
				
						case qy_back:
		qyback();
                			HAL_UART_Printf(USART_DEBUG, "qyback\r\n");
        break;
						
								case speedlow:
		speed_low();
                			HAL_UART_Printf(USART_DEBUG, "speed_low\r\n");
        break;
								
										case speedmiddle:
		speed_middle();
                			HAL_UART_Printf(USART_DEBUG, "speed_middle\r\n");
        break;
										
												case speedhight:
		speed_hight();
                			HAL_UART_Printf(USART_DEBUG, "speed_hight\r\n");
        break;
												
    default:
                			HAL_UART_Printf(USART_DEBUG, "default\r\n");
        break;
}
				cJSON_Delete(raw_json);//释放cjson

				
		
}

		case MQTT_PKT_PUBACK:														//发送Publish消息，平台回复的Ack
		
			if(MQTT_UnPacketPublishAck(cmd) == 0)
				HAL_UART_Printf(USART_DEBUG, "Tips:	MQTT Publish Send OK\r\n");
			
		break;
		
		case MQTT_PKT_SUBACK:																//发送Subscribe消息的Ack
		
			if(MQTT_UnPacketSubscribe(cmd) == 0)
				HAL_UART_Printf(USART_DEBUG, "Tips:	MQTT Subscribe OK\r\n");
			else
				HAL_UART_Printf(USART_DEBUG, "Tips:	MQTT Subscribe Err\r\n");
		
		break;
		
		default:
			result = -1;
		break;
	}
	
	ESP8266_Clear();									//清空缓存
	
	if(result == -1)
		return;
	
	if(type == MQTT_PKT_CMD || type == MQTT_PKT_PUBLISH)
	{
		MQTT_FreeBuffer(cmdid_topic);
		MQTT_FreeBuffer(req_payload);
	}
}




