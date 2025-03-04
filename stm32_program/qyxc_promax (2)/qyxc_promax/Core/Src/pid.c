#include "pid.h"

void PID_Init(PID_TypeDef *pid, float kp, float ki, float kd, float max, float min)
{
    pid->target_val = 0;
    pid->actual_val = 0;
    pid->err = 0;
    pid->err_last = 0;
    pid->err_sum = 0;
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
    pid->output_val = 0;
    pid->output_max = max;
    pid->output_min = min;
}

float PID_Calculate(PID_TypeDef *pid, float target, float actual)
{
    float p_out, i_out, d_out;
    
    pid->target_val = target;
    pid->actual_val = actual;
    
    // 计算当前偏差
    pid->err = pid->target_val - pid->actual_val;
    
    // 比例项
    p_out = pid->Kp * pid->err;
    
    // 积分项
    pid->err_sum += pid->err;
    i_out = pid->Ki * pid->err_sum;
    
    // 微分项
    d_out = pid->Kd * (pid->err - pid->err_last);
    
    // 记录上次偏差
    pid->err_last = pid->err;
    
    // 计算输出值
    pid->output_val = p_out + i_out + d_out;
    
    // 输出限幅
    if(pid->output_val > pid->output_max)
        pid->output_val = pid->output_max;
    else if(pid->output_val < pid->output_min)
        pid->output_val = pid->output_min;
    
    return pid->output_val;
}

void PID_SetParameters(PID_TypeDef *pid, float kp, float ki, float kd)
{
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
}

void PID_SetLimits(PID_TypeDef *pid, float max, float min)
{
    pid->output_max = max;
    pid->output_min = min;
}

void PID_Reset(PID_TypeDef *pid)
{
    pid->err = 0;
    pid->err_last = 0;
    pid->err_sum = 0;
    pid->output_val = 0;
} 