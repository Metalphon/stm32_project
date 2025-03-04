#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/TIMER/gtim.h"
#include "./BSP/PID/pid.h"
#include "./BSP/LCD/lcd.h"


extern TIM_HandleTypeDef g_timx_pwm_chy_handle;     /* 定时器x PWM 句柄 */
extern TIM_HandleTypeDef g_timx_cnt_chy_handle;     /* 定时器x 脉冲计数句柄 */


int main(void)
{
   
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7);         /* 设置时钟,168Mhz */
    delay_init(168);                            /* 延时初始化 */
    usart_init(115200);                         /* 串口初始化为115200 */
    led_init();                                 /* 初始化LED */
    PID_init();
    lcd_init();

    gtim_timx_cnt_chy_init(0);                  /* 定时器计数初始化, 不分频 */
    

    gtim_timx_pwm_chy_init(1000 - 1, 84 - 1);//pwm setting,tim14(changeable)
    gtim_timx_int_init(200-1,8400-1);           /*   10 ms 中断*///setting the tim3 with arr for period and psc for frequency dividing
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
