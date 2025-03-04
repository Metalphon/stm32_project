#ifndef _lcd_H
#define _lcd_H

#include "System.h"

/////////////////////////////////////////////////
//LCD驱动参数
//LCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	 wramcmd;		//开始写gram指令
	u16  setxcmd;		//设置x坐标指令
	u16  setycmd;		//设置y坐标指令 
}_lcd_dev;

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
//LCD的画笔颜色和背景色	   
extern u16  POINT_COLOR;//默认红色    
extern u16  BACK_COLOR; //背景颜色.默认为白色
//////////////////////////////////////////////////////////////////////////////////	 
//-----------------LCD背光端口定义---------------- 
#define	LCD_BACK PFout(10)  //LCD背光    	PF10 	


//A12(RS)作为数据命令区分线  
//RS线是用来决定传输的是数据还是命令。因此，本质上可以将其理解为一个地址信号，比如，将RS接到A0上面，
//那么当FSMC控制器写地址0的时候，将A0变为0，对TFTLCD来说，就是写命令。而FSMC写地址1的时候，就将A0变
//为1，对LCD来说，就是写数据了。这样就将数据和命令区分开来。
#define  CMD_BASE     ((u32)(0x6C000000 | 0x00001FFE))     //0X00001FFE为相对于基地址的偏移量。
#define  DATA_BASE    ((u32)(0x6C000000 | 0x00002000))

#define LCD_CMD       ( * (u16 *) CMD_BASE )      //此时RS==0     
#define LCD_DATA      ( * (u16 *) DATA_BASE)      //此时RS==1

//扫描方向定义
#define  L2R_U2D  0 //从左到右,从上到下
#define  L2R_D2U  1 //从左到右,从下到上
#define  R2L_U2D  2 //从右到左,从上到下
#define  R2L_D2U  3 //从右到左,从下到上

#define  U2D_L2R  4 //从上到下,从左到右
#define  U2D_R2L  5 //从上到下,从右到左
#define  D2U_L2R  6 //从下到上,从左到右
#define  D2U_R2L  7 //从下到上,从右到左	 


//颜色值定义
#define  WHITE        0xFFFF
#define  BLACK        0x0000	
#define  RED          0xF800
#define  GREEN        0x07E0
#define  BLUE         0x001F 
#define  BRED         0XF81F
#define  GRED 			  0XFFE0
#define  GBLUE			  0X07FF
#define  BROWN 			  0XBC40  
#define  BRRED 			  0XFC07  
#define  GRAY  			  0X8430  
#define  MAGENTA      0xF81F
#define  CYAN         0x7FFF
#define  YELLOW       0xFFE0

void LCD_WriteReg(u16 LCD_Reg, u16 LCD_Value);
u16 LCD_ReadReg(u16 LCD_Reg);
void LCD_WriteGRAM(void);

void LCD_Init(void);													   	//初始化
void LCD_DisplayOn(void);													//开显示
void LCD_DisplayOff(void);												//关显示
void LCD_Clear(u16 Color);	 											//清屏
void LCD_SetCursor(u16 Xpos, u16 Ypos);						//设置光标
void LCD_DrawPoint(u16 x,u16 y);									//画点
void LCD_Color_DrawPoint(u16 x,u16 y,u16 color);	//颜色画点
u16  LCD_GetPoint(u16 x,u16 y); 								  //读点 

void LCD_Open_Window(u16 X0,u16 Y0,u16 width,u16 height); 	
void Set_Scan_Direction(u8 direction);					 
void Set_Display_Mode(u8 mode);						 

void LCD_Fill_onecolor(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);		//填充单个颜色
void LCD_Draw_Picture(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);		//填充指定颜色
void LCD_DisplayChar(u16 x,u16 y,u8 word,u8 size);						    //显示一个字符
void LCD_DisplayString(u16 x,u16 y,u8 size,u8 *p);		            //显示一个12/16/24字体字符串
void LCD_DisplayString_color(u16 x,u16 y,u8 size,u8 *p,u16 brushcolor,u16 backcolor); //显示一个12/16/24字体自定义颜色的字符串
void LCD_DisplayNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);				//显示 数字
void LCD_DisplayNum_color(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode,u16 brushcolor,u16 backcolor); //显示自定义颜色数字	  	   																			 

#endif


