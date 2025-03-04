#ifndef _PID_H
#define _PID_H

#define LIMIT(x,min,max) (x)=(((x)<=(min))?(min):(((x)>=(max))?(max):(x)))  //�޷�����

typedef struct
{
	float target_val;   //Ŀ��ֵ
	float err;          //ƫ��ֵ
	float err_last;     //��һ��ƫ��ֵ
	float err_prev;     //��һ��ƫ��ֵ
	float Kp,Ki,Kd;     //���������֡�΢��ϵ��
	float integral;     //����ֵ
	float output_val;   //���ֵ
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
