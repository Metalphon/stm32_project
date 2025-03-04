/**
 ****************************************************************************************************
 * @file        gtim.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.3
 * @date        2021-10-15
 * @brief       通用定时器 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 探索者 F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20211015
 * 第一次发布
 * V1.1 20211015
 * 1,新增gtim_timx_pwm_chy_init函数
 * V1.2 20211015
 * 1,新增gtim_timx_cap_chy_init函数
 * V1.3 20211015
 * 支持外部脉冲计数功能
 * 1,新增gtim_timx_cnt_chy_init等函数
 * 2,新增gtim_timx_cnt_chy_get_count函数
 * 3,新增gtim_timx_cnt_chy_restart函数
 *
 ****************************************************************************************************
 */

#include "./BSP/TIMER/gtim.h"
#include "./BSP/LED/led.h"
#include "./SYSTEM/usart/usart.h"
#include "variables.h"


//TIM_HandleTypeDef g_timx_cnt_chy_handle;        /* 定时器x句柄 */



///**
// * @brief       定时器中断服务函数
// * @param       无
// * @retval      无
// */
//void GTIM_TIMX_INT_IRQHandler(void)
//{
//    uint16_t count = 0,speed=0;
//    //static uint8_t t=0;
//    /* 以下代码没有使用定时器HAL库共用处理函数来处理，而是直接通过判断中断标志位的方式         20ms  */
//    if(__HAL_TIM_GET_FLAG(&g_timx_handle, TIM_FLAG_UPDATE) != RESET)
//    { 
//        //t++;
//         __HAL_TIM_CLEAR_IT(&g_timx_handle, TIM_IT_UPDATE);  /* 清除定时器溢出中断标志位 */
//        //if(t>=100)        
//       // {   
//           // t=0;
//            count=__HAL_TIM_GET_COUNTER(&g_timx_cnt_chy_handle);
//        // printf("CNT:%d\r\n",count);       /* 打印圈数 */
//        
//        speed=(count)*3000/528;
//        
//        
//         printf("Speed: %d\r\n",speed);
//                __HAL_TIM_SET_COUNTER(&g_timx_cnt_chy_handle, 0);   /* 计数器清零 */

//        
//        
//        
//       // }
//    }
//}

/*********************************以下是通用定时器PWM输出实验程序*************************************/

//TIM_HandleTypeDef g_timx_pwm_chy_handle;     /* 定时器x句柄 */

/**
 * @brief       通用定时器TIMX 通道Y PWM输出 初始化函数（使用PWM模式1）
 * @note
 *              通用定时器的时钟来自APB1,当PPRE1 ≥ 2分频的时候
 *              通用定时器的时钟为APB1时钟的2倍, 而APB1为42M, 所以定时器时钟 = 84Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率,单位:Mhz
 *
 * @param       arr: 自动重装值。
 * @param       psc: 预分频系数
 * @retval      无
 */
void gtim_timx_pwm_chy_init(uint16_t arr, uint16_t psc)
{
    TIM_OC_InitTypeDef timx_oc_pwm_chy = {0};                       /* 定时器输出句柄 */
    
    g_timx_pwm_chy_handle.Instance = GTIM_TIMX_PWM;                 /* 定时器x */
    g_timx_pwm_chy_handle.Init.Prescaler = psc;                     /* 预分频系数 */
    g_timx_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;    /* 递增计数模式 */
    g_timx_pwm_chy_handle.Init.Period = arr;                        /* 自动重装载值 */
    HAL_TIM_PWM_Init(&g_timx_pwm_chy_handle);                       /* 初始化PWM */

    timx_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                       /* 模式选择PWM1 */
    timx_oc_pwm_chy.Pulse = 500 ;                                /* 设置比较值,此值用来确定占空比 */

    timx_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_HIGH;                                        /* 输出比较极性为高 */
    HAL_TIM_PWM_ConfigChannel(&g_timx_pwm_chy_handle, &timx_oc_pwm_chy, GTIM_TIMX_PWM_CHY); /* 配置TIMx通道y */
    HAL_TIM_PWM_Start(&g_timx_pwm_chy_handle, GTIM_TIMX_PWM_CHY);                           /* 开启对应PWM通道 */
}

/**
 * @brief       定时器底层驱动，时钟使能，引脚配置
 *              此函数会被HAL_TIM_PWM_Init()调用
 * @param       htim:定时器句柄
 * @retval      无
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == GTIM_TIMX_PWM)
    {
        GPIO_InitTypeDef gpio_init_struct;
        GTIM_TIMX_PWM_CHY_GPIO_CLK_ENABLE();                            /* 开启通道y的CPIO时钟 */
        GTIM_TIMX_PWM_CHY_CLK_ENABLE();                                 /* 使能定时器时钟 */

        gpio_init_struct.Pin = GTIM_TIMX_PWM_CHY_GPIO_PIN;              /* 通道y的CPIO口 */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;                        /* 复用推完输出 */
        gpio_init_struct.Pull = GPIO_PULLUP;                            /* 上拉 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;                  /* 高速 */
        gpio_init_struct.Alternate = GTIM_TIMX_PWM_CHY_GPIO_AF;         /* IO口REMAP设置, 是否必要查看头文件配置的说明! */
        HAL_GPIO_Init(GTIM_TIMX_PWM_CHY_GPIO_PORT, &gpio_init_struct);
    }
}


/*********************************以下是通用定时器脉冲计数实验程序*************************************/



