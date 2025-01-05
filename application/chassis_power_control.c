#include "chassis_power_control.h"
#include "referee.h"
#include "arm_math.h"
#include "detect_task.h"
#include "chassis_task.h"



Chassis_power_control_t Power_Control;
static void Chassis_power(void);




void chassis_power_control(void);

void chassis_power_init(Chassis_power_control_t *power_control)
{
//	chassis_power_control->Chassis_Power_K.K1 = POWER_K1;
//	chassis_power_control->Chassis_Power_K.K2 = POWER_K2;
//	

//	
//	chassis_power_control->Chassis_Power_K.constant = POWER_CONSTANT;
//	chassis_power_control->SuperPower_state = 0;
//	chassis_power_control->Buffer_set = 60;
//	chassis_power_control->Chassis_Power_K.Cap_power_open=CAP_POWER_OPEN;
//	chassis_power_control->Chassis_Power_K.Cap_power_close=CAP_POWER_CLOSE;
	
		power_control->Chassis_Power_limit.Chassis_Max_power = Referee_System.ext_game_robot_state.chassis_power_limit;

}


void chassis_power_feedback(Chassis_power_control_t *power_control)
{
	get_chassis_power_and_buffer(&power_control->Chassis_Power_limit.Chassis_judge_power,&power_control->Chassis_Power_limit.power_buffer_out);
//	chassis_power_control->Chassis_Power_limit.Chassis_Max_power = MAX_POWER;//Referee_System.ext_game_robot_state.max_chassis_power;
	
//	for(uint8_t i=0;i<4;i++)
//	{
//		chassis_power_control->Chassis_Power_calc.chassis_speed_rpm[i] = chassis_move.motor_chassis[i+4].chassis_motor_measure->rpm;
//		chassis_power_control->Chassis_Power_calc.send_current_value[i] = chassis_move.chassis_course_speed_pid[i].out;
//	}
	//chassis_power_control->Chassis_Power_calc.send_current_value[3] = chassis_move.GM6020_pid.out;
	power_control->Chassis_Power_limit.Chassis_Max_power = Referee_System.ext_game_robot_state.chassis_power_limit;
	Chassis_power();
}

