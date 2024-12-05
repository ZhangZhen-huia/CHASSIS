#include "chassis_task.h"
#include "cmsis_os.h"
#include "chassis_behaviour.h"
#include "arm_math.h"
#include "communicate_task.h"




void Speed_Toggle(chassis_move_t *chassis_move_control_Speed);

static void Chassis_Debug_get_data(void);
static uint8_t chassis_motor_detect(void);


static void chassis_init(chassis_move_t *init);
static void chassis_feedback_update(chassis_move_t *feedback_update);
static fp32 Find_min_Angle(int16_t angle1,fp32 angle2);
bool_t judgeWheelPrepared(chassis_move_t *judge);

static void chassis_control_loop(chassis_move_t *chassis_move_control_loop);
static void chassis_vector_to_agv_calculate(fp32 wheel_angle[4],fp32 wheel_speed[4],fp32 vx_set,fp32 vy_set,fp32 wz_set);


static void chassis_set_mode(chassis_move_t *chassis_move_mode);
static void chassis_mode_change_control_transit(chassis_move_t *chassis_move_transit);
static void chassis_set_contorl(chassis_move_t *chassis_move_control);

chassis_move_t chassis_move;


static fp32 INIT_ECD[5] = {4362.0f,3693.0f,3132.0f,5025.0f,565.0f};


fp32 yaw_diff=0;

//底盘总任务
void chassis_task(void const * argument)
{
    //等待陀螺仪任务更新陀螺仪数据
		osDelay(CHASSIS_TASK_INIT_TIME);

    //底盘初始化
    chassis_init(&chassis_move);

    //判断底盘电机是否都在线
    while (chassis_motor_detect())
    {
        vTaskDelay(CHASSIS_CONTROL_TIME_MS);//保证全部初始化完毕
    }
		
    while (1)
		{
			//底盘控制模式选择
			chassis_set_mode(&chassis_move);
			//模式更改数据传递
			chassis_mode_change_control_transit(&chassis_move);
			//底盘数据更新
			chassis_feedback_update(&chassis_move);
      //底盘控制量设置（设置遥控器输入控制量）
      chassis_set_contorl(&chassis_move);
			//底盘控制
			chassis_control_loop(&chassis_move);			

			
			if(rc_is_error())
			{
				
				CAN_cmd_course(0,0,0,0);
				CAN_cmd_drive(0,0,0,0);

			}
			else
			{
				
				CAN_cmd_course(chassis_move.chassis_course_speed_pid[0].out,chassis_move.chassis_course_speed_pid[1].out,chassis_move.chassis_course_speed_pid[2].out,chassis_move.chassis_course_speed_pid[3].out);
					CAN_cmd_drive(chassis_move.chassis_drive_speed_pid[0].out,chassis_move.chassis_drive_speed_pid[1].out,chassis_move.chassis_drive_speed_pid[2].out,chassis_move.chassis_drive_speed_pid[3].out);
			}
			osDelay(1);//控制频率为1khz，与can接收中断频率一致
		}
}


