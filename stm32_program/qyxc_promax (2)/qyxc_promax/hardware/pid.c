#include "pid.h"
#include "usart.h"
#include "tim.h"

//PID��ʼ������
PID pid;
PID pid_R;
PID pid_L;

float R_output_val;
float R_err;
float R_err_last;
float R_err_prev;
float R_target_val;

float L_output_val;
float L_err;
float L_err_last;
float L_err_prev;
float L_target_val;

float l_sum=0,R_bum=0,brush_sum=0;      /*������ֵ��PID���룩*/

int sport_Flag;

/*����ʽpid*/
//brush
float PID_call(float actual_val)
{
    pid.Kp=0.6;
    pid.Ki=0.757;
    pid.Kd=0.239;
	/*����Ŀ��ֵ��ʵ��ֵ�����*/
//  printf("%f \r\n",pid.target_val);
	pid.err = pid.target_val - actual_val; 

	/*PID�㷨ʵ��*/
	pid.output_val += pid.Kp * (pid.err - pid.err_last) + 
				     pid.Ki * pid.err + 
				     pid.Kd * (pid.err - 2.0f*pid.err_last+pid.err_prev);

	/*����*/
	pid.err_prev = pid.err_last ;
	pid.err_last = pid.err;

	/*���ص�ǰʵ��ֵ*/
	return pid.output_val;
}
//right
float PID_call2(float actual_val_R)
{
float R_Kp=0.845;
float R_Ki=0.753;
float R_Kd=0.239;
	/*����Ŀ��ֵ��ʵ��ֵ�����*/
//  printf("%f \r\n",pid.target_val);
	R_err = R_target_val- actual_val_R; 

	/*PID�㷨ʵ��*/
	R_output_val += R_Kp * (R_err - R_err_last) + 
				     R_Ki * R_err + 
				     R_Kd * (R_err - 2.0f*R_err_last+R_err_prev);

	/*����*/
	R_err_prev= R_err_last ;
	R_err_last = R_err;

	/*���ص�ǰʵ��ֵ*/
	return R_output_val;
}

//left
float PID_call3(float actual_val_L)
{
float L_Kp=0.4;
float L_Ki=0.757;
float L_Kd=0.239;
	/*����Ŀ��ֵ��ʵ��ֵ�����*/
//  pLintf("%f \L\n",pid.taLget_val);
	L_err = L_target_val- actual_val_L; 

	/*PID�㷨ʵ��*/
	L_output_val += L_Kp * (L_err - L_err_last) + 
				     L_Ki * L_err + 
				     L_Kd * (L_err - 2.0f*L_err_last+L_err_prev);

	/*����*/
	L_err_prev= L_err_last ;
	L_err_last = L_err;

	/*���ص�ǰʵ��ֵ*/
	return L_output_val;
}


void AutoReloadCallback()
{
float Aes_pwm ;  /*PWMֵ��PID�����*/	
float Bes_pwm ;  
float Ces_pwm ;  
float l_sum,R_bum,brush_sum;      /*������ֵ��PID���룩*/
    /*��ȡ�������������ٶ�ֵ*/
//	l_sum = Read_Encoder(4);
//  R_bum = Read_Encoder(1);
//	brush_sum = Read_Encoder(24);
	R_bum = Read_Encoder(1);
		HAL_UART_Printf(&huart3 , "111:    %f     %f      %f\r\n",l_sum,R_bum,brush_sum);

	
//  l_sum=abs0(l_sum);
//	R_bum=abs0(R_bum);
//	brush_sum=abs0(brush_sum);
	
	// HAL_UART_Printf(&huart3 , "222:    %f     %f      %f\r\n",l_sum,R_bum,brush_sum);

    /*����PID���㣬�õ�PWM���ֵ*/
 
  Aes_pwm = PID_call3(l_sum);  
  Bes_pwm = PID_call2(R_bum);
	Ces_pwm = PID_call(brush_sum);


	HAL_UART_Printf(&huart3 , "333:    %f     %f      %f\r\n",Aes_pwm,Bes_pwm,Ces_pwm);


limit(Aes_pwm);
limit(Bes_pwm);
limit(Ces_pwm);

  /*�޷�*/
//  if(Aes_pwm>1.34f) Aes_pwm=1.34f;
//  else if(Aes_pwm<0) Aes_pwm=0;
//  
//  if(Bes_pwm>1.34f) Bes_pwm=1.34f;
//  else if(Bes_pwm<0) Bes_pwm=0; 
	/*����PWMֵ���Ƶ��ת��*/
	Give_Motor_PWM(Aes_pwm*0.61f,Bes_pwm*0.61f,Ces_pwm*0.61f,sport_Flag);
//  printf("%f,%f,%f,%f\r\n",sum/31.2f,pid.target_val,Aes_pwm,sum);   
}


int Read_Encoder(int TIMX)
{
    	float Encoder_TIM;
      switch(TIMX)
      {
        case 1: Encoder_TIM= (short)TIM1 -> CNT;TIM1 -> CNT=0; break;
        case 4: Encoder_TIM= (short)TIM4 -> CNT;TIM4 -> CNT=0; break; 
        case 24: Encoder_TIM= (short)TIM24 -> CNT;TIM24 -> CNT=0; break;     
				
       	default:Encoder_TIM=0;
      }
   	    return Encoder_TIM;
}


int abs0(float a)
{
	a = a>0?a:(-a);
	return a;
}

void limit(float a)
{
if(a>5000)
{
a=5000;
}
else if(a<0) a=0;
}
