#include "chassis_power_control.h"
#include "referee.h"
#include "arm_math.h"
#include "detect_task.h"
#include "chassis_task.h"


Chassis_power_control_t Chassis_power_control;




void chassis_power_control(Chassis_power_control_t *power_control);
fp32 chassis_RobotLevel_power_limit(uint8_t grade);

void chassis_power_init(Chassis_power_control_t *chassis_power_control)
{
	chassis_power_control->Chassis_Power_K.K1 = POWER_K1;
	chassis_power_control->Chassis_Power_K.K2 = POWER_K2;
	chassis_power_control->Chassis_Power_K.constant = POWER_CONSTANT;
	chassis_power_control->SuperPower_state = 0;
	chassis_power_control->Buffer_set = 60;
	chassis_power_control->Chassis_Power_K.Cap_power_open=CAP_POWER_OPEN;
	chassis_power_control->Chassis_Power_K.Cap_power_close=CAP_POWER_CLOSE;
}


void chassis_power_feedback(Chassis_power_control_t *chassis_power_control)
{
	get_chassis_power_and_buffer(&chassis_power_control->Chassis_Power_limit.Chassis_judge_power,&chassis_power_control->Chassis_Power_limit.power_buffer_out);
	chassis_power_control->Chassis_Power_limit.Chassis_Max_power = 50;//Referee_System.ext_game_robot_state.max_chassis_power;
	
	for(uint8_t i=0;i<4;i++)
	{
		chassis_power_control->Chassis_Power_calc.chassis_speed_rpm[i] = chassis_move.motor_chassis[i].chassis_motor_measure->rpm;
		chassis_power_control->Chassis_Power_calc.send_current_value[i] = chassis_move.chassis_drive_speed_pid[i].out;
	}
	
}



void chassis_power_control(Chassis_power_control_t *power_control)
{

    uint8_t robot_id = get_robot_id();
	

		
		for(uint8_t i=0;i<4;i++)//当前电机功率模型计算
			 power_control->Chassis_Power_calc.initial_give_power[i] = TOQUE_COEFFICIENT*(power_control->Chassis_Power_calc.send_current_value[i])*(power_control->Chassis_Power_calc.chassis_speed_rpm[i])//,力矩和转速（力矩使用电流发送值代替，因为其是线性关系）
																																 +power_control->Chassis_Power_K.K1*power_control->Chassis_Power_calc.send_current_value[i]*power_control->Chassis_Power_calc.send_current_value[i]//,力矩平方
																																 +power_control->Chassis_Power_K.K2*power_control->Chassis_Power_calc.chassis_speed_rpm[i]*power_control->Chassis_Power_calc.chassis_speed_rpm[i];//,转速平方

				//当前四个3508总功率
				power_control->Chassis_Power_calc.initial_total_power=power_control->Chassis_Power_calc.initial_give_power[0]+power_control->Chassis_Power_calc.initial_give_power[1]
																			+power_control->Chassis_Power_calc.initial_give_power[2]+power_control->Chassis_Power_calc.initial_give_power[3];//,当前总功率（4个3508加起来）

		//计算出来的功率大于最大功率时进行功率限制和分配
		if(power_control->Chassis_Power_calc.initial_total_power > power_control->Chassis_Power_limit.Chassis_Max_power)
		{
			float a,b,c,temp;
			float power_scale=power_control->Chassis_Power_limit.Chassis_Max_power / power_control->Chassis_Power_calc.initial_total_power;//缩放系数计算
			
			//计算缩放之后的功率
			for(uint8_t i=0;i<4;i++)
			{
				power_control->Chassis_Power_calc.scaled_give_power[i] = power_control->Chassis_Power_calc.initial_give_power[i] * power_scale;
			}
			
			//缩放之后的总功率
			power_control->Chassis_Power_calc.scaled_total_power = power_control->Chassis_Power_calc.scaled_give_power[0] + power_control->Chassis_Power_calc.scaled_give_power[1] + power_control->Chassis_Power_calc.scaled_give_power[2] + power_control->Chassis_Power_calc.scaled_give_power[3];
			
			a = power_control->Chassis_Power_K.K1;//二元一次方程的a
			
			for(uint8_t i = 0;i<4;i++)
			{
				b = TOQUE_COEFFICIENT*power_control->Chassis_Power_calc.chassis_speed_rpm[i];//二元一次方程的b
				c = power_control->Chassis_Power_K.K2*power_control->Chassis_Power_calc.chassis_speed_rpm[i]*power_control->Chassis_Power_calc.chassis_speed_rpm[i]-power_control->Chassis_Power_calc.scaled_give_power[i];//，二元一次方程的c
				if(power_control->Chassis_Power_calc.send_current_value[i]>0)//,向前走取正解
				{
					 temp=(-b+sqrt(b*b-4*a*c))/(2*a);//二元一次方程的正解
					if(temp>16000)//，限制最大解
						power_control->Chassis_Power_calc.send_current_value[i]=16000;
					else
						power_control->Chassis_Power_calc.send_current_value[i]=temp;
				}
				else//,向后走取负解
				{
					 temp=(-b-sqrt(b*b-4*a*c))/(2*a);//二元一次方程的负解
					if(temp<-16000)//，限制最大解
						power_control->Chassis_Power_calc.send_current_value[i]=-16000;
					else
						power_control->Chassis_Power_calc.send_current_value[i]=temp;		
				}
			}
				
			
			
		}

}