static void chassis_init(chassis_move_t *init)
{
	uint8_t i=0;
	
	static float PID_SPEED_DRIVE[4][3]={DRIVE_MOTOR1_SPEED_PID_KP,DRIVE_MOTOR1_SPEED_PID_KI,DRIVE_MOTOR1_SPEED_PID_KD,
																			DRIVE_MOTOR2_SPEED_PID_KP,DRIVE_MOTOR2_SPEED_PID_KI,DRIVE_MOTOR2_SPEED_PID_KD,
																			DRIVE_MOTOR3_SPEED_PID_KP,DRIVE_MOTOR3_SPEED_PID_KI,DRIVE_MOTOR3_SPEED_PID_KD,
																			DRIVE_MOTOR4_SPEED_PID_KP,DRIVE_MOTOR4_SPEED_PID_KI,DRIVE_MOTOR4_SPEED_PID_KD};
	
	static float PID_ANGLE_COURSE[4][3]={COURSE_MOTOR1_ANGLE_PID_KP,COURSE_MOTOR1_ANGLE_PID_KI,COURSE_MOTOR1_ANGLE_PID_KD,
																			 COURSE_MOTOR2_ANGLE_PID_KP,COURSE_MOTOR2_ANGLE_PID_KI,COURSE_MOTOR2_ANGLE_PID_KD,
																			 COURSE_MOTOR3_ANGLE_PID_KP,COURSE_MOTOR3_ANGLE_PID_KI,COURSE_MOTOR3_ANGLE_PID_KD,
																			 COURSE_MOTOR4_ANGLE_PID_KP,COURSE_MOTOR4_ANGLE_PID_KI,COURSE_MOTOR4_ANGLE_PID_KD};
	
	static float PID_SPEED_COURSE[4][3]={COURSE_MOTOR1_SPEED_PID_KP,COURSE_MOTOR1_SPEED_PID_KI,COURSE_MOTOR1_SPEED_PID_KD,
																		   COURSE_MOTOR2_SPEED_PID_KP,COURSE_MOTOR2_SPEED_PID_KI,COURSE_MOTOR2_SPEED_PID_KD,
																			 COURSE_MOTOR3_SPEED_PID_KP,COURSE_MOTOR3_SPEED_PID_KI,COURSE_MOTOR3_SPEED_PID_KD,
																			 COURSE_MOTOR4_SPEED_PID_KP,COURSE_MOTOR4_SPEED_PID_KI,COURSE_MOTOR4_SPEED_PID_KD};
	
	static float YAW_ANGLE_PID[3] = {YAW_ANGLE_PID_KP,YAW_ANGLE_PID_KI,YAW_ANGLE_PID_KD};
	
	static float SPEED_PID_MAX_OUT_DRIVE[4]={DRIVE_MOTOR1_SPEED_PID_MAX_OUT,DRIVE_MOTOR2_SPEED_PID_MAX_OUT,DRIVE_MOTOR3_SPEED_PID_MAX_OUT,DRIVE_MOTOR4_SPEED_PID_MAX_OUT}; 
	static float SPEED_PID_MAX_IOUT_DRIVE[4]={DRIVE_MOTOR1_SPEED_PID_MAX_IOUT,DRIVE_MOTOR2_SPEED_PID_MAX_IOUT,DRIVE_MOTOR3_SPEED_PID_MAX_IOUT,DRIVE_MOTOR4_SPEED_PID_MAX_IOUT}; 
	
	static float SPEED_PID_MAX_OUT_COURSE[4] = {COURSE_MOTOR1_SPEED_PID_MAX_OUT,COURSE_MOTOR2_SPEED_PID_MAX_OUT,COURSE_MOTOR3_SPEED_PID_MAX_OUT,COURSE_MOTOR4_SPEED_PID_MAX_OUT};
	static float SPEED_PID_MAX_IOUT_COURSE[4] = {COURSE_MOTOR1_SPEED_PID_MAX_IOUT,COURSE_MOTOR2_SPEED_PID_MAX_IOUT,COURSE_MOTOR3_SPEED_PID_MAX_IOUT,COURSE_MOTOR4_SPEED_PID_MAX_IOUT};

	static float ANGLE_PID_MAX_OUT_COURSE[4] = {COURSE_MOTOR1_ANGLE_PID_MAX_OUT,COURSE_MOTOR2_ANGLE_PID_MAX_OUT,COURSE_MOTOR3_ANGLE_PID_MAX_OUT,COURSE_MOTOR4_ANGLE_PID_MAX_OUT};
	static float ANGLE_PID_MAX_IOUT_COURSE[4] = {COURSE_MOTOR1_ANGLE_PID_MAX_IOUT,COURSE_MOTOR2_ANGLE_PID_MAX_IOUT,COURSE_MOTOR3_ANGLE_PID_MAX_IOUT,COURSE_MOTOR4_ANGLE_PID_MAX_IOUT};
	
	static float YAW_PID_MAX[2] = {YAW_PID_MAX_OUT,YAW_PID_MAX_IOUT};
	const static fp32 chassis_x_order_filter[1] = {CHASSIS_ACCEL_X_NUM};
  const static fp32 chassis_y_order_filter[1] = {CHASSIS_ACCEL_Y_NUM};
  const static fp32 chassis_w_order_filter[1] = {CHASSIS_ACCEL_W_NUM};
	
	
	init->chassis_RC=get_remote_control_point();
	init->chassis_bmi088_data = get_INS_data_point();
	init->get_gimbal_data = get_gimbal_data_point();
	PID_init(&init->yaw_pid,PID_POSITION,DATA_NORMAL,YAW_ANGLE_PID,YAW_PID_MAX[0],YAW_PID_MAX[1]);//初始化底盘PID

	//初始化驱动速度PID 并获取电机数据
	for(i=0;i<4;i++)
	{
		init->motor_chassis[i].chassis_motor_measure = get_chassis_drive_motor_measure_point(i);//获取底盘3508的数据，接收电机的反馈结构体		
		PID_init(&init->chassis_drive_speed_pid[i],PID_POSITION,DATA_NORMAL,PID_SPEED_DRIVE[i],SPEED_PID_MAX_OUT_DRIVE[i],SPEED_PID_MAX_IOUT_DRIVE[i]);//初始化底盘PID
		init->drive_set_speed[i]=0.0f;
	}
	
	//初始化航向电机速度和角度PID并获取电机数据
	for(i=0;i<4;i++)
	{
		init->motor_chassis[i+4].chassis_motor_measure = get_chassis_course_motor_measure_point(i);//获取航向电机的数据，接收电机的反馈结构体
		PID_init(&init->chassis_course_speed_pid[i],PID_POSITION,DATA_NORMAL,PID_SPEED_COURSE[i],SPEED_PID_MAX_OUT_COURSE[i],SPEED_PID_MAX_IOUT_COURSE[i]);//初始化速度PID
		PID_init(&init->chassis_course_angle_pid[i],PID_POSITION,DATA_NORMAL,PID_ANGLE_COURSE[i],ANGLE_PID_MAX_OUT_COURSE[i],ANGLE_PID_MAX_IOUT_COURSE[i]);//初始化角度PID
		init->course_set_angle[i]=0.0f;
	}
	//用一阶滤波
  first_order_filter_init(&init->chassis_cmd_slow_set_vx, CHASSIS_CONTROL_TIME, chassis_x_order_filter);
  first_order_filter_init(&init->chassis_cmd_slow_set_vy, CHASSIS_CONTROL_TIME, chassis_y_order_filter);
  first_order_filter_init(&init->chassis_cmd_slow_set_wz, CHASSIS_CONTROL_TIME_W, chassis_w_order_filter);
	
	//最大 最小速度
  init->vx_max_speed = NORMAL_MAX_CHASSIS_SPEED_X;
  init->vx_min_speed = -NORMAL_MAX_CHASSIS_SPEED_X;

  init->vy_max_speed = NORMAL_MAX_CHASSIS_SPEED_Y;
  init->vy_min_speed = -NORMAL_MAX_CHASSIS_SPEED_Y;
	
  init->wz_max_speed = NORMAL_MAX_CHASSIS_SPEED_W;
  init->wz_min_speed = -NORMAL_MAX_CHASSIS_SPEED_W;
	
	
	//更新底盘数据
	chassis_feedback_update(init);
}









