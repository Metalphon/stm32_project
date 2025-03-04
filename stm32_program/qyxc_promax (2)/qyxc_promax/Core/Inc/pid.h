#ifndef __PID_H
#define __PID_H

#include "main.h"

typedef struct {
    float target_val;   // 目标值
    float actual_val;   // 实际值
    float err;          // 当前偏差
    float err_last;     // 上一次偏差
    float err_sum;      // 误差累计值
    float Kp, Ki, Kd;   // PID参数
    float output_val;   // PID输出值
    float output_max;   // 输出限幅最大值
    float output_min;   // 输出限幅最小值
} PID_TypeDef;

// PID初始化函数
void PID_Init(PID_TypeDef *pid, float kp, float ki, float kd, float max, float min);

// PID计算函数
float PID_Calculate(PID_TypeDef *pid, float target, float actual);

// PID参数设置函数
void PID_SetParameters(PID_TypeDef *pid, float kp, float ki, float kd);

// PID限幅设置函数
void PID_SetLimits(PID_TypeDef *pid, float max, float min);

// PID重置函数
void PID_Reset(PID_TypeDef *pid);

#endif /* __PID_H */ 