fp32 power[4];
#define RPM_TO_RADPS (2.0f * PI / 60.0f)
void chassis_power_control(void)
{
		float Current_To_Out = 16384.0f / 3.0f;
		fp32 a,b,c,temp;
		fp32 course_consume_power = 0,drive_consume_power = 0;
		fp32 course_initial_give_power[4],drive_initial_give_power[4];
		fp32 sum_available_power,course_available_power,drive_available_power;
		fp32 drive_factor,course_factor;
		sum_available_power = Power_Control.Chassis_Power_limit.Chassis_Max_power;//获取可用功率

		/*--	计算预测的功率		--*/
		for(uint8_t i=0;i<4;i++)
		{

			//3508功率计算
			drive_initial_give_power[i] = TOQUE_COEFFICIENT_3508 * chassis_move.chassis_drive_speed_pid[i].out * chassis_move.motor_chassis[i].chassis_motor_measure->rpm	//,力矩和转速（力矩使用电流发送值代替，因为其是线性关系）
																																	 +POWER_3508_K1 * chassis_move.chassis_drive_speed_pid[i].out * chassis_move.chassis_drive_speed_pid[i].out//,力矩平方
																																	 +POWER_3508_K2 * chassis_move.motor_chassis[i].chassis_motor_measure->rpm * chassis_move.motor_chassis[i].chassis_motor_measure->rpm + POWER_CONSTANT;//,转速平方

			power[i] = drive_initial_give_power[i];
				//正功计入消耗	，负功计入补偿
			if(drive_initial_give_power[i] > 0)
				drive_consume_power +=drive_initial_give_power[i];
			else
				sum_available_power -=drive_initial_give_power[i];

			//6020功率计算
			course_initial_give_power[i] = TOQUE_COEFFICIENT_6020 * chassis_move.chassis_course_speed_pid[i].out / Current_To_Out * chassis_move.motor_chassis[i+4].chassis_motor_measure->rpm * RPM_TO_RADPS		//,力矩和转速（力矩使用电流发送值代替，因为其是线性关系）
																																	 +POWER_6020_K1 * chassis_move.chassis_course_speed_pid[i].out  / Current_To_Out * chassis_move.chassis_course_speed_pid[i].out / Current_To_Out //,力矩平方
																																	 +POWER_6020_K2 * chassis_move.motor_chassis[i+4].chassis_motor_measure->rpm * RPM_TO_RADPS * chassis_move.motor_chassis[i+4].chassis_motor_measure->rpm * RPM_TO_RADPS+1.3715f;// + POWER_CONSTANT;//,转速平方
			//正功计入消耗，负功计入补偿
			if(course_initial_give_power[i] > 0)
				course_consume_power +=course_initial_give_power[i];
			else
				sum_available_power -=course_initial_give_power[i];
		}
		
		/*--	分配总功率	 --*/
		course_available_power = sum_available_power * 0.2f;
		drive_available_power = sum_available_power * 0.8f;
		
		
		/*-- 判断是否超功率 --*/
		if(drive_consume_power > drive_available_power)
			drive_factor = drive_available_power/drive_consume_power;
		else
			drive_factor = 1.0f;
		
		
		if(course_consume_power > course_available_power)
			course_factor = course_available_power/course_consume_power;
		else
			course_factor = 1.0f;
		

			/*--	功率分配 	--*/
			for(uint8_t i=0;i<4;i++)
			{
				if(drive_factor != 1)
				{
					a = POWER_3508_K1;
					b	= TOQUE_COEFFICIENT_3508*chassis_move.motor_chassis[i].chassis_motor_measure->rpm;
					c = POWER_3508_K2*chassis_move.motor_chassis[i].chassis_motor_measure->rpm * chassis_move.motor_chassis[i].chassis_motor_measure->rpm - drive_available_power/4.0f/*drive_initial_give_power[i] * drive_factor*/ + POWER_CONSTANT;	//二元一次方程的c
					
					if(chassis_move.chassis_drive_speed_pid[i].out > 0)
					{
							temp=(-b+sqrt(b*b-4*a*c))/(2*a);//二元一次方程的正解
							if(temp>16000)//限制最大解
								chassis_move.chassis_drive_speed_pid[i].out = 16000;
							else
								chassis_move.chassis_drive_speed_pid[i].out = temp;
					}
					else//向后走取负解
					{
						 temp=(-b-sqrt(b*b-4*a*c))/(2*a);//二元一次方程的负解
						if(temp<-16000)//限制最大解
							chassis_move.chassis_drive_speed_pid[i].out = -16000;
						else
							chassis_move.chassis_drive_speed_pid[i].out = temp;		
					}
				}
		
		
			if(course_factor != 1)
				{
						a = POWER_6020_K1;
						b	= TOQUE_COEFFICIENT_6020 * chassis_move.motor_chassis[i+4].chassis_motor_measure->rpm * RPM_TO_RADPS;//>0
						c = POWER_6020_K2 * chassis_move.motor_chassis[i+4].chassis_motor_measure->rpm * RPM_TO_RADPS * chassis_move.motor_chassis[i+4].chassis_motor_measure->rpm * RPM_TO_RADPS - course_initial_give_power[i] * course_factor+1.3715f;// + POWER_CONSTANT;	//二元一次方程的c
						float h;
						arm_sqrt_f32(b * b - 4 * a * c, &h);
						float result_1, result_2;
						result_1 = (-b + h) / (2.0f * a);
						result_2 = (-b - h) / (2.0f * a);


						 // 两个潜在的可行电流值, 取绝对值最小的那个
						//若是两个结果是异号的，那么一定是result_1的绝对值小，因为b>0，则-b+一个正数的绝对值一定小于-b减一个正数的绝对值
						 if ((result_1 > 0.0f && result_2 < 0.0f) || (result_1 < 0.0f && result_2 > 0.0f))
						 {
								 if ((chassis_move.chassis_course_speed_pid[i].out > 0.0f && result_1 > 0.0f) || (chassis_move.chassis_course_speed_pid[i].out < 0.0f && result_1 < 0.0f))
								 {
										 chassis_move.chassis_course_speed_pid[i].out = result_1* Current_To_Out;
								 }
								 else
								 {
										 chassis_move.chassis_course_speed_pid[i].out = result_2* Current_To_Out;
								 }
						 }
						 //若是两个解都大于0，那就取小的
						 else
						 {
								 if (Math_Abs(result_1) < Math_Abs(result_2))
								 {
										 chassis_move.chassis_course_speed_pid[i].out = result_1* Current_To_Out;
								 }
								 else
								 {
										 chassis_move.chassis_course_speed_pid[i].out = result_2* Current_To_Out;
								 }
						 }
				 }
		}
}

static void Chassis_power(void)
{
	Power_Control.chassis_power_data_debug.data1 = chassis_move.motor_chassis[0].chassis_motor_measure->rpm;	
	Power_Control.chassis_power_data_debug.data2 = chassis_move.motor_chassis[1].chassis_motor_measure->rpm;	
	Power_Control.chassis_power_data_debug.data3 = chassis_move.motor_chassis[2].chassis_motor_measure->rpm;	
	
	Power_Control.chassis_power_data_debug.data4 = chassis_move.motor_chassis[3].chassis_motor_measure->rpm;	
	Power_Control.chassis_power_data_debug.data5 = Power_Control.Chassis_Power_limit.Chassis_judge_power;
	Power_Control.chassis_power_data_debug.data6 = Power_Control.Chassis_Power_limit.Chassis_Max_power;
}

const DebugData * get_chassis_power(void)
{
	return &Power_Control.chassis_power_data_debug;
}