/**
  * @brief          设置底盘控制模式，主要在'chassis_behaviour_mode_set'函数中改变
  * @param[out]     chassis_move_mode:"chassis_move"变量指针.
  * @retval         none
  */
static void chassis_set_mode(chassis_move_t *chassis_move_mode)
{
    if (chassis_move_mode == NULL)
    {
        return;
    }
    chassis_behaviour_mode_set(chassis_move_mode);
}


/**
  * @brief          底盘模式改变，有些参数需要改变，例如底盘控制yaw角度设定值应该变成当前底盘yaw角度
  * @param[out]     chassis_move_transit:"chassis_move"变量指针.
  * @retval         none
  */
static void chassis_mode_change_control_transit(chassis_move_t *chassis_move_transit)
{
    if (chassis_move_transit == NULL)
    {
        return;
    }

    if (chassis_move_transit->last_chassis_mode == chassis_move_transit->chassis_mode)
    {
        return;
    }

//    //change to follow gimbal angle mode
//    //切入跟随云台模式
//    if ((chassis_move_transit->last_chassis_mode != CHASSIS_VECTOR_FOLLOW_GIMBAL_YAW) && chassis_move_transit->chassis_mode == CHASSIS_VECTOR_FOLLOW_GIMBAL_YAW)
//    {
//        chassis_move_transit->chassis_relative_angle_set = 0.0f;
//    }
//    //change to follow chassis yaw angle
//    //切入跟随底盘角度模式
//    else if ((chassis_move_transit->last_chassis_mode != CHASSIS_VECTOR_FOLLOW_CHASSIS_YAW) && chassis_move_transit->chassis_mode == CHASSIS_VECTOR_FOLLOW_CHASSIS_YAW)
//    {
//        chassis_move_transit->chassis_yaw_set = chassis_move_transit->chassis_yaw;
//    }
//    //change to no follow angle
//    //切入不跟随云台模式
//    else if ((chassis_move_transit->last_chassis_mode != CHASSIS_VECTOR_NO_FOLLOW_YAW) && chassis_move_transit->chassis_mode == CHASSIS_VECTOR_NO_FOLLOW_YAW)
//    {
//        chassis_move_transit->chassis_yaw_set = chassis_move_transit->chassis_yaw;
//    }

//    chassis_move_transit->last_chassis_mode = chassis_move_transit->chassis_mode;
}



