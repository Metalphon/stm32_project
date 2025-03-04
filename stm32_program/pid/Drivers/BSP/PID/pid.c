#include "./BSP/PID/pid.h"
#include "./SYSTEM/sys/sys.h"
#include "./BSP/LCD/lcd.h"
#include "./SYSTEM/usart/usart.h"

TIM_HandleTypeDef g_timx_pwm_chy_handle;
TIM_HandleTypeDef g_timx_cnt_chy_handle;
TIM_HandleTypeDef g_timx_handle;             /* 定时器x句柄 */

uint8_t Point=120;                    /* 设定目标转速  min/r  */
//#define SetPointL Point*528/3000      /* 转速转化为 20ms 编码器脉冲数量 */
uint16_t Moto;

PID zspeed,zpos;
Error value,vpos;

void PID_init(void)    /* 初始化 P I D */
{
    /* 速度PID  */
    zspeed.P=20;
    zspeed.I=2.5;
    zspeed.D=0;
    
    value.Current_Error=0;//当前误差
    value.Last_Error=0;//上一次误差
    value.Previous_Error=0;//上上次误差
    
    /* 位置PID */
    zpos.P=20;
    zpos.I=2.5;
    zpos.D=0;
    
    vpos.Current_Error=0;//当前误差
    vpos.Last_Error=0;//上一次误差
    vpos.Previous_Error=0;//上上次误差
    
}

/*! 
 *  @brief      增量式PID
 *  @since      v1.0
 *  *sptr ：误差参数
 *  *pid:  PID参数
 *  NowPlace：实际值
 *  Point：   期望值
 */
// 增量式PID电机控制


uint32_t PID_Increase(Error *sptr, PID *pid, uint32_t NowPlace, uint32_t Point)
{
 
	uint32_t iError,	//当前误差
		Increase;	//最后得出的实际增量
 
	iError = Point - NowPlace;	// 计算当前误差
 
	Increase =  pid->P * (iError - sptr->Last_Error)   //比例P
			  + pid->I * iError      //积分I
			  + pid->D * (iError - 2 * sptr->Last_Error + sptr->Previous_Error);  //微分D
	
	sptr->Previous_Error = sptr->Last_Error;	// 更新前次误差
	sptr->Last_Error = iError;		  	// 更新上次误差
	
	return Increase;	// 返回增量
}

/*! 
 *  @brief      位置式PID
 *  @since      v1.0
 *  *sptr ：误差参数
 *  *pid:  PID参数
 *  NowPlace：当前位置
 *  Point：   预期位置  
 */
 
// 位置式PID控制
float PID_Realize(Error *sptr,PID *pid, uint32_t NowPlace, float Point)
{
 
	uint32_t iError,	// 当前误差
		 Realize;   //实际输出	
 
	iError = Point - NowPlace;	// 计算当前误差
	sptr->Current_Error += pid->I * iError;	// 误差积分
      
    sptr->Current_Error = sptr->Current_Error > pid->limit?pid->limit:sptr->Current_Error;//积分限幅
      sptr->Current_Error = sptr->Current_Error <-pid->limit?-pid->limit:sptr->Current_Error;
	
    Realize = pid->P * iError       //比例P
            + sptr->Current_Error   //积分I
			+ pid->D * (iError - sptr->Last_Error);  //微分D
	sptr->Last_Error = iError;		  	// 更新上次误差
	return Realize;	// 返回实际值
}

//速度PID
void Speed_PID(uint8_t Point)
{
    uint32_t SetPointL=Point*528/3000;                   /* 转速转化为 20ms 编码器脉冲数量 */
    uint32_t count = 0,speed=0,Increase=0;
   
    count=__HAL_TIM_GET_COUNTER(&g_timx_cnt_chy_handle); /* 获取当前编码器脉冲计数值 */

    speed=(count)*3000/528;                              /* 计算当前的转速 */
    
    __HAL_TIM_SET_COUNTER(&g_timx_cnt_chy_handle, 0);    /* 计数器清零 */      
    lcd_show_num(50,20,speed,3,16,RED);

    Increase=PID_Increase( &value , &zspeed , count , SetPointL );  /* 计数得到增量式PID的增量数值 */
    lcd_show_num(50,60,Increase,3,16,RED);
    Moto+=Increase;

    if(Moto>=950) Moto=950;                                         /* 限制PWM占空比防止损坏电机 */
    lcd_show_num(50,40,Moto,3,16,RED);
    __HAL_TIM_SET_COMPARE(&g_timx_pwm_chy_handle, TIM_CHANNEL_1 ,Moto );/* 调节PWM使速度达到目标值 */
    
    printf("%d\r\n",speed);                                             /* 串口打印当前速度 */


}
/**
 * @brief       通用定时器TIMX定时中断初始化函数

 * @param       arr: 自动重装值
 * @param       psc: 预分频系数
 * @retval      无
 */
void gtim_timx_int_init(uint16_t arr, uint16_t psc)
{
    GTIM_TIMX_INT_CLK_ENABLE(); /* 使能TIMx时钟 */

    g_timx_handle.Instance = GTIM_TIMX_INT;                 /* 通用定时器x */
    g_timx_handle.Init.Prescaler = psc;                     /* 预分频系数 */
    g_timx_handle.Init.CounterMode = TIM_COUNTERMODE_UP;    /* 递增计数模式 */
    g_timx_handle.Init.Period = arr;                        /* 自动装载值 */
    HAL_TIM_Base_Init(&g_timx_handle);
    
    HAL_NVIC_SetPriority(GTIM_TIMX_INT_IRQn, 1, 2);         /* 设置中断优先级，抢占优先级1，子优先级3 */
    HAL_NVIC_EnableIRQ(GTIM_TIMX_INT_IRQn);                 /* 开启ITMx中断 */

    HAL_TIM_Base_Start_IT(&g_timx_handle);                  /* 使能定时器x和定时器x更新中断 */
}

/**
 * @brief       定时器中断服务函数 计算转速
 * @param       无
 * @retval      无
 */
void GTIM_TIMX_INT_IRQHandler(void)
{   
    /* 以下代码没有使用定时器HAL库共用处理函数来处理，而是直接通过判断中断标志位的方式         20ms  */
    if(__HAL_TIM_GET_FLAG(&g_timx_handle, TIM_FLAG_UPDATE) != RESET)
    { 
         __HAL_TIM_CLEAR_IT(&g_timx_handle, TIM_IT_UPDATE);  /* 清除定时器溢出中断标志位 */
        Speed_PID(120);                                       /* 目标速度 */
                      
    }
}
