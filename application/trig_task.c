#include "trig_task.h"
#include "tim.h"
#include "communicate_task.h"
#include "detect_task.h"

#ifdef CASCADE
static int64_t  trig_ecd_sum=0;
#endif

shoot_control_t trig_control;

static void trig_init(shoot_control_t *shoot_init);

static void trig_feedback_update(shoot_control_t *feedback_update);
static void trig_motor_control(shoot_control_t * control_loop);
void trig_motor_mode_set(shoot_control_t *shoot_mode);
static fp32 trig_block_detect(shoot_control_t * control_loop);
bool_t trig_detect(void);



void trig_task(void const * argument)
{
		vTaskDelay(TRIG_TASK_INIT_TIME);

    trig_init(&trig_control);

		
		//�жϲ�����2006����Ƿ�����
		while(trig_detect())
		{
			vTaskDelay(TRIG_TASK_CONTROL_TIME);
		}
		while(1)
		{
			trig_motor_mode_set(&trig_control);				//������ģʽ����
		  trig_feedback_update(&trig_control);			//���������ݸ���
			trig_motor_control(&trig_control);				//��ת���+pid���

			if (rc_is_error())
      {
				CAN_cmd_trig(0);

      }
      else
      {
				//CAN_cmd_trig(0);
				CAN_cmd_trig(trig_control.shoot_trig_motor.current_set);
      }
			osDelay(1);
		
		}



}


/**
	* @brief          ���������ʼ��
  * @param[out]     shoot_init:"shoot_control"����ָ��.
  * @retval         none
  */
static void trig_init(shoot_control_t *shoot_init)
{
//	static const fp32 Shoot_trig_speed_pid_cascade[3] = {TRIG_SPEED_PID_KP_CASCADE,TRIG_SPEED_PID_KI_CASCADE,TRIG_SPEED_PID_KD_CASCADE};
//	static const fp32 Shoot_trig_angle_pid_cascade[3] = {TRIG_ANGLE_PID_KP_CASCADE,TRIG_ANGLE_PID_KI_CASCADE,TRIG_ANGLE_PID_KD_CASCADE};
	static const fp32 Shoot_trig_speed_pid_single[3]	= {TRIG_SPEED_PID_KP_SINGLE,TRIG_SPEED_PID_KI_SINGLE,TRIG_SPEED_PID_KD_SINGLE};
	
	shoot_init->shoot_trig_motor.shoot_motor_measure = get_gimbal_trigger_motor_measure_point();
	
	//ң��������ָ���ȡ
	shoot_init->rc_data = get_gimbal_rc_data_point();
//	shoot_init->is_fire = get_autofire_flag_point();
	shoot_init->trig_referee = get_referee_data_point();

	//	//��ʼ��Trig����ٶ�pid
//	PID_init(&shoot_init->shoot_trig_motor.shoot_speed_pid_cascade,DATA_NORMAL,Shoot_trig_speed_pid_cascade,TRIG_SPEED_PID_MAX_OUT_CASCADE,TRIG_SPEED_PID_MAX_IOUT_CASCADE,NONE);
//	PID_init(&shoot_init->shoot_trig_motor.shoot_angle_pid_cascade,DATA_NORMAL,Shoot_trig_angle_pid_cascade,TRIG_ANGLE_PID_MAX_OUT_CASCADE,TRIG_ANGLE_PID_MAX_IOUT_CASCADE,NONE);
	
	PID_init(&shoot_init->shoot_trig_motor.shoot_speed_pid_single,DATA_NORMAL,Shoot_trig_speed_pid_single,TRIG_SPEED_PID_MAX_OUT_SINGLE,TRIG_SPEED_PID_MAX_OUT_SINGLE,NONE);
	
	shoot_init->trig_fire_mode = Cease_fire;
	
	trig_feedback_update(shoot_init);
}


static void trig_feedback_update(shoot_control_t *feedback_update)
{
	feedback_update->shoot_trig_motor.motor_speed = feedback_update->shoot_trig_motor.shoot_motor_measure->rpm*TRIG_RPM_TO_SPEED_SEN;

}





/**
  * @brief          ��������� + ��ת���
  * @param[out]     control_loop:"shoot_control"����ָ��.
  * @retval         none
  */
static void trig_motor_control(shoot_control_t * control_loop)
{

	control_loop->shoot_trig_motor.motor_speed_set = trig_block_detect(control_loop);
	
	
	control_loop->shoot_trig_motor.current_set = PID_calc(&control_loop->shoot_trig_motor.shoot_speed_pid_single,control_loop->shoot_trig_motor.motor_speed,control_loop->shoot_trig_motor.motor_speed_set);
}