/**
  * @brief          控制循环，根据控制设定值，计算电机电流值，进行控制
  * @param[out]     chassis_move_control_loop:"chassis_move"变量指针.
  * @retval         none
  */
static void chassis_control_loop(chassis_move_t *chassis_move_control_loop)
{
	//求航向电机的角度
	chassis_vector_to_agv_calculate(chassis_move_control_loop->course_set_angle,chassis_move_control_loop->drive_set_speed,
																	chassis_move_control_loop->vx_set,chassis_move_control_loop->vy_set,chassis_move_control_loop->wz_set);

	

	//计算pid
	for (int i = 0; i < 4; i++)
		{
        PID_calc(&chassis_move_control_loop->chassis_drive_speed_pid[i], 
					chassis_move_control_loop->motor_chassis[i].speed,chassis_move_control_loop->drive_set_speed[i]);
		}
					//计算6020角度串速度环，速度环做内环，角度环做外环
		for(int i=0;i<4;i++)
		{
				//角度环
				PID_Calc_Ecd(&chassis_move_control_loop->chassis_course_angle_pid[i],
					chassis_move_control_loop->course_angle[i],chassis_move_control_loop->course_set_angle[i],360.0f);//set[i]);
				//速度环
				PID_calc(&chassis_move_control_loop->chassis_course_speed_pid[i],
					chassis_move_control_loop->motor_chassis[i+4].chassis_motor_measure->rpm,
					chassis_move_control_loop->chassis_course_angle_pid[i].out);
		}


}
/**
  * @brief          计算ecd与offset_ecd之间的相对角度
  * @param[in]      ecd: 电机当前编码
  * @param[in]      offset_ecd: 电机中值编码
  * @retval         相对角度，单位rad
  */
static fp32 motor_ecd_to_angle_change(uint16_t ecd, uint16_t offset_ecd)
{
    int32_t relative_ecd = ecd - offset_ecd;
    if (relative_ecd > HALF_ECD_RANGE)
    {
        relative_ecd -= ECD_RANGE;
    }
    else if (relative_ecd < -HALF_ECD_RANGE)
    {
        relative_ecd += ECD_RANGE;
    }

    return relative_ecd * MOTOR_ECD_TO_RAD;
}


/**
  * @brief          底盘测量数据更新，包括电机速度，欧拉角度，机器人速度
  * @param[out]     gimbal_feedback_update:"gimbal_control"变量指针.
  * @retval         none
  */
