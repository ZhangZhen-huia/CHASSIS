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
static fp32 trig_block_detect_Serial(shoot_control_t * control_loop);//连发
static void trig_block_detect_single(shoot_control_t * control_loop,fp32 *angle_set);//单发

bool_t trig_detect(void);



void trig_task(void const * argument)
{
		vTaskDelay(TRIG_TASK_INIT_TIME);

    trig_init(&trig_control);

		
		/*-- 判断拨弹盘2006电机是否在线 --*/
		while(trig_detect())
		{
			vTaskDelay(TRIG_TASK_CONTROL_TIME);
		}
		while(1)
		{
			trig_feedback_update(&trig_control);			//发射电机数据更新
			trig_heat_limit(&trig_control);				//拨弹盘模式设置
			trig_motor_speed_set(&trig_control);				//堵转检测+pid输出
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
	* @brief          发射机构初始化
  * @param[out]     shoot_init:"shoot_control"变量指针.
  * @retval         none
  */
static void trig_init(shoot_control_t *shoot_init)
{
	static const fp32 Shoot_trig_speed_pid_cascade[3] = {TRIG_SPEED_PID_KP_CASCADE,TRIG_SPEED_PID_KI_CASCADE,TRIG_SPEED_PID_KD_CASCADE};
	static const fp32 Shoot_trig_angle_pid_cascade[3] = {TRIG_ANGLE_PID_KP_CASCADE,TRIG_ANGLE_PID_KI_CASCADE,TRIG_ANGLE_PID_KD_CASCADE};
	static const fp32 Shoot_trig_speed_pid_single[3]	= {TRIG_SPEED_PID_KP_SINGLE,TRIG_SPEED_PID_KI_SINGLE,TRIG_SPEED_PID_KD_SINGLE};
	
	shoot_init->shoot_trig_motor.shoot_motor_measure = get_gimbal_trigger_motor_measure_point();
	
	//遥控器数据指针获取
	shoot_init->rc_data = get_gimbal_rc_data_point();
	shoot_init->trig_referee = get_referee_data_point();

		//初始化Trig电机速度pid
	PID_init(&shoot_init->shoot_trig_motor.shoot_speed_pid_cascade,DATA_NORMAL,Shoot_trig_speed_pid_cascade,TRIG_SPEED_PID_MAX_OUT_CASCADE,TRIG_SPEED_PID_MAX_IOUT_CASCADE,NONE);
	PID_init(&shoot_init->shoot_trig_motor.shoot_angle_pid_cascade,DATA_NORMAL,Shoot_trig_angle_pid_cascade,TRIG_ANGLE_PID_MAX_OUT_CASCADE,TRIG_ANGLE_PID_MAX_IOUT_CASCADE,NONE);
	
	PID_init(&shoot_init->shoot_trig_motor.shoot_speed_pid_single,DATA_NORMAL,Shoot_trig_speed_pid_single,TRIG_SPEED_PID_MAX_OUT_SINGLE,TRIG_SPEED_PID_MAX_OUT_SINGLE,NONE);
	
	shoot_init->trig_fire_mode = Serial_fire;//Single_fire;
	
	trig_feedback_update(shoot_init);
}


static void trig_feedback_update(shoot_control_t *feedback_update)
{
	static int16_t trig_ecd_error;
	
	//拨弹盘过零检测
	trig_ecd_error = feedback_update->shoot_trig_motor.shoot_motor_measure->ecd -feedback_update->shoot_trig_motor.shoot_motor_measure->last_ecd;
	trig_ecd_error = trig_ecd_error >  4095 ?   trig_ecd_error - 8191 : trig_ecd_error;
	trig_ecd_error = trig_ecd_error < -4095 ?   trig_ecd_error + 8191 : trig_ecd_error;
	trig_ecd_sum += trig_ecd_error;
	
	//把当前转的ecd转成0到pi，然后除以减速比36，一圈8个弹，所以理论上拨出一个弹需要pi/8，实际测试在pi/3左右
	feedback_update->shoot_trig_motor.motor_angle = trig_ecd_sum/8191.0f*3.1415926f/36.0f;
	feedback_update->shoot_trig_motor.motor_speed = feedback_update->shoot_trig_motor.shoot_motor_measure->rpm*TRIG_RPM_TO_SPEED_SEN;

}





/**
  * @brief          拨弹盘输出 + 堵转检测
  * @param[out]     control_loop:"shoot_control"变量指针.
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
			/*不同发射模式下的目标值设定*/
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

/*-- PID计算 --*/
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
//单发模式
static void trig_block_detect_single(shoot_control_t * control_loop,fp32 *angle_set)
{
	if(control_loop->Fire)
	{
		/*-- 收到开火标志位 --*/
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
		/*-- 遥控器掉线 --*/
	if(POWER_OFF)
		*angle_set=0;
}

//连发模式
static fp32 trig_block_detect_Serial(shoot_control_t * control_loop)
{
	static fp32 trig_speed_set,temp_speed;
	static int16_t   NoMove_counter;
	static uint8_t   NoMove_flag;
	
	//拨弹盘转一圈拨出去8个
	//2006减速比为36:1
	//rpm/60/36 为拨弹盘一秒转的圈数
	//*8等于一秒发弹
	//开火

	/*-- 遥控器掉线 --*/
	if(POWER_OFF)
		trig_speed_set = 0;
	
	if(control_loop->Fire)
	{

		/*-- 爆发阶段 --*/
		if(control_loop->ultimate_explosion)
			trig_speed_set = TRIG_EXPLOSION_SPEED;/*-- 30发每秒 --*/
		else 
			trig_speed_set = TRIG_BASE_SPEED;			/*-- 15发每秒 --*/
		
		temp_speed = trig_speed_set;
		
		/*-- 收到开火标志位 --*/
		if(gimbal_data.FireFlag)
		{
			/*-- 正常转动 --*/
			if(control_loop->shoot_trig_motor.shoot_motor_measure->rpm >STANDARD_NOMOVE_RPM && NoMove_flag ==0)
			{
				trig_speed_set = temp_speed;
				NoMove_counter =0;
			}
			
			/*-- 卡弹转动 --*/
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
			/*-- 反向转动 --*/
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
		
		/*-- 没有收到开火标志位 --*/
		else 
		{
				trig_speed_set = 0;
		}
	}
	else
		trig_speed_set = 0;
	return trig_speed_set;
}

uint16_t shoot_time;//打多少毫秒
//当前最大热量限制
//射击热量每100ms冷却一次，每一次冷却热量	冷却值/10
//打一发弹丸	+10 
//当前热量 + (shoot_time/1000.0f*波胆频率)*10 -  每秒冷却值/10*	shoot_time/1000.0f = 最大热量限制 
//shoot_time(10/1000.0f*波胆频率 - 每秒冷却值/10/1000.0f) = 最大热量限制 - 当前热量
//shoot_time =最大热量限制 / (10/1000.0f*波胆频率 - 每秒冷却值/10/1000.0f)
static void trig_heat_limit(shoot_control_t *trig_mode)
{
	/*-- 获取裁判系统热量限制 --*/
	uint16_t shoot_cooling_limit = trig_mode->trig_referee->ext_game_robot_state.shooter_cooling_limit;	
	/*-- 获取裁判系统当前枪口热量 --*/
	uint16_t shoot_cooling_heat = trig_mode->trig_referee->ext_power_heat_data.shooter_id1_17mm_cooling_heat; 
	
	uint16_t shoot_cooling_rate = trig_mode->trig_referee->ext_game_robot_state.shooter_cooling_rate;
	
//	//打到热量限制需要shoot_time毫秒
//	shoot_time = (shoot_cooling_limit - shoot_cooling_heat-40)/(10.f/1000.f * TRIG_BASE_SPEED - shoot_cooling_rate/10.f/1000.f);
//	
//	
//	/*-- 新赛季只要超热量就会锁定发射机构 --*/
//	if(shoot_cooling_heat<(shoot_cooling_limit-30))//正常状态
//	{
//		trig_mode->Fire = 1;
//	}
//	else
//	{
//				/*-- 即将超热量，此时按下SHIFT+X会最后爆发几颗弹丸，比赛最终几秒再开启 --*/
//		if((trig_mode->rc_data->rc_key_v & KEY_PRESSED_SHIFT_X))
//		{
//			trig_mode->Fire = 1;
//			trig_mode->ultimate_explosion = 1;
//		}
//		
//		/*-- 写一个遥控器强制发弹检录的时候退弹用 --*/		
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
