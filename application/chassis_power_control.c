#include "chassis_power_control.h"
#include "referee.h"
#include "arm_math.h"
#include "detect_task.h"
#include "chassis_task.h"



Chassis_power_control_t Chassis_power_control;
static void Chassis_power(void);




void chassis_power_control(void);

void chassis_power_init(Chassis_power_control_t *chassis_power_control)
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
	
		chassis_power_control->Chassis_Power_limit.Chassis_Max_power = MAX_POWER;//Referee_System.ext_game_robot_state.max_chassis_power;

}


void chassis_power_feedback(Chassis_power_control_t *chassis_power_control)
{
	get_chassis_power_and_buffer(&chassis_power_control->Chassis_Power_limit.Chassis_judge_power,&chassis_power_control->Chassis_Power_limit.power_buffer_out);
//	chassis_power_control->Chassis_Power_limit.Chassis_Max_power = MAX_POWER;//Referee_System.ext_game_robot_state.max_chassis_power;
	
//	for(uint8_t i=0;i<4;i++)
//	{
//		chassis_power_control->Chassis_Power_calc.chassis_speed_rpm[i] = chassis_move.motor_chassis[i+4].chassis_motor_measure->rpm;
//		chassis_power_control->Chassis_Power_calc.send_current_value[i] = chassis_move.chassis_course_speed_pid[i].out;
//	}
	//chassis_power_control->Chassis_Power_calc.send_current_value[3] = chassis_move.GM6020_pid.out;
	Chassis_power();
}