static void chassis_feedback_update(chassis_move_t *feedback_update)
{
	uint8_t i = 0;
	
	//更新驱动电机实际速度-->额定3.54m/s
	for (i = 0; i < 4; i++)
	{
		feedback_update->motor_chassis[i].speed = feedback_update->motor_chassis[i].chassis_motor_measure->rpm * (CHASSIS_MOTOR_RPM_TO_VECTOR_SEN);
	}
	
	feedback_update->wz_rad = feedback_update->chassis_bmi088_data->gyro[INS_GYRO_X_ADDRESS_OFFSET];
	
	feedback_update->chassis_yaw = feedback_update->chassis_bmi088_data->INS_angle[INS_YAW_ADDRESS_OFFSET];

	
		yaw_diff = motor_ecd_to_angle_change(yaw_motor.ecd,INIT_ECD[4]);
	
	
	for(i=0;i<4;i++)
	feedback_update->course_angle[i] = Angle_Limit((feedback_update->motor_chassis[4+i].chassis_motor_measure->ecd - INIT_ECD[i])*360/8192,360);
	
	#ifdef CHASSIS_DEBUG
	Chassis_Debug_get_data();
	#endif
}


/**
  * @brief          设置底盘控制设置值, 三运动控制值是通过chassis_behaviour_control_set函数设置的
  * @param[out]     chassis_move_update:"chassis_move"变量指针.
  * @retval         none
  */
static void chassis_set_contorl(chassis_move_t *chassis_move_control)
{
		//检查是否为空指针
    if (chassis_move_control == NULL)
    {
        return;
    }
			//设置速度
		
    fp32 vx_set = 0.0f, vy_set = 0.0f, wz_set = 0.0f;

			//根据底盘状态设置来计算vx_set，vy_set，wz_set
    chassis_behaviour_control_set(&vx_set,&vy_set,&wz_set,chassis_move_control);
    
		//跟随云台yaw
		if (chassis_move_control->chassis_mode == CHASSIS_VECTOR_FOLLOW_GIMBAL_YAW)
    {
			static fp32 last_angle_diff;
			fp32 angle_diff;
			
			last_angle_diff = angle_diff;
			angle_diff=rad_format(yaw_diff* PI / 180.0f);
			
			chassis_move_control->vx_set = -vx_set * sin(angle_diff) + vy_set * cos(angle_diff);
			chassis_move_control->vy_set = -vx_set * cos(angle_diff) - vy_set * sin(angle_diff);
			
			if(fabs((last_angle_diff- angle_diff)*57.2957f) > 10)
			{
				PID_calc(&chassis_move_control->yaw_pid,angle_diff,0);
				chassis_move_control->wz_set = chassis_move_control->yaw_pid.out;
			}
			else
				chassis_move_control->wz_set=0;
		
			chassis_move_control->wz_set = fp32_constrain(chassis_move_control->wz_set, chassis_move_control->wz_min_speed, chassis_move_control->wz_max_speed);
			chassis_move_control->vx_set = fp32_constrain(chassis_move_control->vx_set, chassis_move_control->vx_min_speed, chassis_move_control->vx_max_speed);
			chassis_move_control->vy_set = fp32_constrain(chassis_move_control->vy_set, chassis_move_control->vy_min_speed, chassis_move_control->vy_max_speed);
			
		}
		
		//不跟随云台
    else if (chassis_move_control->chassis_mode == CHASSIS_VECTOR_FOLLOW_CHASSIS_YAW)
		{
			 fp32 angle_diff=yaw_diff* PI / 180.0f;
			 
			chassis_move_control->vx_set = -vx_set * sin(angle_diff) + vy_set * cos(angle_diff);
			chassis_move_control->vy_set = -vx_set * cos(angle_diff) - vy_set * sin(angle_diff);
			 chassis_move_control->wz_set = 0.5f;
		
				
				
			 chassis_move_control->wz_set = fp32_constrain(chassis_move_control->wz_set, chassis_move_control->wz_min_speed, chassis_move_control->wz_max_speed);
			 chassis_move_control->vx_set = fp32_constrain(chassis_move_control->vx_set, chassis_move_control->vx_min_speed, chassis_move_control->vx_max_speed);
			 chassis_move_control->vy_set = fp32_constrain(chassis_move_control->vy_set, chassis_move_control->vy_min_speed, chassis_move_control->vy_max_speed);
			
		}
		
		//不跟随云台
    else if (chassis_move_control->chassis_mode == CHASSIS_VECTOR_ZERO_FORCE)
    {
			 chassis_move_control->wz_set = vy_set;
			 chassis_move_control->vx_set = vx_set;
			 chassis_move_control->vy_set = wz_set;
    }
}

