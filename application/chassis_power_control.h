#ifndef __CHASSIS_POWER_CONTROL_H
#define __CHASSIS_POWER_CONTROL_H

#include "main.h"
#include "vofa_task.h"


#define TOQUE_COEFFICIENT_3508 1.99688994e-6f
#define TOQUE_COEFFICIENT_6020 0.8130f//1.3568115234375e-4f
/*
k1调，使其车堵转，使裁判系统功率达到功率上限附近，超功率时加大k1.没达到时则减小
k2调，使其车小陀螺，使裁判系统功率达到功率上限附近，超功率时加大k2.没达到时则减小

调试原理：公式-P=k1*F*F+b*V*F+k2*V*V+C,(二元一次方程)
因为堵转时速度基本很小，功率主要为力矩决定，所以k1影响大。调k1
小陀螺时速度主导，则调k2
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


	 fp32 constant;//,常数
		
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
	fp32 power_buffer_set;//缓冲能量pid目标值
	fp32 power_buffer_out;//缓冲能量pid输出限制

	uint16_t grade_power_limit;//此时等级的功率限制
	fp32 Max_input_power;//最大功率限制
	fp32 Chassis_Max_power;//底盘最大功率
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