#define RPM_TO_RADPS (2.0f * PI / 60.0f)
void chassis_power_control(void)
{
		float Current_To_Out = 16384.0f / 3.0f;
		fp32 a,b,c,temp;
		fp32 course_consume_power,drive_consume_power;
		fp32 course_initial_give_power[4],drive_initial_give_power[4];
		fp32 sum_available_power,course_available_power,drive_available_power;
		fp32 drive_factor,course_factor;
		sum_available_power = Chassis_power_control.Chassis_Power_limit.Chassis_Max_power;//��ȡ���ù���

		/*--	����Ԥ��Ĺ���		--*/
		for(uint8_t i=0;i<4;i++)
		{
			//3508���ʼ���
			drive_initial_give_power[i] = TOQUE_COEFFICIENT_3508 * chassis_move.chassis_drive_speed_pid[i].out * chassis_move.motor_chassis[i].chassis_motor_measure->rpm	//,���غ�ת�٣�����ʹ�õ�������ֵ���棬��Ϊ�������Թ�ϵ��
																																	 +POWER_3508_K1 * chassis_move.chassis_drive_speed_pid[i].out * chassis_move.chassis_drive_speed_pid[i].out//,����ƽ��
																																	 +POWER_3508_K2 * chassis_move.motor_chassis[i].chassis_motor_measure->rpm * chassis_move.motor_chassis[i].chassis_motor_measure->rpm + POWER_CONSTANT;//,ת��ƽ��
			//������������	���������벹��
			if(drive_initial_give_power[i] > 0)
				drive_consume_power +=drive_initial_give_power[i];
			else
				sum_available_power -=drive_initial_give_power[i];
				
			//6020���ʼ���
			course_initial_give_power[i] = TOQUE_COEFFICIENT_6020 * chassis_move.chassis_course_speed_pid[i].out / Current_To_Out * chassis_move.motor_chassis[i+4].chassis_motor_measure->rpm * RPM_TO_RADPS		//,���غ�ת�٣�����ʹ�õ�������ֵ���棬��Ϊ�������Թ�ϵ��
																																	 +POWER_6020_K1 * chassis_move.chassis_course_speed_pid[i].out  / Current_To_Out * chassis_move.chassis_course_speed_pid[i].out / Current_To_Out //,����ƽ��
																																	 +POWER_6020_K2 * chassis_move.motor_chassis[i+4].chassis_motor_measure->rpm * RPM_TO_RADPS * chassis_move.motor_chassis[i+4].chassis_motor_measure->rpm * RPM_TO_RADPS+1.3715f;// + POWER_CONSTANT;//,ת��ƽ��
			//�����������ģ��������벹��
			if(course_initial_give_power[i] > 0)
				course_consume_power +=course_initial_give_power[i];
			else
				sum_available_power -=course_initial_give_power[i];
		}
		
		/*--	�����ܹ���	 --*/
		course_available_power = sum_available_power * 0.4f;
		drive_available_power = sum_available_power * 0.6f;
		
		
		/*-- �ж��Ƿ񳬹��� --*/
		if(drive_consume_power > drive_available_power)
			drive_factor = drive_available_power/drive_consume_power;
		else
			drive_factor = 1.0f;
		
		
		if(course_consume_power > course_available_power)
			course_factor = course_available_power/course_consume_power;
		else
			course_factor = 1.0f;
		
		
		/*--	���ʷ��� 	--*/
		for(uint8_t i=0;i<4;i++)
		{
			
			a = POWER_3508_K1;
			b	= TOQUE_COEFFICIENT_3508*chassis_move.motor_chassis[i].chassis_motor_measure->rpm;
			c = POWER_3508_K2*chassis_move.motor_chassis[i].chassis_motor_measure->rpm * chassis_move.motor_chassis[i].chassis_motor_measure->rpm - drive_initial_give_power[i] * drive_factor + POWER_CONSTANT;	//��Ԫһ�η��̵�c
			
			if(chassis_move.chassis_drive_speed_pid[i].out > 0)
			{
					temp=(-b+sqrt(b*b-4*a*c))/(2*a);//��Ԫһ�η��̵�����
					if(temp>16000)//��������
						chassis_move.chassis_drive_speed_pid[i].out = 16000;
					else
						chassis_move.chassis_drive_speed_pid[i].out = temp;
			}
			else//�����ȡ����
			{
				 temp=(-b-sqrt(b*b-4*a*c))/(2*a);//��Ԫһ�η��̵ĸ���
				if(temp<-16000)//��������
					chassis_move.chassis_drive_speed_pid[i].out = -16000;
				else
					chassis_move.chassis_drive_speed_pid[i].out = temp;		
			}

			a = POWER_6020_K1;
			b	= TOQUE_COEFFICIENT_6020 * chassis_move.motor_chassis[i+4].chassis_motor_measure->rpm * RPM_TO_RADPS;
			c = POWER_6020_K2 * chassis_move.motor_chassis[i+4].chassis_motor_measure->rpm * RPM_TO_RADPS * chassis_move.motor_chassis[i+4].chassis_motor_measure->rpm * RPM_TO_RADPS - course_initial_give_power[i] * course_factor+1.3715f;// + POWER_CONSTANT;	//��Ԫһ�η��̵�c
			
			if(chassis_move.chassis_course_speed_pid[i].out > 0)
			{
					temp=(-b+sqrt(b*b-4*a*c))/(2*a);//��Ԫһ�η��̵�����
					if(temp>3)//��������
						chassis_move.chassis_course_speed_pid[i].out = 16000;
					else
						chassis_move.chassis_course_speed_pid[i].out = temp * Current_To_Out ;
			}
			else//�����ȡ����
			{
				 temp=(-b-sqrt(b*b-4*a*c))/(2*a);//��Ԫһ�η��̵ĸ���
				if(temp<-3)//��������
					chassis_move.chassis_course_speed_pid[i].out = -16000;
				else
					chassis_move.chassis_course_speed_pid[i].out = temp * Current_To_Out;		
			}
		}
}

static void Chassis_power(void)
{
	Chassis_power_control.chassis_power_data_debug.data1 = Chassis_power_control.Chassis_Power_limit.Chassis_Max_power;			//�������������
	Chassis_power_control.chassis_power_data_debug.data2 = Chassis_power_control.Chassis_Power_limit.Chassis_judge_power;		//����ϵͳ�������Ĺ���
	Chassis_power_control.chassis_power_data_debug.data3 = Chassis_power_control.Chassis_Power_calc.scaled_total_power;			//���ź���ܹ���
	
	Chassis_power_control.chassis_power_data_debug.data4 = Chassis_power_control.Chassis_Power_calc.initial_total_power;		//����ǰ���ܹ���
	Chassis_power_control.chassis_power_data_debug.data5 = 0;
	Chassis_power_control.chassis_power_data_debug.data6 = 0;
}

const DebugData * get_chassis_power(void)
{
	return &Chassis_power_control.chassis_power_data_debug;
}