float My_Max(float *data)
{
	int i;
	float result=0;
	for(i=0;*(data+i) !=NULL;i++)
		if(data[i]>result)
			result=data[i];
	return result;
		
}

fp32 drct[4] = {1,1,1,1};
static void chassis_vector_to_agv_calculate(fp32 wheel_angle[4],fp32 wheel_speed[4],fp32 vx_set,fp32 vy_set,fp32 wz_set)
{
	//上一次角度
	static fp32 last_wheel_angle[4];
	
	
	//0
	wheel_angle[0]=Angle_Limit(atan2(vy_set + wz_set*0.707f,vx_set + wz_set*0.707f)/PI*180.0,360.0f);//45  (0 -- 360)
	
	//寻找最小转角然后驱动3508正反转，然后再由pid的过零处理算出来正确的err
	//比如：ref:30   set:130  
	//经以下函数得wheel_angle[0] = 130+180 = 310;
	//pid计算ecd_zero(set, ref, ecd_range);算出来err=-80，
	//然后计算后得数据就是让6020反转80度到达-50度，与130度刚好构成180度，此时让drct取反了，所以优劣弧生效
	if(fabs(Find_min_Angle(chassis_move.course_angle[0],wheel_angle[0]))>90)
	{
			wheel_angle[0] += 180;		
			wheel_angle[0]=Angle_Limit(wheel_angle[0],360);
			drct[0] = -1;
	}
	else
			drct[0]=1;
	wheel_speed[0] = drct[0]*sqrt(pow((vy_set + wz_set*0.707f),2)+pow(vx_set + wz_set*0.707f,2));
	
	
	
	
	//1
	wheel_angle[1]=Angle_Limit(atan2(vy_set + wz_set*0.707f,vx_set - wz_set*0.707f)/PI*180.0,360.0f); //135 (0 -- 360)
	if(fabs(Find_min_Angle(chassis_move.course_angle[1],wheel_angle[1]))>90)
	{
			wheel_angle[1] += 180;		
			wheel_angle[1]=Angle_Limit(wheel_angle[1],360);
			drct[1] = -1;
	}
	else
			drct[1]=1;
	wheel_speed[1] = drct[1]*sqrt(pow((vy_set + wz_set*0.707f),2)+pow(vx_set - wz_set*0.707f,2));

	
	
	
	//2
	wheel_angle[2]=Angle_Limit(atan2(vy_set - wz_set*0.707f,vx_set - wz_set*0.707f)/PI*180.0,360.0f); //45 (0 -- 360)
	if(fabs(Find_min_Angle(chassis_move.course_angle[2],wheel_angle[2]))>90)
	{
			wheel_angle[2] += 180;		
			wheel_angle[2]=Angle_Limit(wheel_angle[2],360);
			drct[2] = -1;
	}
	else
			drct[2]=1;
	wheel_speed[2] = drct[2]*sqrt(pow((vy_set - wz_set*0.707f),2)+pow(vx_set - wz_set*0.707f,2));


	//3
	wheel_angle[3]=Angle_Limit(atan2(vy_set - wz_set*0.707f,vx_set + wz_set*0.707f)/PI*180.0,360.0f); // -45(0 -- 360)
	
	if(fabs(Find_min_Angle(chassis_move.course_angle[3],wheel_angle[3]))>90)
	{
			wheel_angle[3] += 180;		
			wheel_angle[3]=Angle_Limit(wheel_angle[3],360);
			drct[3] = -1;
	}
	else
			drct[3]=1;
	wheel_speed[3] = drct[3]*sqrt(pow((vy_set - wz_set*0.707f),2)+pow(vx_set + wz_set*0.707f,2));


		
		if(vx_set == 0 && vy_set == 0 && wz_set == 0)//摇杆回中时，保持6020角度
		{
			for(int i=0;i<4;i++)//memcpy狗都不用
			wheel_angle[i] = last_wheel_angle[i];
		}
		else
		{
			for(int i=0;i<4;i++)
			{
				last_wheel_angle[i]=wheel_angle[i];
			}
		}
		for(int i=0;i<4;i++)//减少手抖误差 和回中误差
		{
			if(fabs(wheel_angle[i]-last_wheel_angle[i])<10)//小于10度维持原值
			{
				wheel_angle[i]=last_wheel_angle[i];
			}
		}	
	for(uint8_t i=0;i<4;i++)	
	wheel_speed[i] = fp32_constrain(wheel_speed[i], chassis_move.vy_min_speed, chassis_move.vy_max_speed);


}




