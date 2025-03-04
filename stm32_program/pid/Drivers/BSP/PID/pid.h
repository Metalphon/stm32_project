#ifndef __PID_H
#define	__PID_H
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/sys/sys.h"


/* TIMX �ж϶��� 
 * Ĭ�������TIM2~TIM5
 * ע��: ͨ���޸���4���궨��,����֧��TIM1~TIM8����һ����ʱ��.
 */
 
#define GTIM_TIMX_INT                       TIM3
#define GTIM_TIMX_INT_IRQn                  TIM3_IRQn
#define GTIM_TIMX_INT_IRQHandler            TIM3_IRQHandler
#define GTIM_TIMX_INT_CLK_ENABLE()          do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)   /* TIM3 ʱ��ʹ�� */

typedef struct 
{ 
  float P,I,D,limit;
}PID;

typedef struct
{
  float Current_Error;//��ǰ���
  float Last_Error;//��һ�����
  float Previous_Error;//���ϴ����
}Error;


void gtim_timx_int_init(uint16_t arr, uint16_t psc);

uint32_t PID_Increase(Error *sptr, PID *pid, uint32_t NowPlace, uint32_t Point);
float PID_Realize(Error *sptr,PID *pid, uint32_t NowPlace, float Point);

void PID_init(void);
void Speed_PID(uint8_t Point);

#endif
