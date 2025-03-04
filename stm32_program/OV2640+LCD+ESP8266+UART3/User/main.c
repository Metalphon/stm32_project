#include "stm32f4xx.h"
#include "led.h"
#include "SysTick.h"
#include "beep.h"
#include "exti.h"
#include "usart3.h"
#include "time4.h"
#include "pwm.h"
#include "l298n.h"
#include "lcd.h"
#include "ov2640.h" 
#include "dcmi.h" 
#include "usart4.h"

extern u8 ov_frame;  				//帧率
u8 Com4SendFlag;            //串口发送标记
u8 ov2640_mode=0;						//工作模式:0,RGB565模式;1,JPEG模式

#define jpeg_buf_size 31*1024  			//定义JPEG数据缓存jpeg_buf的大小(*4字节)
__align(4) u32 jpeg_buf[jpeg_buf_size];	//JPEG数据缓存buf

volatile u32 jpeg_data_len=0; 			//buf中的JPEG有效数据长度 
volatile u8 jpeg_data_ok=0;				//JPEG数据采集完成标志 
										//0,数据没有采集完;
										//1,数据采集完了,但是还没处理;
										//2,数据已经处理完成了,可以开始下一帧接收
//JPEG尺寸支持列表
const u16 jpeg_img_size_tbl[][2]=
{
	176,144,	//QCIF
	160,120,	//QQVGA
	352,288,	//CIF
	320,240,	//QVGA
	640,480,	//VGA
	800,600,	//SVGA
	1024,768,	//XGA
	1280,1024,	//SXGA
	1600,1200,	//UXGA
}; 
const u8*EFFECTS_TBL[7]={"Normal","Negative","B&W","Redish","Greenish","Bluish","Antique"};	//7种特效 
const u8*JPEG_SIZE_TBL[9]={"QCIF","QQVGA","CIF","QVGA","VGA","SVGA","XGA","SXGA","UXGA"};	//JPEG图片 9种尺寸 

//处理JPEG数据
//当采集完一帧JPEG数据后,调用此函数,切换JPEG BUF.开始下一帧采集.
void jpeg_data_process(void)
{
	if(ov2640_mode)//只有在JPEG格式下,才需要做处理.
	{
		if(jpeg_data_ok==0)	//jpeg数据还未采集完?
		{	
			DMA_Cmd(DMA2_Stream1, DISABLE);//停止当前传输 
			while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE){}// 等待DMA2_Stream1可配置  
			jpeg_data_len=jpeg_buf_size-DMA_GetCurrDataCounter(DMA2_Stream1);// 得到此次数据传输的长度
      
			jpeg_data_ok = 1;    // 数据已经采集完成，等待被处理
		}
		if(jpeg_data_ok==2)	//上一次的jpeg数据已经被处理了
		{			
			DMA2_Stream1->NDTR=jpeg_buf_size;	
			DMA_SetCurrDataCounter(DMA2_Stream1,jpeg_buf_size);//传输长度为jpeg_buf_size*4字节
			DMA_Cmd(DMA2_Stream1, ENABLE); //重新传输
			jpeg_data_ok=0;						//标记数据未采集
		}
	}
} 

//jpeg模式
void jpeg_test(void)
{ 
  u32 i,jpgstart,jpglen;
  u8 headok=0;	
	u8 *p;
	u8 effect=0,saturation=2,contrast=2;
	u8 size=3;		  
	u8 msgbuf[15];	//消息缓存区 
	
	//uart4初始化
	LTE_uart3_init(115200);
	LCD_Clear(WHITE);
  POINT_COLOR=RED; 
	LCD_DisplayString(30,60,24,"ALIENTEK STM32F4");
	LCD_DisplayString(30,90,24,"OV2640 JPEG Mode");
	
  OV2640_JPEG_Mode(); // JPEG模式
	
	My_DCMI_Init();			//DCMI配置
	DCMI_DMA_Init((u32)&jpeg_buf,jpeg_buf_size,DMA_MemoryDataSize_Word,DMA_MemoryInc_Enable);//DCMI配置 输出到数组
 	OV2640_OutSize_Set(jpeg_img_size_tbl[size][0], jpeg_img_size_tbl[size][1]); 
	DCMI_Start(); 		//启动传输  
	delay_ms(500);
	while(1)
	{
			if(jpeg_data_ok==1)  // 已经采集完成一帧图像，开始处理数据
		  {
					p=(u8*)jpeg_buf; 
					LCD_DisplayString(30,150,24,"Sending JPEG data...");
          jpglen=0;	//设置jpg文件大小为0
					headok=0;	//清除jpg头标记				
			    for(i=0; i<jpeg_data_len*4; i++)
          {
						//查找OXFF,OXD8和0XFF,0XD9，获取jpg文件大小
						if((p[i]==0XFF)&&(p[i+1]==0XD8)){ 
							  jpgstart=i;
							  headok=1;	//标记找到jpg头(FF D8)
						}
						if((p[i]==0XFF)&&(p[i+1]==0XD9)&&headok)//找到头以后,再找FF D9
						{
							  jpglen=i-jpgstart+2;
								break;
						}
          } 
					
					if(jpglen) // 正常的jpeg数据
					{
						 p+=jpgstart;
						 for(i=0;i<jpglen;i++)	//发送整个jpg文件
						 {
								while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	//循环发送,直到发送完毕  		
                USART_SendData(USART3, p[i]); 
						 }

					}
					jpeg_data_ok = 2;	//标记jpeg数据处理完了,可以让DMA去采集下一帧了.
					delay_ms(2000);
	   	}
	}
}



int main()
{
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	  SysTick_Init(168);
	  LED_Init();
	  KEYExti_Init();
	  /**LCD初始化**/
    LCD_Init();           //初始化LCD FSMC接口和显示驱动
    POINT_COLOR=RED;      //设置画笔颜色为红色
	  LCD_DisplayString(10,10,24,"OV2640 Start!");
    /**OV2640初始化**/
		while(OV2640_Init())//初始化OV2640
  	{
	  	LCD_DisplayString(30,130,24,"OV2640 ERR");
	  	delay_ms(200);
  	}
		
		LCD_DisplayString(30,130,24,"OV2640 OK");
	  delay_ms(200);
		
		//设置OV2640模式
		ov2640_mode=1;
		jpeg_test();
		 
}


