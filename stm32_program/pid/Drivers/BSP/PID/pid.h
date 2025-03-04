#ifndef __PID_H
#define	__PID_H
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/sys/sys.h"


/* TIMX 中断定义 
 * 默认是针对TIM2~TIM5
 * 注意: 通过修改这4个宏定义,可以支持TIM1~TIM8任意一个定时器.
 */
 
#define GTIM_TIMX_INT                       TIM3
#define GTIM_TIMX_INT_IRQn                  TIM3_IRQn
#define GTIM_TIMX_INT_IRQHandler            TIM3_IRQHandler
#define GTIM_TIMX_INT_CLK_ENABLE()          do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)   /* TIM3 时钟使能 */

typedef struct 
{ 
  float P,I,D,limit;
}PID;

typedef struct
{
  float Current_Error;//当前误差
  float Last_Error;//上一次误差
  float Previous_Error;//上上次误差
}Error;


void gtim_timx_int_init(uint16_t arr, uint16_t psc);

uint32_t PID_Increase(Error *sptr, PID *pid, uint32_t NowPlace, uint32_t Point);
float PID_Realize(Error *sptr,PID *pid, uint32_t NowPlace, float Point);

void PID_init(void);
void Speed_PID(uint8_t Point);

#endif
