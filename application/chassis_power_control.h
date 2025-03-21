#ifndef __CHASSIS_POWER_CONTROL_H
#define __CHASSIS_POWER_CONTROL_H

#include "main.h"
#include "vofa_task.h"


#define TOQUE_COEFFICIENT_3508 1.99688994e-6f
#define TOQUE_COEFFICIENT_6020 0.8130f//1.3568115234375e-4f
/*
k1����ʹ�䳵��ת��ʹ����ϵͳ���ʴﵽ�������޸�����������ʱ�Ӵ�k1.û�ﵽʱ���С
k2����ʹ�䳵С���ݣ�ʹ����ϵͳ���ʴﵽ�������޸�����������ʱ�Ӵ�k2.û�ﵽʱ���С

����ԭ����ʽ-P=k1*F*F+b*V*F+k2*V*V+C,(��Ԫһ�η���)
��Ϊ��תʱ�ٶȻ�����С��������ҪΪ���ؾ���������k1Ӱ��󡣵�k1
С����ʱ�ٶ����������k2
*/
#define POWER_3508_K1 			2.23e-07f//6.00e-07f//2.50999989e-07 //1.23e-07
#define POWER_3508_K2			 	1.453e-07f//2.5500006e-07//6.9500004e-07  //1.453e-07f

#define POWER_6020_K1 			6.0021f//
#define POWER_6020_K2			  -0.0005f//



#define POWER_CONSTANT  4.0810f
#define CAP_POWER_OPEN  200
#define CAP_POWER_CLOSE 5

#define MAX_POWER 100


typedef struct
{
	 fp32 K1;
	 fp32 K2;


	 fp32 constant;//,����
		
	 fp32 Cap_power_open;
	 fp32 Cap_power_close;
	
}Chassis_Power_K_t;



typedef enum
{
	OPEN,
	CLOSE
}SuperPowerState_e;




extern SuperPowerState_e SuperPowerState;

typedef struct
{
	fp32 power_buffer_set;//��������pidĿ��ֵ
	fp32 power_buffer_out;//��������pid�������

	uint16_t grade_power_limit;//��ʱ�ȼ��Ĺ�������
	fp32 Max_input_power;//���������
	fp32 Chassis_Max_power;//���������
	fp32 Chassis_judge_power;
	DebugData chassis_power_data_debug;
	fp32	factor_Course;
	fp32	factor_Drive;
}Chassis_Power_limit_t;



#define RPM_TO_RADPS (2.0f * PI / 60.0f)


extern Chassis_Power_limit_t PowerLimit;
void chassis_power_control(void);
void chassis_power_feedback(Chassis_Power_limit_t *power_control);






#endif