/* 记录定时器计数器的溢出次数, 方便计算总脉冲个数 */
uint32_t g_timxchy_cnt_ofcnt = 0 ;              /* 计数溢出次数 */

/**
 * @brief       通用定时器TIMX 通道Y 脉冲计数 初始化函数
 * @note
 *              本函数选择通用定时器的时钟选择: 外部时钟源模式1(SMS[2:0] = 111)
 *              这样CNT的计数时钟源就来自 TIMX_CH1/CH2, 可以实现外部脉冲计数(脉冲接入CH1/CH2)
 *
 *              时钟分频数 = psc, 一般设置为0, 表示每一个时钟都会计数一次, 以提高精度.
 *              通过读取CNT和溢出次数, 经过简单计算, 可以得到当前的计数值, 从而实现脉冲计数
 *
 * @param       arr: 自动重装值 
 * @retval      无
 */
void gtim_timx_cnt_chy_init(uint16_t psc)
{
    GPIO_InitTypeDef gpio_init_struct;
    TIM_SlaveConfigTypeDef tim_slave_config = {0};
    GTIM_TIMX_CNT_CHY_CLK_ENABLE();                                        /* 使能TIMx时钟 */
    GTIM_TIMX_CNT_CHY_GPIO_CLK_ENABLE();                                   /* 开启GPIOA时钟 */
    
    g_timx_cnt_chy_handle.Instance = GTIM_TIMX_CNT;                        /* 定时器x */
    g_timx_cnt_chy_handle.Init.Prescaler = psc;                            /* 预分频系数 */
    g_timx_cnt_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;           /* 递增计数模式 */
    g_timx_cnt_chy_handle.Init.Period = 65535;                             /* 自动重装载值 */
    HAL_TIM_IC_Init(&g_timx_cnt_chy_handle);

    gpio_init_struct.Pin = GTIM_TIMX_CNT_CHY_GPIO_PIN;                     /* 输入捕获的GPIO口 */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                               /* 复用推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                                 /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;                    /* 高速 */
    gpio_init_struct.Alternate = GTIM_TIMX_CNT_CHY_GPIO_AF;                /* 复用为捕获TIMx的通道 */
    HAL_GPIO_Init(GTIM_TIMX_CNT_CHY_GPIO_PORT, &gpio_init_struct);

    /* 从模式：外部触发模式1 */
    tim_slave_config.SlaveMode = TIM_SLAVEMODE_EXTERNAL1;                  /* 从模式：外部触发模式1 */
    tim_slave_config.InputTrigger = TIM_TS_TI1FP1;                         /* 输入触发：选择 TI1FP1(TIMX_CH1) 作为输入源 */
    tim_slave_config.TriggerPolarity = TIM_TRIGGERPOLARITY_FALLING;         /* 触发极性：上升沿 */
    tim_slave_config.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1;         /* 触发预分频：无 */
    tim_slave_config.TriggerFilter = 0x0;                                  /* 滤波：本例中不需要任何滤波 */
    HAL_TIM_SlaveConfigSynchronization(&g_timx_cnt_chy_handle, &tim_slave_config);

    HAL_NVIC_SetPriority(GTIM_TIMX_CNT_IRQn, 1, 3);                        /* 设置中断优先级，抢占优先级1，子优先级3 */
    HAL_NVIC_EnableIRQ(GTIM_TIMX_CNT_IRQn);                                /* 开启ITMx中断 */

    __HAL_TIM_ENABLE_IT(&g_timx_cnt_chy_handle, TIM_IT_UPDATE);            /* 使能更新中断 */
    HAL_TIM_IC_Start(&g_timx_cnt_chy_handle, GTIM_TIMX_CNT_CHY1);           /* 开始捕获TIMx的通道y */
}

/**
 * @brief       通用定时器TIMX 通道Y 获取当前计数值 
 * @param       无
 * @retval      当前计数值
 */
uint32_t gtim_timx_cnt_chy_get_count(void)
{
    uint32_t count = 0;
    //count = g_timxchy_cnt_ofcnt * 65535;                    /* 计算溢出次数对应的计数值 */
    count += __HAL_TIM_GET_COUNTER(&g_timx_cnt_chy_handle); /* 加上当前CNT的值 */
//    printf("gtim_timx count %d \r\n", count);
    return count;
}

/**
 * @brief       通用定时器TIMX 通道Y 重启计数器
 * @param       无
 * @retval      当前计数值
 */
void gtim_timx_cnt_chy_restart(void)
{
    __HAL_TIM_DISABLE(&g_timx_cnt_chy_handle);          /* 关闭定时器TIMX */
    g_timxchy_cnt_ofcnt = 0;                            /* 累加器清零 */
    __HAL_TIM_SET_COUNTER(&g_timx_cnt_chy_handle, 0);   /* 计数器清零 */
    __HAL_TIM_ENABLE(&g_timx_cnt_chy_handle);           /* 使能定时器TIMX */
}

/**
 * @brief       通用定时器TIMX 脉冲计数 更新中断服务函数
 * @param       无
 * @retval      无
 */
void GTIM_TIMX_CNT_IRQHandler(void)
{
    /* 以下代码没有使用定时器HAL库共用处理函数来处理，而是直接通过判断中断标志位的方式 */
    if(__HAL_TIM_GET_FLAG(&g_timx_cnt_chy_handle, TIM_FLAG_UPDATE) != RESET)
    {
        g_timxchy_cnt_ofcnt++;          /* 累计溢出次数 */
    }

    __HAL_TIM_CLEAR_IT(&g_timx_cnt_chy_handle, TIM_IT_UPDATE);
}

