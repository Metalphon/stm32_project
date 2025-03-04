/**
 ****************************************************************************************************
 * @file        gtim.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.3
 * @date        2021-10-15
 * @brief       ͨ�ö�ʱ�� ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ̽���� F407������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20211015
 * ��һ�η���
 * V1.1 20211015
 * 1,����gtim_timx_pwm_chy_init����
 * V1.2 20211015
 * 1,����gtim_timx_cap_chy_init����
 * V1.3 20211015
 * ֧���ⲿ�����������
 * 1,����gtim_timx_cnt_chy_init�Ⱥ���
 * 2,����gtim_timx_cnt_chy_get_count����
 * 3,����gtim_timx_cnt_chy_restart����
 *
 ****************************************************************************************************
 */

#include "./BSP/TIMER/gtim.h"
#include "./BSP/LED/led.h"
#include "./SYSTEM/usart/usart.h"
#include "variables.h"


//TIM_HandleTypeDef g_timx_cnt_chy_handle;        /* ��ʱ��x��� */



///**
// * @brief       ��ʱ���жϷ�����
// * @param       ��
// * @retval      ��
// */
//void GTIM_TIMX_INT_IRQHandler(void)
//{
//    uint16_t count = 0,speed=0;
//    //static uint8_t t=0;
//    /* ���´���û��ʹ�ö�ʱ��HAL�⹲�ô���������������ֱ��ͨ���ж��жϱ�־λ�ķ�ʽ         20ms  */
//    if(__HAL_TIM_GET_FLAG(&g_timx_handle, TIM_FLAG_UPDATE) != RESET)
//    { 
//        //t++;
//         __HAL_TIM_CLEAR_IT(&g_timx_handle, TIM_IT_UPDATE);  /* �����ʱ������жϱ�־λ */
//        //if(t>=100)        
//       // {   
//           // t=0;
//            count=__HAL_TIM_GET_COUNTER(&g_timx_cnt_chy_handle);
//        // printf("CNT:%d\r\n",count);       /* ��ӡȦ�� */
//        
//        speed=(count)*3000/528;
//        
//        
//         printf("Speed: %d\r\n",speed);
//                __HAL_TIM_SET_COUNTER(&g_timx_cnt_chy_handle, 0);   /* ���������� */

//        
//        
//        
//       // }
//    }
//}

/*********************************������ͨ�ö�ʱ��PWM���ʵ�����*************************************/

//TIM_HandleTypeDef g_timx_pwm_chy_handle;     /* ��ʱ��x��� */

/**
 * @brief       ͨ�ö�ʱ��TIMX ͨ��Y PWM��� ��ʼ��������ʹ��PWMģʽ1��
 * @note
 *              ͨ�ö�ʱ����ʱ������APB1,��PPRE1 �� 2��Ƶ��ʱ��
 *              ͨ�ö�ʱ����ʱ��ΪAPB1ʱ�ӵ�2��, ��APB1Ϊ42M, ���Զ�ʱ��ʱ�� = 84Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ��
 * @param       psc: Ԥ��Ƶϵ��
 * @retval      ��
 */
void gtim_timx_pwm_chy_init(uint16_t arr, uint16_t psc)
{
    TIM_OC_InitTypeDef timx_oc_pwm_chy = {0};                       /* ��ʱ�������� */
    
    g_timx_pwm_chy_handle.Instance = GTIM_TIMX_PWM;                 /* ��ʱ��x */
    g_timx_pwm_chy_handle.Init.Prescaler = psc;                     /* Ԥ��Ƶϵ�� */
    g_timx_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;    /* ��������ģʽ */
    g_timx_pwm_chy_handle.Init.Period = arr;                        /* �Զ���װ��ֵ */
    HAL_TIM_PWM_Init(&g_timx_pwm_chy_handle);                       /* ��ʼ��PWM */

    timx_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                       /* ģʽѡ��PWM1 */
    timx_oc_pwm_chy.Pulse = 500 ;                                /* ���ñȽ�ֵ,��ֵ����ȷ��ռ�ձ� */

    timx_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_HIGH;                                        /* ����Ƚϼ���Ϊ�� */
    HAL_TIM_PWM_ConfigChannel(&g_timx_pwm_chy_handle, &timx_oc_pwm_chy, GTIM_TIMX_PWM_CHY); /* ����TIMxͨ��y */
    HAL_TIM_PWM_Start(&g_timx_pwm_chy_handle, GTIM_TIMX_PWM_CHY);                           /* ������ӦPWMͨ�� */
}

