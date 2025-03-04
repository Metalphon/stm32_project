#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/TIMER/gtim.h"
#include "./BSP/PID/pid.h"
#include "./BSP/LCD/lcd.h"


extern TIM_HandleTypeDef g_timx_pwm_chy_handle;     /* ��ʱ��x PWM ��� */
extern TIM_HandleTypeDef g_timx_cnt_chy_handle;     /* ��ʱ��x ���������� */


int main(void)
{
   
    HAL_Init();                                 /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7);         /* ����ʱ��,168Mhz */
    delay_init(168);                            /* ��ʱ��ʼ�� */
    usart_init(115200);                         /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                                 /* ��ʼ��LED */
    PID_init();
    lcd_init();

    gtim_timx_cnt_chy_init(0);                  /* ��ʱ��������ʼ��, ����Ƶ */
    

    gtim_timx_pwm_chy_init(1000 - 1, 84 - 1);//pwm setting,tim14(changeable)
    gtim_timx_int_init(200-1,8400-1);           /*   10 ms �ж�*///setting the tim3 with arr for period and psc for frequency dividing
   /* lcd_clear(WHITE);
    lcd_show_string(0,20,200,16,16,"SPEED:",RED);

    lcd_show_string(0,40,200,16,16,"Moto:",RED);

    lcd_show_string(0,60,200,16,16,"Reali:",RED);
    
    lcd_show_string(0,80,200,16,16,"Desti:",RED);
    
    lcd_show_string(0,100,200,16,16,"Count:",RED);
*/

    __HAL_TIM_SET_COMPARE(&g_timx_pwm_chy_handle, TIM_CHANNEL_1 ,0);

    while (1)
    {

       


    }
}
