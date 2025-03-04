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

extern u8 ov_frame;  				//֡��
u8 Com4SendFlag;            //���ڷ��ͱ��
u8 ov2640_mode=0;						//����ģʽ:0,RGB565ģʽ;1,JPEGģʽ

#define jpeg_buf_size 31*1024  			//����JPEG���ݻ���jpeg_buf�Ĵ�С(*4�ֽ�)
__align(4) u32 jpeg_buf[jpeg_buf_size];	//JPEG���ݻ���buf

volatile u32 jpeg_data_len=0; 			//buf�е�JPEG��Ч���ݳ��� 
volatile u8 jpeg_data_ok=0;				//JPEG���ݲɼ���ɱ�־ 
										//0,����û�вɼ���;
										//1,���ݲɼ�����,���ǻ�û����;
										//2,�����Ѿ����������,���Կ�ʼ��һ֡����
//JPEG�ߴ�֧���б�
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
const u8*EFFECTS_TBL[7]={"Normal","Negative","B&W","Redish","Greenish","Bluish","Antique"};	//7����Ч 
const u8*JPEG_SIZE_TBL[9]={"QCIF","QQVGA","CIF","QVGA","VGA","SVGA","XGA","SXGA","UXGA"};	//JPEGͼƬ 9�ֳߴ� 

//����JPEG����
//���ɼ���һ֡JPEG���ݺ�,���ô˺���,�л�JPEG BUF.��ʼ��һ֡�ɼ�.
void jpeg_data_process(void)
{
	if(ov2640_mode)//ֻ����JPEG��ʽ��,����Ҫ������.
	{
		if(jpeg_data_ok==0)	//jpeg���ݻ�δ�ɼ���?
		{	
			DMA_Cmd(DMA2_Stream1, DISABLE);//ֹͣ��ǰ���� 
			while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE){}// �ȴ�DMA2_Stream1������  
			jpeg_data_len=jpeg_buf_size-DMA_GetCurrDataCounter(DMA2_Stream1);// �õ��˴����ݴ���ĳ���
      
			jpeg_data_ok = 1;    // �����Ѿ��ɼ���ɣ��ȴ�������
		}
		if(jpeg_data_ok==2)	//��һ�ε�jpeg�����Ѿ���������
		{			
			DMA2_Stream1->NDTR=jpeg_buf_size;	
			DMA_SetCurrDataCounter(DMA2_Stream1,jpeg_buf_size);//���䳤��Ϊjpeg_buf_size*4�ֽ�
			DMA_Cmd(DMA2_Stream1, ENABLE); //���´���
			jpeg_data_ok=0;						//�������δ�ɼ�
		}
	}
} 

//jpegģʽ
void jpeg_test(void)
{ 
  u32 i,jpgstart,jpglen;
  u8 headok=0;	
	u8 *p;
	u8 effect=0,saturation=2,contrast=2;
	u8 size=3;		  
	u8 msgbuf[15];	//��Ϣ������ 
	
	//uart4��ʼ��
	LTE_uart3_init(115200);
	LCD_Clear(WHITE);
  POINT_COLOR=RED; 
	LCD_DisplayString(30,60,24,"ALIENTEK STM32F4");
	LCD_DisplayString(30,90,24,"OV2640 JPEG Mode");
	
  OV2640_JPEG_Mode(); // JPEGģʽ
	
	My_DCMI_Init();			//DCMI����
	DCMI_DMA_Init((u32)&jpeg_buf,jpeg_buf_size,DMA_MemoryDataSize_Word,DMA_MemoryInc_Enable);//DCMI���� ���������
 	OV2640_OutSize_Set(jpeg_img_size_tbl[size][0], jpeg_img_size_tbl[size][1]); 
	DCMI_Start(); 		//��������  
	delay_ms(500);
	while(1)
	{
			if(jpeg_data_ok==1)  // �Ѿ��ɼ����һ֡ͼ�񣬿�ʼ��������
		  {
					p=(u8*)jpeg_buf; 
					LCD_DisplayString(30,150,24,"Sending JPEG data...");
          jpglen=0;	//����jpg�ļ���СΪ0
					headok=0;	//���jpgͷ���				
			    for(i=0; i<jpeg_data_len*4; i++)
          {
						//����OXFF,OXD8��0XFF,0XD9����ȡjpg�ļ���С
						if((p[i]==0XFF)&&(p[i+1]==0XD8)){ 
							  jpgstart=i;
							  headok=1;	//����ҵ�jpgͷ(FF D8)
						}
						if((p[i]==0XFF)&&(p[i+1]==0XD9)&&headok)//�ҵ�ͷ�Ժ�,����FF D9
						{
							  jpglen=i-jpgstart+2;
								break;
						}
          } 
					
					if(jpglen) // ������jpeg����
					{
						 p+=jpgstart;
						 for(i=0;i<jpglen;i++)	//��������jpg�ļ�
						 {
								while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);	//ѭ������,ֱ���������  		
                USART_SendData(USART3, p[i]); 
						 }

					}
					jpeg_data_ok = 2;	//���jpeg���ݴ�������,������DMAȥ�ɼ���һ֡��.
					delay_ms(2000);
	   	}
	}
}



int main()
{
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	  SysTick_Init(168);
	  LED_Init();
	  KEYExti_Init();
	  /**LCD��ʼ��**/
    LCD_Init();           //��ʼ��LCD FSMC�ӿں���ʾ����
    POINT_COLOR=RED;      //���û�����ɫΪ��ɫ
	  LCD_DisplayString(10,10,24,"OV2640 Start!");
    /**OV2640��ʼ��**/
		while(OV2640_Init())//��ʼ��OV2640
  	{
	  	LCD_DisplayString(30,130,24,"OV2640 ERR");
	  	delay_ms(200);
  	}
		
		LCD_DisplayString(30,130,24,"OV2640 OK");
	  delay_ms(200);
		
		//����OV2640ģʽ
		ov2640_mode=1;
		jpeg_test();
		 
}