/**
 * @brief       ��ʱ���ײ�������ʱ��ʹ�ܣ���������
 *              �˺����ᱻHAL_TIM_PWM_Init()����
 * @param       htim:��ʱ�����
 * @retval      ��
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == GTIM_TIMX_PWM)
    {
        GPIO_InitTypeDef gpio_init_struct;
        GTIM_TIMX_PWM_CHY_GPIO_CLK_ENABLE();                            /* ����ͨ��y��CPIOʱ�� */
        GTIM_TIMX_PWM_CHY_CLK_ENABLE();                                 /* ʹ�ܶ�ʱ��ʱ�� */

        gpio_init_struct.Pin = GTIM_TIMX_PWM_CHY_GPIO_PIN;              /* ͨ��y��CPIO�� */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;                        /* ����������� */
        gpio_init_struct.Pull = GPIO_PULLUP;                            /* ���� */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;                  /* ���� */
        gpio_init_struct.Alternate = GTIM_TIMX_PWM_CHY_GPIO_AF;         /* IO��REMAP����, �Ƿ��Ҫ�鿴ͷ�ļ����õ�˵��! */
        HAL_GPIO_Init(GTIM_TIMX_PWM_CHY_GPIO_PORT, &gpio_init_struct);
    }
}


/*********************************������ͨ�ö�ʱ���������ʵ�����*************************************/



/* ��¼��ʱ�����������������, ���������������� */
uint32_t g_timxchy_cnt_ofcnt = 0 ;              /* ����������� */

/**
 * @brief       ͨ�ö�ʱ��TIMX ͨ��Y ������� ��ʼ������
 * @note
 *              ������ѡ��ͨ�ö�ʱ����ʱ��ѡ��: �ⲿʱ��Դģʽ1(SMS[2:0] = 111)
 *              ����CNT�ļ���ʱ��Դ������ TIMX_CH1/CH2, ����ʵ���ⲿ�������(�������CH1/CH2)
 *
 *              ʱ�ӷ�Ƶ�� = psc, һ������Ϊ0, ��ʾÿһ��ʱ�Ӷ������һ��, ����߾���.
 *              ͨ����ȡCNT���������, �����򵥼���, ���Եõ���ǰ�ļ���ֵ, �Ӷ�ʵ���������
 *
 * @param       arr: �Զ���װֵ 
 * @retval      ��
 */
