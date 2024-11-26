#include "trig_task.h"
#include "tim.h"
#include "user_task.h"


static int64_t  trig_ecd_sum=0;

shoot_control_t trig_control;

static void trig_init(shoot_control_t *shoot_init);

static void trig_feedback_update(shoot_control_t *feedback_update);
static void trig_motor_control(shoot_control_t * control_loop);
static void shoot_control_loop(shoot_control_t *control_loop);
void shoot_trig_motor_mode_set(shoot_control_t *shoot_mode);
int64_t trig_block_detect(shoot_control_t * control_loop,int64_t angle_set);
static void shoot_motor_control(shoot_motor_t *shoot_motor);
static void shoot_trig_motor_behaviour_set(shoot_control_t *shoot_behaviour);



void trig_task(void const * argument)
{
		vTaskDelay(TRIG_TASK_INIT_TIME);

    trig_init(&trig_control);

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
	static const fp32 Shoot_trig_speed_pid[3] = {TRIG_SPEED_PID_KP,TRIG_SPEED_PID_KI,TRIG_SPEED_PID_KD};
	static const fp32 Shoot_trig_angle_pid[3] = {TRIG_ANGLE_PID_KP,TRIG_ANGLE_PID_KI,TRIG_ANGLE_PID_KD};
	
	shoot_init->shoot_trig_motor.shoot_motor_measure = get_gimbal_trigger_motor_measure_point();
	
	//遥控器数据指针获取
	shoot_init->rc_data = get_gimbal_rc_data_point();

	//初始化Trig电机速度pid
	K_FF_init(&shoot_init->shoot_trig_motor.shoot_speed_pid,PID_POSITION,Shoot_trig_speed_pid,TRIG_SPEED_PID_MAX_OUT,TRIG_SPEED_PID_MAX_IOUT,TRIG_SPEED_KF_STATIC,TRIG_SPEED_KF_DYNAMIC);
	K_FF_init(&shoot_init->shoot_trig_motor.shoot_angle_pid,PID_POSITION,Shoot_trig_angle_pid,TRIG_ANGLE_PID_MAX_OUT,TRIG_ANGLE_PID_MAX_IOUT,TRIG_ANGLE_KF_STATIC,TRIG_ANGLE_KF_DYNAMIC);
	
	shoot_init->trig_fire_mode = Cease_fire;
	
	trig_feedback_update(shoot_init);
}


static void trig_feedback_update(shoot_control_t *feedback_update)
{
	feedback_update->shoot_trig_motor.motor_speed = feedback_update->shoot_trig_motor.shoot_motor_measure->rpm;//*TRIG_RPM_TO_SPEED_SEN;//rpm最大14000以上

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
	static int16_t trig_ecd_error;
	
	shoot_trig_motor_mode_set(control_loop);

	//拨弹盘过零检测
	trig_ecd_error = control_loop->shoot_trig_motor.shoot_motor_measure->ecd -control_loop->shoot_trig_motor.shoot_motor_measure->last_ecd;
	trig_ecd_error = trig_ecd_error >  4095 ?   trig_ecd_error - 8191 : trig_ecd_error;
	trig_ecd_error = trig_ecd_error < -4095 ?   trig_ecd_error + 8191 : trig_ecd_error;
	trig_ecd_sum += trig_ecd_error;
	
	trig_ecd_sum = trig_block_detect(control_loop,trig_ecd_sum);
	
}


/**
  * @brief          拨弹盘堵转检测
  * @param[out]     control_loop:"shoot_control"变量指针.
	* @param[out]     angle_set:圈数设置量
  * @retval         圈数设置量
  */
int64_t trig_block_detect(shoot_control_t * control_loop,int64_t angle_set)
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

		shoot_motor->motor_speed_set = K_FF_Cal(&shoot_motor->shoot_angle_pid,trig_ecd_sum,0/*trig_angle_set*8191*/);
		shoot_motor->current_set = K_FF_Cal(&shoot_motor->shoot_speed_pid,shoot_motor->shoot_motor_measure->rpm,shoot_motor->motor_speed_set);
	


}

extern osTimerId ShootTimerHandle;




static void shoot_trig_motor_behaviour_set(shoot_control_t *shoot_behaviour)
{
	if(switch_is_mid(shoot_behaviour->rc_data->rc_sr))
	{
		if(switch_is_up(shoot_behaviour->rc_data->rc_sl))
		{
			shoot_behaviour->trig_fire_mode = Serial_fire;
		}
		else if(switch_is_down(shoot_behaviour->rc_data->rc_sl))
		{
			shoot_behaviour->trig_fire_mode = Single_fire;
		}
		else if(switch_is_mid(shoot_behaviour->rc_data->rc_sl))
		{
			shoot_behaviour->trig_fire_mode = Cease_fire;
		}
	}
	else
	{
			shoot_behaviour->trig_fire_mode = Cease_fire;
	}
	if (rc_is_error())
	{
		shoot_behaviour->trig_fire_mode = Cease_fire;
	}

	shoot_behaviour->last_trig_fire_mode = shoot_behaviour->trig_fire_mode;
}


//static void shoot_trig_motor_behaviour_set(shoot_control_t *shoot_behaviour)
//{
//	static uint8_t first=1;
//	if(switch_is_up(shoot_behaviour->rc_data->rc_sl))
//	{
//		if(first)
//		{
//			osTimerStart(ShootTimerHandle,500);
//			first=0;
//		}
//		if(shoot_behaviour->last_trig_fire_mode == Cease_fire)
//		shoot_behaviour->trig_fire_mode = Single_fire;

//	}
////	else if(switch_is_down(shoot_behaviour->rc_data->rc_sl))
////	{
////		first=1;
////		shoot_behaviour->trig_fire_mode = Cease_fire;
////	}
//	else
//	{
//		first=1;
//		shoot_behaviour->trig_fire_mode = Cease_fire;
//	}
//	shoot_behaviour->last_trig_fire_mode = shoot_behaviour->trig_fire_mode;
//}



//2500单点,385连发
//宏定义用来给定时器arr寄存器赋值
//__HAL_TIM_SET_AUTORELOAD(&htim5,2500);
//__HAL_TIM_SET_AUTORELOAD(&htim5,385);
void shoot_trig_motor_mode_set(shoot_control_t *shoot_mode)
{
	shoot_trig_motor_behaviour_set(shoot_mode);
	
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
}


