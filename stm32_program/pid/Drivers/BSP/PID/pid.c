#include "./BSP/PID/pid.h"
#include "./SYSTEM/sys/sys.h"
#include "./BSP/LCD/lcd.h"
#include "./SYSTEM/usart/usart.h"

TIM_HandleTypeDef g_timx_pwm_chy_handle;
TIM_HandleTypeDef g_timx_cnt_chy_handle;
TIM_HandleTypeDef g_timx_handle;             /* ��ʱ��x��� */

uint8_t Point=120;                    /* �趨Ŀ��ת��  min/r  */
//#define SetPointL Point*528/3000      /* ת��ת��Ϊ 20ms �������������� */
uint16_t Moto;

PID zspeed,zpos;
Error value,vpos;

void PID_init(void)    /* ��ʼ�� P I D */
{
    /* �ٶ�PID  */
    zspeed.P=20;
    zspeed.I=2.5;
    zspeed.D=0;
    
    value.Current_Error=0;//��ǰ���
    value.Last_Error=0;//��һ�����
    value.Previous_Error=0;//���ϴ����
    
    /* λ��PID */
    zpos.P=20;
    zpos.I=2.5;
    zpos.D=0;
    
    vpos.Current_Error=0;//��ǰ���
    vpos.Last_Error=0;//��һ�����
    vpos.Previous_Error=0;//���ϴ����
    
}

/*! 
 *  @brief      ����ʽPID
 *  @since      v1.0
 *  *sptr ��������
 *  *pid:  PID����
 *  NowPlace��ʵ��ֵ
 *  Point��   ����ֵ
 */
// ����ʽPID�������


uint32_t PID_Increase(Error *sptr, PID *pid, uint32_t NowPlace, uint32_t Point)
{
 
	uint32_t iError,	//��ǰ���
		Increase;	//���ó���ʵ������
 
	iError = Point - NowPlace;	// ���㵱ǰ���
 
	Increase =  pid->P * (iError - sptr->Last_Error)   //����P
			  + pid->I * iError      //����I
			  + pid->D * (iError - 2 * sptr->Last_Error + sptr->Previous_Error);  //΢��D
	
	sptr->Previous_Error = sptr->Last_Error;	// ����ǰ�����
	sptr->Last_Error = iError;		  	// �����ϴ����
	
	return Increase;	// ��������
}

/*! 
 *  @brief      λ��ʽPID
 *  @since      v1.0
 *  *sptr ��������
 *  *pid:  PID����
 *  NowPlace����ǰλ��
 *  Point��   Ԥ��λ��  
 */
 
// λ��ʽPID����
float PID_Realize(Error *sptr,PID *pid, uint32_t NowPlace, float Point)
{
 
	uint32_t iError,	// ��ǰ���
		 Realize;   //ʵ�����	
 
	iError = Point - NowPlace;	// ���㵱ǰ���
	sptr->Current_Error += pid->I * iError;	// ������
      
    sptr->Current_Error = sptr->Current_Error > pid->limit?pid->limit:sptr->Current_Error;//�����޷�
      sptr->Current_Error = sptr->Current_Error <-pid->limit?-pid->limit:sptr->Current_Error;
	
    Realize = pid->P * iError       //����P
            + sptr->Current_Error   //����I
			+ pid->D * (iError - sptr->Last_Error);  //΢��D
	sptr->Last_Error = iError;		  	// �����ϴ����
	return Realize;	// ����ʵ��ֵ
}

//�ٶ�PID
void Speed_PID(uint8_t Point)
{
    uint32_t SetPointL=Point*528/3000;                   /* ת��ת��Ϊ 20ms �������������� */
    uint32_t count = 0,speed=0,Increase=0;
   
    count=__HAL_TIM_GET_COUNTER(&g_timx_cnt_chy_handle); /* ��ȡ��ǰ�������������ֵ */

    speed=(count)*3000/528;                              /* ���㵱ǰ��ת�� */
    
    __HAL_TIM_SET_COUNTER(&g_timx_cnt_chy_handle, 0);    /* ���������� */      
    lcd_show_num(50,20,speed,3,16,RED);

    Increase=PID_Increase( &value , &zspeed , count , SetPointL );  /* �����õ�����ʽPID��������ֵ */
    lcd_show_num(50,60,Increase,3,16,RED);
    Moto+=Increase;

    if(Moto>=950) Moto=950;                                         /* ����PWMռ�ձȷ�ֹ�𻵵�� */
    lcd_show_num(50,40,Moto,3,16,RED);
    __HAL_TIM_SET_COMPARE(&g_timx_pwm_chy_handle, TIM_CHANNEL_1 ,Moto );/* ����PWMʹ�ٶȴﵽĿ��ֵ */
    
    printf("%d\r\n",speed);                                             /* ���ڴ�ӡ��ǰ�ٶ� */


}
/**
 * @brief       ͨ�ö�ʱ��TIMX��ʱ�жϳ�ʼ������

 * @param       arr: �Զ���װֵ
 * @param       psc: Ԥ��Ƶϵ��
 * @retval      ��
 */
void gtim_timx_int_init(uint16_t arr, uint16_t psc)
{
    GTIM_TIMX_INT_CLK_ENABLE(); /* ʹ��TIMxʱ�� */

    g_timx_handle.Instance = GTIM_TIMX_INT;                 /* ͨ�ö�ʱ��x */
    g_timx_handle.Init.Prescaler = psc;                     /* Ԥ��Ƶϵ�� */
    g_timx_handle.Init.CounterMode = TIM_COUNTERMODE_UP;    /* ��������ģʽ */
    g_timx_handle.Init.Period = arr;                        /* �Զ�װ��ֵ */
    HAL_TIM_Base_Init(&g_timx_handle);
    
    HAL_NVIC_SetPriority(GTIM_TIMX_INT_IRQn, 1, 2);         /* �����ж����ȼ�����ռ���ȼ�1�������ȼ�3 */
    HAL_NVIC_EnableIRQ(GTIM_TIMX_INT_IRQn);                 /* ����ITMx�ж� */

    HAL_TIM_Base_Start_IT(&g_timx_handle);                  /* ʹ�ܶ�ʱ��x�Ͷ�ʱ��x�����ж� */
}

/**
 * @brief       ��ʱ���жϷ����� ����ת��
 * @param       ��
 * @retval      ��
 */
void GTIM_TIMX_INT_IRQHandler(void)
{   
    /* ���´���û��ʹ�ö�ʱ��HAL�⹲�ô���������������ֱ��ͨ���ж��жϱ�־λ�ķ�ʽ         20ms  */
    if(__HAL_TIM_GET_FLAG(&g_timx_handle, TIM_FLAG_UPDATE) != RESET)
    { 
         __HAL_TIM_CLEAR_IT(&g_timx_handle, TIM_IT_UPDATE);  /* �����ʱ������жϱ�־λ */
        Speed_PID(120);                                       /* Ŀ���ٶ� */
                      
    }
}