void gtim_timx_cnt_chy_init(uint16_t psc)
{
    GPIO_InitTypeDef gpio_init_struct;
    TIM_SlaveConfigTypeDef tim_slave_config = {0};
    GTIM_TIMX_CNT_CHY_CLK_ENABLE();                                        /* ʹ��TIMxʱ�� */
    GTIM_TIMX_CNT_CHY_GPIO_CLK_ENABLE();                                   /* ����GPIOAʱ�� */
    
    g_timx_cnt_chy_handle.Instance = GTIM_TIMX_CNT;                        /* ��ʱ��x */
    g_timx_cnt_chy_handle.Init.Prescaler = psc;                            /* Ԥ��Ƶϵ�� */
    g_timx_cnt_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;           /* ��������ģʽ */
    g_timx_cnt_chy_handle.Init.Period = 65535;                             /* �Զ���װ��ֵ */
    HAL_TIM_IC_Init(&g_timx_cnt_chy_handle);

    gpio_init_struct.Pin = GTIM_TIMX_CNT_CHY_GPIO_PIN;                     /* ���벶���GPIO�� */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                               /* ����������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                                 /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;                    /* ���� */
    gpio_init_struct.Alternate = GTIM_TIMX_CNT_CHY_GPIO_AF;                /* ����Ϊ����TIMx��ͨ�� */
    HAL_GPIO_Init(GTIM_TIMX_CNT_CHY_GPIO_PORT, &gpio_init_struct);

    /* ��ģʽ���ⲿ����ģʽ1 */
    tim_slave_config.SlaveMode = TIM_SLAVEMODE_EXTERNAL1;                  /* ��ģʽ���ⲿ����ģʽ1 */
    tim_slave_config.InputTrigger = TIM_TS_TI1FP1;                         /* ���봥����ѡ�� TI1FP1(TIMX_CH1) ��Ϊ����Դ */
    tim_slave_config.TriggerPolarity = TIM_TRIGGERPOLARITY_FALLING;         /* �������ԣ������� */
    tim_slave_config.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1;         /* ����Ԥ��Ƶ���� */
    tim_slave_config.TriggerFilter = 0x0;                                  /* �˲��������в���Ҫ�κ��˲� */
    HAL_TIM_SlaveConfigSynchronization(&g_timx_cnt_chy_handle, &tim_slave_config);

    HAL_NVIC_SetPriority(GTIM_TIMX_CNT_IRQn, 1, 3);                        /* �����ж����ȼ�����ռ���ȼ�1�������ȼ�3 */
    HAL_NVIC_EnableIRQ(GTIM_TIMX_CNT_IRQn);                                /* ����ITMx�ж� */

    __HAL_TIM_ENABLE_IT(&g_timx_cnt_chy_handle, TIM_IT_UPDATE);            /* ʹ�ܸ����ж� */
    HAL_TIM_IC_Start(&g_timx_cnt_chy_handle, GTIM_TIMX_CNT_CHY1);           /* ��ʼ����TIMx��ͨ��y */
}

/**
 * @brief       ͨ�ö�ʱ��TIMX ͨ��Y ��ȡ��ǰ����ֵ 
 * @param       ��
 * @retval      ��ǰ����ֵ
 */
uint32_t gtim_timx_cnt_chy_get_count(void)
{
    uint32_t count = 0;
    //count = g_timxchy_cnt_ofcnt * 65535;                    /* �������������Ӧ�ļ���ֵ */
    count += __HAL_TIM_GET_COUNTER(&g_timx_cnt_chy_handle); /* ���ϵ�ǰCNT��ֵ */
//    printf("gtim_timx count %d \r\n", count);
    return count;
}

/**
 * @brief       ͨ�ö�ʱ��TIMX ͨ��Y ����������
 * @param       ��
 * @retval      ��ǰ����ֵ
 */
void gtim_timx_cnt_chy_restart(void)
{
    __HAL_TIM_DISABLE(&g_timx_cnt_chy_handle);          /* �رն�ʱ��TIMX */
    g_timxchy_cnt_ofcnt = 0;                            /* �ۼ������� */
    __HAL_TIM_SET_COUNTER(&g_timx_cnt_chy_handle, 0);   /* ���������� */
    __HAL_TIM_ENABLE(&g_timx_cnt_chy_handle);           /* ʹ�ܶ�ʱ��TIMX */
}

/**
 * @brief       ͨ�ö�ʱ��TIMX ������� �����жϷ�����
 * @param       ��
 * @retval      ��
 */
void GTIM_TIMX_CNT_IRQHandler(void)
{
    /* ���´���û��ʹ�ö�ʱ��HAL�⹲�ô���������������ֱ��ͨ���ж��жϱ�־λ�ķ�ʽ */
    if(__HAL_TIM_GET_FLAG(&g_timx_cnt_chy_handle, TIM_FLAG_UPDATE) != RESET)
    {
        g_timxchy_cnt_ofcnt++;          /* �ۼ�������� */
    }

    __HAL_TIM_CLEAR_IT(&g_timx_cnt_chy_handle, TIM_IT_UPDATE);
}

