#include "trig_task.h"
#include "tim.h"
#include "communicate_task.h"
#include "detect_task.h"
#include "key_task.h"

int64_t  trig_ecd_sum=0;
fp32 angle_target;
uint8_t fire_flag = 1;
shoot_control_t trig_control;

static void trig_init(shoot_control_t *shoot_init);

static void trig_feedback_update(shoot_control_t *feedback_update);
static void trig_motor_speed_set(shoot_control_t * control_loop);
static void trig_heat_limit(shoot_control_t *trig_mode);
static void trig_motor_pid_cal(shoot_control_t * control_loop);
static fp32 trig_block_detect_Serial(shoot_control_t * control_loop);//����
static void trig_block_detect_single(shoot_control_t * control_loop,fp32 *angle_set);//����

bool_t trig_detect(void);



void trig_task(void const * argument)
{
		vTaskDelay(TRIG_TASK_INIT_TIME);

    trig_init(&trig_control);

		
		/*-- �жϲ�����2006����Ƿ����� --*/
		while(trig_detect())
		{
			vTaskDelay(TRIG_TASK_CONTROL_TIME);
		}
		while(1)
		{
			trig_feedback_update(&trig_control);			//���������ݸ���
			trig_heat_limit(&trig_control);				//������ģʽ����
			trig_motor_speed_set(&trig_control);				//��ת���+pid���
			trig_motor_pid_cal(&trig_control);
			

			
			if (POWER_OFF)
      {
				CAN_cmd_trig(0);

      }
      else
      {
				
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
	static const fp32 Shoot_trig_speed_pid_cascade[3] = {TRIG_SPEED_PID_KP_CASCADE,TRIG_SPEED_PID_KI_CASCADE,TRIG_SPEED_PID_KD_CASCADE};
	static const fp32 Shoot_trig_angle_pid_cascade[3] = {TRIG_ANGLE_PID_KP_CASCADE,TRIG_ANGLE_PID_KI_CASCADE,TRIG_ANGLE_PID_KD_CASCADE};
	static const fp32 Shoot_trig_speed_pid_single[3]	= {TRIG_SPEED_PID_KP_SINGLE,TRIG_SPEED_PID_KI_SINGLE,TRIG_SPEED_PID_KD_SINGLE};
	
	shoot_init->shoot_trig_motor.shoot_motor_measure = get_gimbal_trigger_motor_measure_point();
	
	//ң��������ָ���ȡ
	shoot_init->rc_data = get_gimbal_rc_data_point();
	shoot_init->trig_referee = get_referee_data_point();

		//��ʼ��Trig����ٶ�pid
	PID_init(&shoot_init->shoot_trig_motor.shoot_speed_pid_cascade,DATA_NORMAL,Shoot_trig_speed_pid_cascade,TRIG_SPEED_PID_MAX_OUT_CASCADE,TRIG_SPEED_PID_MAX_IOUT_CASCADE,NONE);
	PID_init(&shoot_init->shoot_trig_motor.shoot_angle_pid_cascade,DATA_NORMAL,Shoot_trig_angle_pid_cascade,TRIG_ANGLE_PID_MAX_OUT_CASCADE,TRIG_ANGLE_PID_MAX_IOUT_CASCADE,NONE);
	
	PID_init(&shoot_init->shoot_trig_motor.shoot_speed_pid_single,DATA_NORMAL,Shoot_trig_speed_pid_single,TRIG_SPEED_PID_MAX_OUT_SINGLE,TRIG_SPEED_PID_MAX_OUT_SINGLE,NONE);
	
	shoot_init->trig_fire_mode = Serial_fire;//Single_fire;
	
	trig_feedback_update(shoot_init);
}


static void trig_feedback_update(shoot_control_t *feedback_update)
{
	static int16_t trig_ecd_error;
	
	//�����̹�����
	trig_ecd_error = feedback_update->shoot_trig_motor.shoot_motor_measure->ecd -feedback_update->shoot_trig_motor.shoot_motor_measure->last_ecd;
	trig_ecd_error = trig_ecd_error >  4095 ?   trig_ecd_error - 8191 : trig_ecd_error;
	trig_ecd_error = trig_ecd_error < -4095 ?   trig_ecd_error + 8191 : trig_ecd_error;
	trig_ecd_sum += trig_ecd_error;
	
	//�ѵ�ǰת��ecdת��0��pi��Ȼ����Լ��ٱ�36��һȦ8���������������ϲ���һ������Ҫpi/8��ʵ�ʲ�����pi/3����
	feedback_update->shoot_trig_motor.motor_angle = trig_ecd_sum/8191.0f*3.1415926f/36.0f;
	feedback_update->shoot_trig_motor.motor_speed = feedback_update->shoot_trig_motor.shoot_motor_measure->rpm*TRIG_RPM_TO_SPEED_SEN;

}





/**
  * @brief          ��������� + ��ת���
  * @param[out]     control_loop:"shoot_control"����ָ��.
  * @retval         none static
  */
static void trig_motor_speed_set(shoot_control_t * control_loop)
{	 
	static uint8_t mode_flag;
	if(Key_ScanValue.Key_Value.Q)
	{
		if(++mode_flag==2)mode_flag = 0;
		if(mode_flag==0)control_loop->trig_fire_mode = Serial_fire;
		else control_loop->trig_fire_mode = Single_fire;
	}
			/*��ͬ����ģʽ�µ�Ŀ��ֵ�趨*/
		if(control_loop->trig_fire_mode == Serial_fire)//Serial_fire
		{
			control_loop->shoot_trig_motor.motor_speed_set = trig_block_detect_Serial(control_loop);//TRIG_BASE_SPEED;
		}
		else if(control_loop->trig_fire_mode == Single_fire)
		{
			trig_block_detect_single(control_loop,&angle_target);
		}
	if(gimbal_data.FireFlag==0)
	{
		control_loop->shoot_trig_motor.motor_speed_set = 0;
		trig_ecd_sum = 0;
		angle_target = 0;
	}
	
}

/*-- PID���� --*/
static void trig_motor_pid_cal(shoot_control_t * control_loop)
{
	
	if(control_loop->trig_fire_mode == Serial_fire)//Serial_fire
	{
		control_loop->shoot_trig_motor.current_set = PID_calc(&control_loop->shoot_trig_motor.shoot_speed_pid_single,control_loop->shoot_trig_motor.motor_speed,control_loop->shoot_trig_motor.motor_speed_set);
	}
	else if(control_loop->trig_fire_mode == Single_fire)
	{
		PID_calc(&control_loop->shoot_trig_motor.shoot_angle_pid_cascade,angle_target,-control_loop->shoot_trig_motor.motor_angle);
		control_loop->shoot_trig_motor.current_set = PID_calc(&control_loop->shoot_trig_motor.shoot_speed_pid_cascade,control_loop->shoot_trig_motor.motor_speed,control_loop->shoot_trig_motor.shoot_angle_pid_cascade.out);
	}
}
//����ģʽ
static void trig_block_detect_single(shoot_control_t * control_loop,fp32 *angle_set)
{
	if(control_loop->Fire)
	{
		/*-- �յ������־λ --*/
		if(gimbal_data.FireFlag==1)
		{
				if(fire_flag)
				{
					*angle_set = -control_loop->shoot_trig_motor.motor_angle -0.8f;// 
					fire_flag = 0;
				}
		}
		else if(gimbal_data.FireFlag==0)
		{
			fire_flag = 1;
		}
	}
	else
	{
		*angle_set=0;
	}
		/*-- ң�������� --*/
	if(POWER_OFF)
		*angle_set=0;
}

//����ģʽ
static fp32 trig_block_detect_Serial(shoot_control_t * control_loop)
{
	static fp32 trig_speed_set,temp_speed;
	static int16_t   NoMove_counter;
	static uint8_t   NoMove_flag;
	
	//������תһȦ����ȥ8��
	//2006���ٱ�Ϊ36:1
	//rpm/60/36 Ϊ������һ��ת��Ȧ��
	//*8����һ�뷢��
	//����

	/*-- ң�������� --*/
	if(POWER_OFF)
		trig_speed_set = 0;
	
	if(control_loop->Fire)
	{

		/*-- �����׶� --*/
		if(control_loop->ultimate_explosion)
			trig_speed_set = TRIG_EXPLOSION_SPEED;/*-- 30��ÿ�� --*/
		else 
			trig_speed_set = TRIG_BASE_SPEED;			/*-- 15��ÿ�� --*/
		
		temp_speed = trig_speed_set;
		
		/*-- �յ������־λ --*/
		if(gimbal_data.FireFlag)
		{
			/*-- ����ת�� --*/
			if(control_loop->shoot_trig_motor.shoot_motor_measure->rpm >STANDARD_NOMOVE_RPM && NoMove_flag ==0)
			{
				trig_speed_set = temp_speed;
				NoMove_counter =0;
			}
			
			/*-- ����ת�� --*/
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
			/*-- ����ת�� --*/
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
		
		/*-- û���յ������־λ --*/
		else 
		{
				trig_speed_set = 0;
		}
	}
	else
		trig_speed_set = 0;
	return trig_speed_set;
}

uint16_t shoot_time;//����ٺ���
//��ǰ�����������
//�������ÿ100ms��ȴһ�Σ�ÿһ����ȴ����	��ȴֵ/10
//��һ������	+10 
//��ǰ���� + (shoot_time/1000.0f*����Ƶ��)*10 -  ÿ����ȴֵ/10*	shoot_time/1000.0f = ����������� 
//shoot_time(10/1000.0f*����Ƶ�� - ÿ����ȴֵ/10/1000.0f) = ����������� - ��ǰ����
//shoot_time =����������� / (10/1000.0f*����Ƶ�� - ÿ����ȴֵ/10/1000.0f)
static void trig_heat_limit(shoot_control_t *trig_mode)
{
	/*-- ��ȡ����ϵͳ�������� --*/
	uint16_t shoot_cooling_limit = trig_mode->trig_referee->ext_game_robot_state.shooter_cooling_limit;	
	/*-- ��ȡ����ϵͳ��ǰǹ������ --*/
	uint16_t shoot_cooling_heat = trig_mode->trig_referee->ext_power_heat_data.shooter_id1_17mm_cooling_heat; 
	
	uint16_t shoot_cooling_rate = trig_mode->trig_referee->ext_game_robot_state.shooter_cooling_rate;
	
//	//������������Ҫshoot_time����
//	shoot_time = (shoot_cooling_limit - shoot_cooling_heat-40)/(10.f/1000.f * TRIG_BASE_SPEED - shoot_cooling_rate/10.f/1000.f);
//	
//	
//	/*-- ������ֻҪ�������ͻ������������ --*/
//	if(shoot_cooling_heat<(shoot_cooling_limit-30))//����״̬
//	{
//		trig_mode->Fire = 1;
//	}
//	else
//	{
//				/*-- ��������������ʱ����SHIFT+X����󱬷����ŵ��裬�������ռ����ٿ��� --*/
//		if((trig_mode->rc_data->rc_key_v & KEY_PRESSED_SHIFT_X))
//		{
//			trig_mode->Fire = 1;
//			trig_mode->ultimate_explosion = 1;
//		}
//		
//		/*-- дһ��ң����ǿ�Ʒ�����¼��ʱ���˵��� --*/		
//		else if(trig_mode->rc_data->vx_set>300)
//		{
//			trig_mode->Fire = 1;
//		}
//		else
//		{
//			trig_mode->Fire = 0;
//			trig_mode->ultimate_explosion = 0;
//		}
//	}
		

}



bool_t trig_detect(void)
{
	return toe_is_error(TRIGGER_MOTOR_TOE);
}
