#ifndef __CHASSIS_POWER_CONTROL_H
#define __CHASSIS_POWER_CONTROL_H

#include "main.h"

#define TOQUE_COEFFICIENT 1.99688994e-6f
/*
k1调，使其车堵转，使裁判系统功率达到功率上限附近，超功率时加大k1.没达到时则减小
k2调，使其车小陀螺，使裁判系统功率达到功率上限附近，超功率时加大k2.没达到时则减小

调试原理：公式-P=k1*F*F+b*V*F+k2*V*V+C,(二元一次方程)
因为堵转时速度基本很小，功率主要为力矩决定，所以k1影响大。调k1
小陀螺时速度主导，则调k2
*/
#define POWER_K1 			  2.09999911e-07
#define POWER_K2			  2.95000007e-07  //1.453e-07f
#define POWER_CONSTANT  4.081f
#define CAP_POWER_OPEN  200
#define CAP_POWER_CLOSE 5



typedef struct
{
	 fp32 K1;
	 fp32 K2;


	 fp32 constant;//,常数
		
	 fp32 Cap_power_open;
	 fp32 Cap_power_close;
	
}Chassis_Power_K_t;


typedef struct
{
	 fp32 power_buffer_set;//缓冲能量pid目标值
	 fp32 power_buffer_out;//缓冲能量pid输出限制
		
	 uint16_t grade_power_limit;//此时等级的功率限制
	 fp32 Max_input_power;//最大功率限制
	 fp32 Chassis_Max_power;//底盘最大功率
	 fp32 Chassis_judge_power;
}Chassis_Power_limit_t;



typedef struct
{
	fp32 chassis_speed_rpm[4];//四个轮子速度反馈
	fp32 initial_total_power;//计算得到的总功率
	fp32 initial_give_power[4];//四个轮子计算后的功率
	fp32 scaled_give_power[4];//四个轮子缩放后的功率
	fp32 scaled_total_power;//缩放后的总功率
	fp32 send_current_value[4];//pid计算计算得到的发送电流值
}Chassis_Power_calc_t;


typedef struct
{
	uint8_t SuperPower_state;
	fp32 Buffer_set;
	Chassis_Power_calc_t Chassis_Power_calc;
	Chassis_Power_limit_t Chassis_Power_limit;
	Chassis_Power_K_t Chassis_Power_K;
}Chassis_power_control_t;



extern Chassis_power_control_t Chassis_power_control;
void chassis_power_control(Chassis_power_control_t *power_control);
void chassis_power_feedback(Chassis_power_control_t *chassis_power_control);
void chassis_power_init(Chassis_power_control_t *chassis_power_control);

#endif
