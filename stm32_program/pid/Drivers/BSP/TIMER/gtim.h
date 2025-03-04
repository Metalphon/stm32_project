/**
 ****************************************************************************************************
 * @file        gtim.h
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

#ifndef __GTIM_H
#define __GTIM_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* ͨ�ö�ʱ�� ���� */


/* TIMX PWM������� 
 * ���������PWM����LED0(RED)������
 * Ĭ�������TIM2~TIM5
 * ע��: ͨ���޸��⼸���궨��,����֧��TIM1~TIM8����һ����ʱ��,����һ��IO�����PWM
 */
#define GTIM_TIMX_PWM_CHY_GPIO_PORT         GPIOF
#define GTIM_TIMX_PWM_CHY_GPIO_PIN          GPIO_PIN_9
#define GTIM_TIMX_PWM_CHY_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)  /* PF��ʱ��ʹ�� */
#define GTIM_TIMX_PWM_CHY_GPIO_AF           GPIO_AF9_TIM14                               /* �˿ڸ��õ�TIM14 */

/* TIMX REMAP����
 * ��Ϊ����LED0����PF9��, ����ͨ������TIM14�Ĳ�����ӳ�书��, ���ܽ�TIM14_CH1�����PF9��
 */

#define GTIM_TIMX_PWM                       TIM14                                        /* TIM14 */
#define GTIM_TIMX_PWM_CHY                   TIM_CHANNEL_1                                /* ͨ��Y,  1<= Y <=4 */
#define GTIM_TIMX_PWM_CHY_CCRX              TIM14->CCR1                                  /* ͨ��Y������ȽϼĴ��� */
#define GTIM_TIMX_PWM_CHY_CLK_ENABLE()      do{ __HAL_RCC_TIM14_CLK_ENABLE(); }while(0)  /* TIM14 ʱ��ʹ�� */


/* TIM2 �ٶ� �����������  
* ������������ʹ�ö�ʱ��TIM2_CH1,����WK_UP����������
* Ĭ�������TIM2~TIM5, ֻ��CH1��CH2ͨ�����������������, CH3/CH4��֧��!
* ע��: ͨ���޸��⼸���궨��,����֧��TIM1~TIM8����һ����ʱ��,CH1/CH2��ӦIO�����������
*       �ر�Ҫע��:Ĭ���õ�PA0,���õ�����������!���������IO,��Ӧ����������ʽҲ�ø�!
*/
#define GTIM_TIMX_CNT_CHY_GPIO_PORT            GPIOA
#define GTIM_TIMX_CNT_CHY_GPIO_PIN             GPIO_PIN_0
#define GTIM_TIMX_CNT_CHY_GPIO_AF              GPIO_AF1_TIM2                                /* AF����ѡ�� */
#define GTIM_TIMX_CNT_CHY_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)  /* PA��ʱ��ʹ�� */

#define GTIM_TIMX_CNT                          TIM2
#define GTIM_TIMX_CNT_IRQn                     TIM2_IRQn
#define GTIM_TIMX_CNT_IRQHandler               TIM2_IRQHandler
#define GTIM_TIMX_CNT_CHY1                      TIM_CHANNEL_1                                /* ͨ��Y,  1<= Y <=2 */

#define GTIM_TIMX_CNT_CHY_CLK_ENABLE()         do{ __HAL_RCC_TIM2_CLK_ENABLE(); }while(0)   /* TIM2 ʱ��ʹ�� */

/******************************************************************************************/


/******************************************************************************************/

void gtim_timx_pwm_chy_init(uint16_t arr, uint16_t psc);    /* ͨ�ö�ʱ�� PWM��ʼ������ */
void gtim_timx_cnt_chy_init(uint16_t psc);                  /* ͨ�ö�ʱ�� ���������ʼ������ */
uint32_t gtim_timx_cnt_chy_get_count(void);                 /* ͨ�ö�ʱ�� ��ȡ������� */
void gtim_timx_cnt_chy_restart(void);                       /* ͨ�ö�ʱ�� ���������� */

void gtim_tim5_cnt_chy_init(uint16_t psc);                  /* ͨ�ö�ʱ�� ���������ʼ������ */

#endif

