/**
  * @brief          根据遥控器通道值，计算纵向和横移速度               
  * @param[out]     vx_set: 纵向速度指针
  * @param[out]     vy_set: 横向速度指针
  * @param[out]     chassis_move_rc_to_vector: "chassis_move" 变量指针
  * @retval         none
  */
void chassis_rc_to_control_vector(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector)
{
	int16_t vx_channel, vy_channel, wz_channel;
  fp32 vx_set_channel, vy_set_channel,wz_set_channel;
//  //死区限制，因为遥控器可能存在差异 摇杆在中间，其值不为0
//  rc_deadband_limit(chassis_move_rc_to_vector->chassis_RC->rc.ch[CHASSIS_X_CHANNEL], vx_channel, CHASSIS_RC_DEADLINE);
//  rc_deadband_limit(chassis_move_rc_to_vector->chassis_RC->rc.ch[CHASSIS_Y_CHANNEL], vy_channel, CHASSIS_RC_DEADLINE);
//  rc_deadband_limit(chassis_move_rc_to_vector->chassis_RC->rc.ch[CHASSIS_W_CHANNEL], wz_channel, CHASSIS_RC_DEADLINE);
	
  rc_deadband_limit(chassis_move_rc_to_vector->get_gimbal_data->rc_data.vx_set, vx_channel, CHASSIS_RC_DEADLINE);
  rc_deadband_limit(chassis_move_rc_to_vector->get_gimbal_data->rc_data.vy_set, vy_channel, CHASSIS_RC_DEADLINE);
  rc_deadband_limit(chassis_move_rc_to_vector->get_gimbal_data->rc_data.wz_set, wz_channel, CHASSIS_RC_DEADLINE);

		vx_set_channel = vx_channel * CHASSIS_VX_RC_SEN;
    vy_set_channel = vy_channel * -CHASSIS_VY_RC_SEN;
	  wz_set_channel = wz_channel * CHASSIS_WZ_RC_SEN;
//    //keyboard set speed set-point
//    //键盘控制
//    if (chassis_move_rc_to_vector->chassis_RC->key.v & CHASSIS_FRONT_KEY)
//    {
//        vx_set_channel = chassis_move_rc_to_vector->vx_max_speed;
//    }
//    else if (chassis_move_rc_to_vector->chassis_RC->key.v & CHASSIS_BACK_KEY)
//    {
//        vx_set_channel = chassis_move_rc_to_vector->vx_min_speed;
//    }

//    if (chassis_move_rc_to_vector->chassis_RC->key.v & CHASSIS_LEFT_KEY)
//    {
//        vy_set_channel = chassis_move_rc_to_vector->vy_max_speed;
//    }
//    else if (chassis_move_rc_to_vector->chassis_RC->key.v & CHASSIS_RIGHT_KEY)
//    {
//        vy_set_channel = chassis_move_rc_to_vector->vy_min_speed;
//    }

	//一阶低通滤波代替斜波作为底盘速度输入
  first_order_filter_cali(&chassis_move_rc_to_vector->chassis_cmd_slow_set_vx, vx_set_channel);
  first_order_filter_cali(&chassis_move_rc_to_vector->chassis_cmd_slow_set_vy, vy_set_channel);
  first_order_filter_cali(&chassis_move_rc_to_vector->chassis_cmd_slow_set_wz, wz_set_channel);
	
  if (vx_set_channel < CHASSIS_RC_DEADLINE * CHASSIS_VX_RC_SEN && vx_set_channel > -CHASSIS_RC_DEADLINE * CHASSIS_VX_RC_SEN)
  {
      chassis_move_rc_to_vector->chassis_cmd_slow_set_vx.out = 0.0f;
  }

  if (vy_set_channel < CHASSIS_RC_DEADLINE * CHASSIS_VY_RC_SEN && vy_set_channel > -CHASSIS_RC_DEADLINE * CHASSIS_VY_RC_SEN)
  {
      chassis_move_rc_to_vector->chassis_cmd_slow_set_vy.out = 0.0f;
  }
  if (wz_set_channel < CHASSIS_RC_DEADLINE * CHASSIS_WZ_RC_SEN && wz_set_channel > -CHASSIS_RC_DEADLINE * CHASSIS_WZ_RC_SEN)
  {
      chassis_move_rc_to_vector->chassis_cmd_slow_set_wz.out = 0.0f;
  }    
	
	*vx_set = chassis_move_rc_to_vector->chassis_cmd_slow_set_vx.out;
  *vy_set = chassis_move_rc_to_vector->chassis_cmd_slow_set_vy.out;
	*wz_set = chassis_move_rc_to_vector->chassis_cmd_slow_set_wz.out;

}

