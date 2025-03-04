#ifndef _PID_H
#define _PID_H

#define LIMIT(x,min,max) (x)=(((x)<=(min))?(min):(((x)>=(max))?(max):(x)))  //限幅定义

typedef struct
{
	float target_val;   //目标值
	float err;          //偏差值
	float err_last;     //上一个偏差值
	float err_prev;     //上一个偏差值
	float Kp,Ki,Kd;     //比例、积分、微分系数
	float integral;     //积分值
	float output_val;   //输出值
}PID;

float PID_call(float actual_val);
float PID_call2(float actual_val_R);
float PID_call3(float actual_val_R);
void limit(float a);
extern PID pid_R;
extern PID pid;
extern float R_target_val;
extern float L_target_val;

extern int sport_Flag;
int abs0(float a);
void AutoReloadCallback(void);

int Read_Encoder(int TIMX);
void AutoReloadCallback(void);

extern float sum;
extern float bum;
extern float aim;
extern float bim;

#endif