static fp32 trig_block_detect(shoot_control_t * control_loop)
{
	static fp32 trig_speed_set,temp_speed;
	static int16_t   NoMove_counter;
	static uint8_t   NoMove_flag;
	static uint8_t 	 Fire;
	static uint8_t 	 ultimate_explosion;
	//������תһȦ����ȥ8��
	//2006���ٱ�Ϊ36:1
	//rpm/60/36 Ϊ������һ��ת��Ȧ��
	//*8����һ�뷢��
	//����
	if(control_loop->trig_fire_mode == Start_fire)
	{
		Fire = 1;
	}
	else if(control_loop->trig_fire_mode == Cease_fire)
	{
		if(control_loop->rc_data->rc_key_v & KEY_PRESSED_SHIFT_Q)
		{
			Fire = 1;
			ultimate_explosion = 1;
		}
		else
		{
			Fire = 0;
			ultimate_explosion = 0;
		}
	}
//	else if(control_loop->trig_fire_mode == Warning)
//	{
//		if(control_loop->rc_data->rc_key_v & KEY_PRESSED_OFFSET_Q)
//		{
//			Fire = 1;
//		}
//		else
//		{
//			Fire = 0;
//		}
//	}
//	else if(control_loop->trig_fire_mode == danger_state)
//	{
//		if(control_loop->rc_data->rc_key_v & KEY_PRESSED_SHIFT_Q)
//		{
//			Fire = 1;
//		}
//		else
//		{
//			Fire = 0;
//		}
//	}
		//ң��������
	if(rc_is_error())
		trig_speed_set = 0;
	
	if(Fire)
	{
		if(ultimate_explosion)
			trig_speed_set = TRIG_EXPLOSION_SPEED;
		else 
			trig_speed_set = TRIG_BASE_SPEED;
		
		temp_speed = trig_speed_set;
		
			if(gimbal_data.FireFlag)
			{
				if(control_loop->shoot_trig_motor.shoot_motor_measure->rpm >STANDARD_NOMOVE_RPM && NoMove_flag ==0)
				{
					trig_speed_set = temp_speed;
					NoMove_counter =0;
				}
				//����ת��
				else if( control_loop->shoot_trig_motor.shoot_motor_measure->rpm <= STANDARD_NOMOVE_RPM && NoMove_flag ==0 )
				{
					trig_speed_set = temp_speed;						
					NoMove_counter ++;
					if( NoMove_counter >= STANDARD_NOMOVE_TIME ) 
					{
						NoMove_flag = 1;
						NoMove_counter = STANDARD_REMOVE_TIME;
					}	
				}
				//����ת��
				else if( NoMove_flag != 0)
				{
					trig_speed_set =-TRIG_BACK_SPEED;
					
					NoMove_counter--;
					
					if(NoMove_counter <= 0)
					{
					NoMove_flag = 0;
					NoMove_counter = 0;
					}
				}
	
			}
			else 
			{
					trig_speed_set = 0;
			}
	}
	else 
	{
		trig_speed_set = 0;
	}

	
	return trig_speed_set;
}



void trig_motor_mode_set(shoot_control_t *trig_mode)
{
	uint16_t shoot_cooling_limit = trig_mode->trig_referee->ext_game_robot_state.shooter_cooling_limit;	//��������
	uint16_t shoot_cooling_heat = trig_mode->trig_referee->ext_power_heat_data.shooter_id1_17mm_cooling_heat; //��ǰǹ������
	
	if(shoot_cooling_heat<(shoot_cooling_limit-20))//����״̬
	{
		trig_mode->trig_fire_mode = Start_fire;
	}
	else
	{
		trig_mode->trig_fire_mode = Cease_fire;
	}
//	else if(shoot_cooling_heat>=shoot_cooling_limit && shoot_cooling_limit<(2*shoot_cooling_limit-20))//����״̬����Ѫ����
//	{
//		trig_mode->trig_fire_mode = Warning;
//	}
//	else if(shoot_cooling_heat>=shoot_cooling_limit*2 )//Σ��״̬����Ѫ�ܶ�
//	{
//		trig_mode->trig_fire_mode = danger_state;
//	}
	
	trig_mode->last_trig_fire_mode = trig_mode->trig_fire_mode;


}



bool_t trig_detect(void)
{
	return toe_is_error(TRIGGER_MOTOR_TOE);
}
