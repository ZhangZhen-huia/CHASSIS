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
static void shoot_control_loop(shoot_control_t *control_loop);
void shoot_trig_motor_mode_set(shoot_control_t *shoot_mode);
int64_t trig_block_detect_cascade(shoot_control_t * control_loop,int64_t angle_set);
static void shoot_motor_control(shoot_motor_t *shoot_motor);
static void shoot_trig_motor_behaviour_set(shoot_control_t *shoot_behaviour);
static fp32 trig_block_detect_single(shoot_control_t * control_loop);
bool_t trig_detect(void);



void trig_task(void const * argument)
{
		vTaskDelay(TRIG_TASK_INIT_TIME);

    trig_init(&trig_control);

		
		//判断拨弹盘2006电机是否在线
		while(trig_detect())
		{
			vTaskDelay(TRIG_TASK_CONTROL_TIME);
		}
		while(1)
		{
		  trig_feedback_update(&trig_control);			//发射电机数据更新
			trig_motor_control(&trig_control);					//拨弹盘控制量配置
			shoot_control_loop(&trig_control);					//摩擦轮和拨弹盘输出值计算

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
//	shoot_init->is_fire = get_autofire_flag_point();
	shoot_init->shoot_referee = get_referee_data_point();
	//初始化Trig电机速度pid
	PID_init(&shoot_init->shoot_trig_motor.shoot_speed_pid_cascade,PID_POSITION,DATA_NORMAL,Shoot_trig_speed_pid_cascade,TRIG_SPEED_PID_MAX_OUT_CASCADE,TRIG_SPEED_PID_MAX_IOUT_CASCADE);
	PID_init(&shoot_init->shoot_trig_motor.shoot_angle_pid_cascade,PID_POSITION,DATA_NORMAL,Shoot_trig_angle_pid_cascade,TRIG_ANGLE_PID_MAX_OUT_CASCADE,TRIG_ANGLE_PID_MAX_IOUT_CASCADE);
	
	PID_init(&shoot_init->shoot_trig_motor.shoot_speed_pid_single,PID_POSITION,DATA_NORMAL,Shoot_trig_speed_pid_single,TRIG_SPEED_PID_MAX_OUT_SINGLE,TRIG_SPEED_PID_MAX_OUT_SINGLE);
	
	shoot_init->trig_fire_mode = Cease_fire;
	
	trig_feedback_update(shoot_init);
}


static void trig_feedback_update(shoot_control_t *feedback_update)
{
	feedback_update->shoot_trig_motor.motor_speed = feedback_update->shoot_trig_motor.shoot_motor_measure->rpm*TRIG_RPM_TO_SPEED_SEN;

}


/**
  * @brief          控制循环，根据控制设定值，计算电机电流值，进行控制
  * @param[out]     control_loop:"shoot_control"变量指针.
  * @retval         none
  */
static void shoot_control_loop(shoot_control_t *control_loop)
{
    if (control_loop == NULL)
    {
        return;
    }
	shoot_motor_control(&control_loop->shoot_trig_motor);

}



/**
  * @brief          拨弹盘控制量设置 + 堵转检测
  * @param[out]     control_loop:"shoot_control"变量指针.
  * @retval         none
  */
static void trig_motor_control(shoot_control_t * control_loop)
{
	shoot_trig_motor_mode_set(control_loop);
#ifdef CASCADE
	static int16_t trig_ecd_error;
	
	//拨弹盘过零检测
	trig_ecd_error = control_loop->shoot_trig_motor.shoot_motor_measure->ecd -control_loop->shoot_trig_motor.shoot_motor_measure->last_ecd;
	trig_ecd_error = trig_ecd_error >  4095 ?   trig_ecd_error - 8191 : trig_ecd_error;
	trig_ecd_error = trig_ecd_error < -4095 ?   trig_ecd_error + 8191 : trig_ecd_error;
	trig_ecd_sum += trig_ecd_error;
	
	trig_ecd_sum = trig_block_detect_cascade(control_loop,trig_ecd_sum);
	
#else
	control_loop->shoot_trig_motor.motor_speed_set = trig_block_detect_single(control_loop);
	
#endif
}


/**
  * @brief          拨弹盘堵转检测
  * @param[out]     control_loop:"shoot_control"变量指针.
	* @param[out]     angle_set:圈数设置量
  * @retval         圈数设置量
  */
int64_t trig_block_detect_cascade(shoot_control_t * control_loop,int64_t angle_set)
{
	EventBits_t shootEnevnt_rec;
	shootEnevnt_rec = xEventGroupWaitBits(my_shootEventGroupHandle,ShootEvent_1,pdTRUE,pdFALSE,0); 

	if(control_loop->trig_fire_mode != Cease_fire)
	{
		if( control_loop->shoot_trig_motor.shoot_motor_measure->rpm<20 && angle_set<-2000 && shootEnevnt_rec ==ShootEvent_1)
		{
			angle_set=0;
			angle_set+=20*8191;
			//trig_flag=0;
		}
		else if(control_loop->shoot_trig_motor.shoot_motor_measure->rpm>-20 && angle_set>2000 && shootEnevnt_rec ==ShootEvent_1)
		{
			angle_set=0;
			angle_set-=20*8191;
			//trig_flag=0;
		}

		else if((control_loop->trig_fire_mode == Single_fire || control_loop->trig_fire_mode == Serial_fire)&& shootEnevnt_rec == ShootEvent_1)
		{
				//trig_flag = 0;
				angle_set-=10*8191;
		}
	}
	else 
	{
		//trig_flag = 0;
		angle_set = 0;
	}
	return angle_set;
}



static fp32 trig_block_detect_single(shoot_control_t * control_loop)
{
	static fp32 trig_speed_set;
	static int16_t   NoMove_counter;
	static uint8_t   NoMove_flag;

	 
	//拨弹盘转一圈拨出去8个
	//2006减速比为36:1
	//rpm/60/36 为拨弹盘一秒转的圈数
	//*8等于一秒发弹
	//开火
	
	if(gimbal_data.FireFlag)
//	if(control_loop->trig_fire_mode != Cease_fire)
	{
		if(control_loop->shoot_trig_motor.shoot_motor_measure->rpm >STANDARD_NOMOVE_RPM && NoMove_flag ==0)
		{
			trig_speed_set = TRIG_BASE_SPEED;
			NoMove_counter =0;
		}
		//卡弹转动
		else if( control_loop->shoot_trig_motor.shoot_motor_measure->rpm <= STANDARD_NOMOVE_RPM && NoMove_flag ==0 )
		{
			trig_speed_set = TRIG_BASE_SPEED;						
			NoMove_counter ++;
			if( NoMove_counter >= STANDARD_NOMOVE_TIME ) 
			{
				NoMove_flag = 1;
				NoMove_counter = STANDARD_REMOVE_TIME;
			}	
		}
		//反向转动
		else if( NoMove_flag != 0)
		{
			trig_speed_set =-TRIG_BASE_SPEED;
			
			NoMove_counter--;
			
			if(NoMove_counter <= 0)
			{
			NoMove_flag = 0;
			NoMove_counter = 0;
			}
		}
	}
	//不开火
	else 
	{
		trig_speed_set = 0;
	}
	//遥控器掉线
	if(rc_is_error())
		trig_speed_set = 0;
	
	return trig_speed_set;
}

/**
  * @brief          控制循环，根据控制设定值，计算电机电流值，进行控制
  * @param[out]     shoot_motor:"shoot_motor_t"变量指针.
  * @retval         none
  */
static void shoot_motor_control(shoot_motor_t *shoot_motor)
{
    if (shoot_motor == NULL)
    {
        return;
    }
#ifdef CASCADE
		shoot_motor->motor_speed_set = PID_calc(&shoot_motor->shoot_angle_pid_cascade,trig_ecd_sum,0/*trig_angle_set*8191*/);
		shoot_motor->current_set = PID_calc(&shoot_motor->shoot_speed_pid_cascade,shoot_motor->shoot_motor_measure->rpm,shoot_motor->motor_speed_set);
#else
		shoot_motor->current_set = PID_calc(&shoot_motor->shoot_speed_pid_single,shoot_motor->motor_speed,shoot_motor->motor_speed_set);
#endif

}






//static void shoot_trig_motor_behaviour_set(shoot_control_t *shoot_behaviour)
//{
//	if(switch_is_mid(shoot_behaviour->rc_data->rc_sr))
//	{
//		if(switch_is_up(shoot_behaviour->rc_data->rc_sl))
//		{
//			shoot_behaviour->trig_fire_mode = Serial_fire;
//		}
//		else if(switch_is_down(shoot_behaviour->rc_data->rc_sl))
//		{
//			shoot_behaviour->trig_fire_mode = Single_fire;
//		}
//		else if(switch_is_mid(shoot_behaviour->rc_data->rc_sl))
//		{
//			shoot_behaviour->trig_fire_mode = Cease_fire;
//		}
//	}
//	else
//	{
//			shoot_behaviour->trig_fire_mode = Cease_fire;
//	}
//	if (rc_is_error())
//	{
//		shoot_behaviour->trig_fire_mode = Cease_fire;
//	}

//	shoot_behaviour->last_trig_fire_mode = shoot_behaviour->trig_fire_mode;
//}


static void shoot_trig_motor_behaviour_set(shoot_control_t *shoot_behaviour)
{
	
	
#ifdef CASCADE
	static uint8_t first=1;
	if(switch_is_mid(shoot_behaviour->rc_data->rc_sr))
	{
		if(switch_is_mid(shoot_behaviour->rc_data->rc_sl))
		{
			if(first)
			{
				osTimerStart(ShootTimerHandle,500);
				first=0;
			}
			if(shoot_behaviour->last_trig_fire_mode == Cease_fire)
			shoot_behaviour->trig_fire_mode = Single_fire;

		}
	}
//	else if(switch_is_down(shoot_behaviour->rc_data->rc_sl))
//	{
//		first=1;
//		shoot_behaviour->trig_fire_mode = Cease_fire;
//	}
	else
	{
		first=1;
		shoot_behaviour->trig_fire_mode = Cease_fire;
	}
	if(rc_is_error())
	{
		first=1;
		shoot_behaviour->trig_fire_mode = Cease_fire;
	}
	
#else
//	if(shoot_behaviour->is_fire)
//	{
//		
//	}
	if(switch_is_mid(shoot_behaviour->rc_data->rc_sr))
	{
		if(switch_is_mid(shoot_behaviour->rc_data->rc_sl))
		{
			shoot_behaviour->trig_fire_mode = Serial_fire;
		}
		else
		{
			shoot_behaviour->trig_fire_mode = Cease_fire;
		}
	}
	else
	{
		shoot_behaviour->trig_fire_mode = Cease_fire;
	}
	if(rc_is_error())
	{
		shoot_behaviour->trig_fire_mode = Cease_fire;
	}
	
#endif
	
	shoot_behaviour->last_trig_fire_mode = shoot_behaviour->trig_fire_mode;
}



//2500单点,385连发
//宏定义用来给定时器arr寄存器赋值
//__HAL_TIM_SET_AUTORELOAD(&htim5,2500);
//__HAL_TIM_SET_AUTORELOAD(&htim5,385);
void shoot_trig_motor_mode_set(shoot_control_t *shoot_mode)
{
	shoot_trig_motor_behaviour_set(shoot_mode);

#ifdef CASCADE
	if(shoot_mode->trig_fire_mode ==Serial_fire)
	{
		HAL_TIM_Base_Stop_IT(&htim5);
		__HAL_TIM_SET_AUTORELOAD(&htim5,Serial_ARR);
		HAL_TIM_Base_Start_IT(&htim5);

	}
	else if(shoot_mode->trig_fire_mode ==Single_fire)
	{
		HAL_TIM_Base_Stop_IT(&htim5);
		__HAL_TIM_SET_AUTORELOAD(&htim5,Single_ARR);
		HAL_TIM_Base_Start_IT(&htim5);

	}
	else if(shoot_mode->trig_fire_mode == Cease_fire)
	{
			HAL_TIM_Base_Stop_IT(&htim5);
	}
#endif
}



bool_t trig_detect(void)
{
	return toe_is_error(TRIGGER_MOTOR_TOE);
}