/**
	* @brief 等待6020角度转到位在转3508
	* @param 底盘结构体
  * @retval 
  * @attention	
  */

bool_t judgeWheelPrepared(chassis_move_t *judge)
{
	float maxError;
	float Error[4];
	for(uint8_t i=0;i<4;i++)
	Error[i] = fabs(judge->course_angle[i] - judge->course_set_angle[i]);
	
	maxError = My_Max(Error);
	if(maxError < 10)
		return 1;
	else
		return 0;

}




/**
	* @brief 寻找最小转动角度
	* @param  angle1,angle2 一个为目标角度，一个为当前角度
  * @retval 角度差值
  * @attention	返回的差值是经过计算后限制在0-360之间的
  */
static fp32 Find_min_Angle(int16_t angle1,fp32 angle2)
{
	fp32 err;
    err = (fp32)angle1 - angle2;
    if(fabs(err) > 180)
    {
        err = 360 - fabs(err);
    }
    return err;
}


static uint8_t chassis_motor_detect(void)
{
	return 	toe_is_error(COURSE_MOTOR1_TOE)  || toe_is_error(DRIVE_MOTOR2_TOE)  || toe_is_error(DRIVE_MOTOR3_TOE)  || toe_is_error(DRIVE_MOTOR4_TOE)
				 || toe_is_error(COURSE_MOTOR1_TOE) || toe_is_error(COURSE_MOTOR2_TOE) || toe_is_error(COURSE_MOTOR3_TOE) || toe_is_error(COURSE_MOTOR4_TOE);
}


/* ----------------------------------------------------------------------- */
/* -------------------------------vofa调参-------------------------------- */
/* ----------------------------------------------------------------------- */
static void Chassis_Debug_get_data(void)
{
	chassis_move.chassis_debug_data.data1 = chassis_move.motor_chassis[0].speed;
	chassis_move.chassis_debug_data.data2 = chassis_move.motor_chassis[1].speed;
	chassis_move.chassis_debug_data.data3 = chassis_move.motor_chassis[2].speed;
	
	//chassis_debug_data.data4 = chassis_move.motor_chassis[3].speed;
	
	chassis_move.chassis_debug_data.data4 = chassis_move.drive_set_speed[0];
	chassis_move.chassis_debug_data.data5 = chassis_move.drive_set_speed[1];
	chassis_move.chassis_debug_data.data6 = chassis_move.drive_set_speed[2];
}
const DebugData* get_chassis_PID_Debug(void)
{
	return &chassis_move.chassis_debug_data;
}


