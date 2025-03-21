#include "chassis_task.h"
#include "cmsis_os.h"
#include "chassis_behaviour.h"
#include "arm_math.h"
#include "communicate_task.h"
#include "chassis_power_control.h"
#include "key_task.h"

#define CHASSIS_POWER_CONTROL

static void Chassis_Debug_get_data(void);
static uint8_t chassis_motor_detect(void);


static void chassis_init(chassis_move_t *init);
static void chassis_feedback_update(chassis_move_t *feedback_update);
static fp32 Find_min_Angle(int16_t angle1,fp32 angle2);

static void chassis_control_loop(chassis_move_t *chassis_move_control_loop);
static void chassis_vector_to_agv_calculate(fp32 wheel_angle[4],fp32 wheel_speed[4],fp32 vx_set,fp32 vy_set,fp32 wz_set);


static void chassis_set_mode(chassis_move_t *chassis_move_mode);
static void chassis_set_contorl(chassis_move_t *chassis_move_control);

chassis_move_t chassis_move;


static fp32 INIT_ECD[5] = {395.0f,1814.0f,5026.0f,3597.0f,7743.0f};

fp32 yaw_diff=0;

//����������
void chassis_task(void const * argument)
{
    //�ȴ������������������������
		osDelay(CHASSIS_TASK_INIT_TIME);

    //���̳�ʼ��
    chassis_init(&chassis_move);

    //�жϵ��̵���Ƿ�����
    while (chassis_motor_detect())
    {
        vTaskDelay(CHASSIS_CONTROL_TIME_MS);//��֤ȫ����ʼ�����
    }
		
    while (1)
		{
			//���̿���ģʽѡ��
			chassis_set_mode(&chassis_move);
			//�������ݸ���
			chassis_feedback_update(&chassis_move);
      //���̿��������ã�����ң���������������
      chassis_set_contorl(&chassis_move);
			//���̿���
			chassis_control_loop(&chassis_move);			

			
			if(POWER_OFF)
			//if(toe_is_error(DBUS_TOE))
			{
				
				CAN_cmd_course(0,0,0,0);
				CAN_cmd_drive(0,0,0,0);

			}
			else
			{
					
				CAN_cmd_course(chassis_move.chassis_course_speed_pid[0].out,chassis_move.chassis_course_speed_pid[1].out,chassis_move.chassis_course_speed_pid[2].out,chassis_move.chassis_course_speed_pid[3].out);
				CAN_cmd_drive(chassis_move.chassis_drive_speed_pid[0].out,chassis_move.chassis_drive_speed_pid[1].out,chassis_move.chassis_drive_speed_pid[2].out,chassis_move.chassis_drive_speed_pid[3].out);
			}
			osDelay(1);//����Ƶ��Ϊ1khz����can�����ж�Ƶ��һ��
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
	PID_init(&init->yaw_pid,DATA_NORMAL,YAW_ANGLE_PID,YAW_PID_MAX[0],YAW_PID_MAX[1],NONE);//��ʼ������PID
	//PID_init(&init->GM6020_pid,PID_POSITION,DATA_NORMAL,GM6020_SPEED_PID,GM6020_PID_MAX_OUT[0],GM6020_PID_MAX_OUT[1]);
	
	//��ʼ�������ٶ�PID ����ȡ�������
	for(i=0;i<4;i++)
	{
		init->motor_chassis[i].chassis_motor_measure = get_chassis_drive_motor_measure_point(i);//��ȡ����3508�����ݣ����յ���ķ����ṹ��		
		PID_init(&init->chassis_drive_speed_pid[i],DATA_NORMAL,PID_SPEED_DRIVE[i],SPEED_PID_MAX_OUT_DRIVE[i],SPEED_PID_MAX_IOUT_DRIVE[i],NONE);//��ʼ������PID
		init->drive_set_speed[i]=0.0f;
	}
	
	//��ʼ���������ٶȺͽǶ�PID����ȡ�������
	for(i=0;i<4;i++)
	{
		init->motor_chassis[i+4].chassis_motor_measure = get_chassis_course_motor_measure_point(i);//��ȡ�����������ݣ����յ���ķ����ṹ��
		PID_init(&init->chassis_course_speed_pid[i],DATA_NORMAL,PID_SPEED_COURSE[i],SPEED_PID_MAX_OUT_COURSE[i],SPEED_PID_MAX_IOUT_COURSE[i],NONE);//��ʼ���ٶ�PID
		PID_init(&init->chassis_course_angle_pid[i],DATA_NORMAL,PID_ANGLE_COURSE[i],ANGLE_PID_MAX_OUT_COURSE[i],ANGLE_PID_MAX_IOUT_COURSE[i],NONE);//��ʼ���Ƕ�PID
		init->course_set_angle[i]=0.0f;
	}
	//��һ���˲�
  first_order_filter_init(&init->chassis_cmd_slow_set_vx, CHASSIS_CONTROL_TIME, chassis_x_order_filter);
  first_order_filter_init(&init->chassis_cmd_slow_set_vy, CHASSIS_CONTROL_TIME, chassis_y_order_filter);
  first_order_filter_init(&init->chassis_cmd_slow_set_wz, CHASSIS_CONTROL_TIME_W, chassis_w_order_filter);
	
	//��� ��С�ٶ�
  init->vx_max_speed = NORMAL_MAX_CHASSIS_SPEED_X;
  init->vx_min_speed = -NORMAL_MAX_CHASSIS_SPEED_X;

  init->vy_max_speed = NORMAL_MAX_CHASSIS_SPEED_Y;
  init->vy_min_speed = -NORMAL_MAX_CHASSIS_SPEED_Y;
	
  init->wz_max_speed = NORMAL_MAX_CHASSIS_SPEED_W;
  init->wz_min_speed = -NORMAL_MAX_CHASSIS_SPEED_W;

	
	//���µ�������
	chassis_feedback_update(init);
}









/**
  * @brief          ���õ��̿���ģʽ����Ҫ��'chassis_behaviour_mode_set'�����иı�
  * @param[out]     chassis_move_mode:"chassis_move"����ָ��.
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
  * @brief          ����ѭ�������ݿ����趨ֵ������������ֵ�����п���
  * @param[out]     chassis_move_control_loop:"chassis_move"����ָ��.
  * @retval         none
  */
static void chassis_control_loop(chassis_move_t *chassis_move_control_loop)
{
	//�������ĽǶ�
	chassis_vector_to_agv_calculate(chassis_move_control_loop->course_set_angle,chassis_move_control_loop->drive_set_speed,
																	chassis_move_control_loop->vx_set,chassis_move_control_loop->vy_set,chassis_move_control_loop->wz_set);


	//����pid
	for (int i = 0; i < 4; i++)
		{
        PID_calc(&chassis_move_control_loop->chassis_drive_speed_pid[i], 
					chassis_move_control_loop->motor_chassis[i].speed,chassis_move_control_loop->drive_set_speed[i]);
		}
					//����6020�Ƕȴ��ٶȻ����ٶȻ����ڻ����ǶȻ����⻷
		for(int i=0;i<4;i++)
		{
				//�ǶȻ�
				PID_Calc_Ecd(&chassis_move_control_loop->chassis_course_angle_pid[i],
					chassis_move_control_loop->course_angle[i],chassis_move_control_loop->course_set_angle[i],360.0f);//���ﲻ��PID_calc����Ϊ������˵�ͨ�˲�����6020�ǶȻ�����
				//�ٶȻ�
				PID_calc(&chassis_move_control_loop->chassis_course_speed_pid[i],
					chassis_move_control_loop->motor_chassis[i+4].chassis_motor_measure->rpm,
					chassis_move_control_loop->chassis_course_angle_pid[i].out);
		}

		#ifdef CHASSIS_POWER_CONTROL
		chassis_power_control();
		#endif


}
/**
  * @brief          ����ecd��offset_ecd֮�����ԽǶ�
  * @param[in]      ecd: �����ǰ����
  * @param[in]      offset_ecd: �����ֵ����
  * @retval         ��ԽǶȣ���λrad
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
  * @brief          ���̲������ݸ��£���������ٶȣ�ŷ���Ƕȣ��������ٶ�
  * @param[out]     gimbal_feedback_update:"gimbal_control"����ָ��.
  * @retval         none
  */
static void chassis_feedback_update(chassis_move_t *feedback_update)
{
	uint8_t i = 0;
	
	//�����������ʵ���ٶ�-->�3.15m/s  (3.54m/s,���ת����)
	for (i = 0; i < 4; i++)
	{
		feedback_update->motor_chassis[i].speed = feedback_update->motor_chassis[i].chassis_motor_measure->rpm * (CHASSIS_MOTOR_RPM_TO_VECTOR_SEN);
	}
	
	feedback_update->wz_rad = feedback_update->chassis_bmi088_data->gyro[INS_GYRO_X_ADDRESS_OFFSET];
	
	feedback_update->chassis_yaw = feedback_update->chassis_bmi088_data->INS_angle[INS_YAW_ADDRESS_OFFSET];

	
		yaw_diff = motor_ecd_to_angle_change(yaw_motor.ecd,INIT_ECD[4]);
	
	
	for(i=0;i<4;i++)
	feedback_update->course_angle[i] = Angle_Limit((feedback_update->motor_chassis[4+i].chassis_motor_measure->ecd - INIT_ECD[i])*360/8192,360);
	
	chassis_power_feedback(&PowerLimit);
	#ifdef CHASSIS_DEBUG
	Chassis_Debug_get_data();
	#endif
}


/**
  * @brief          ���õ��̿�������ֵ, ���˶�����ֵ��ͨ��chassis_behaviour_control_set�������õ�
  * @param[out]     chassis_move_update:"chassis_move"����ָ��.
  * @retval         none
  */
static void chassis_set_contorl(chassis_move_t *chassis_move_control)
{
		//����Ƿ�Ϊ��ָ��
    if (chassis_move_control == NULL)
    {
        return;
    }
			//�����ٶ�
		
    fp32 vx_set = 0.0f, vy_set = 0.0f, wz_set = 0.0f;

			//���ݵ���״̬����������vx_set��vy_set��wz_set
    chassis_behaviour_control_set(&vx_set,&vy_set,&wz_set,chassis_move_control);
    
		//������̨yaw,����
		if (chassis_move_control->chassis_mode == CHASSIS_VECTOR_DIRECTION_FOLLOW_GIMBAL_YAW)
    {
			fp32 angle_diff;
			angle_diff=rad_format(yaw_diff* PI / 180.0f);
			
			chassis_move_control->vx_set = vy_set * cos(angle_diff) - vx_set * sin(angle_diff);
			chassis_move_control->vy_set = vy_set * sin(angle_diff) + vx_set * cos(angle_diff);
			
			if(fabs(angle_diff)*57.2957f > 5)
			{
				PID_calc(&chassis_move_control->yaw_pid,angle_diff,0);
				chassis_move_control->wz_set = chassis_move_control->yaw_pid.out;
			}
			else
				chassis_move_control->wz_set=0;
		
			

			
			chassis_move_control->wz_set = fp32_constrain(chassis_move_control->wz_set, chassis_move_control->wz_min_speed, chassis_move_control->wz_max_speed);
			chassis_move_control->vy_set = -fp32_constrain(vx_set, chassis_move_control->vx_min_speed, chassis_move_control->vx_max_speed);
			chassis_move_control->vx_set = fp32_constrain(vy_set, chassis_move_control->vy_min_speed, chassis_move_control->vy_max_speed);
			
		}
		
		//С����ģʽ������
    else if (chassis_move_control->chassis_mode == CHASSIS_VECTOR_TOP_FOLLOW_GIMBAL_YAW)
		{
			
			fp32 angle_diff=yaw_diff* PI / 180.0f;
			 
			chassis_move_control->vx_set = -vx_set * sin(angle_diff) + vy_set * cos(angle_diff);
			chassis_move_control->vy_set = -vx_set * cos(angle_diff) - vy_set * sin(angle_diff);
			
				
				if(chassis_move_control->get_gimbal_data->rc_data.rc_key_v & KEY_PRESSED_OFFSET_SHIFT  )
				{
					if(Key_ScanValue.Key_Value.W || Key_ScanValue.Key_Value.A || Key_ScanValue.Key_Value.S || Key_ScanValue.Key_Value.D)
						chassis_move_control->wz_set = 1.0f;	
					else
						chassis_move_control->wz_set = 2.5f;	
				}
				else
					chassis_move_control->wz_set = 2.0f;
		
				
				
			 chassis_move_control->wz_set = fp32_constrain(chassis_move_control->wz_set, chassis_move_control->wz_min_speed, chassis_move_control->wz_max_speed);
			 chassis_move_control->vx_set = fp32_constrain(chassis_move_control->vx_set, chassis_move_control->vx_min_speed, chassis_move_control->vx_max_speed);
			 chassis_move_control->vy_set = fp32_constrain(chassis_move_control->vy_set, chassis_move_control->vy_min_speed, chassis_move_control->vy_max_speed);
			
		}
		
		//��������������
    else if (chassis_move_control->chassis_mode == CHASSIS_VECTOR_ZERO_FORCE)
    {

				 chassis_move_control->vx_set = 0;
				 chassis_move_control->vy_set = 0;
				 chassis_move_control->wz_set = 0;	
			
    }
		
		

		//����ģʽ������
		else if(chassis_move_control->chassis_mode == CHASSIS_VECTOR_FLY)
		{
			fp32 angle_diff;

			angle_diff=rad_format(yaw_diff* PI / 180.0f);
			
			chassis_move_control->vx_set = vy_set * cos(angle_diff) - vx_set * sin(angle_diff);
			chassis_move_control->vy_set = vy_set * sin(angle_diff) + vx_set * cos(angle_diff);
			
			if(fabs(angle_diff)*57.2957f > 5)
			{
				PID_calc(&chassis_move_control->yaw_pid,angle_diff,0);
				chassis_move_control->wz_set = chassis_move_control->yaw_pid.out;
			}
			else
				chassis_move_control->wz_set=0;
		
			

			
			chassis_move_control->wz_set = fp32_constrain(chassis_move_control->wz_set, chassis_move_control->wz_min_speed, chassis_move_control->wz_max_speed);
			chassis_move_control->vx_set = fp32_constrain(chassis_move_control->vx_set, chassis_move_control->vx_min_speed, chassis_move_control->vx_max_speed);
			chassis_move_control->vy_set = -fp32_constrain(chassis_move_control->vy_set, chassis_move_control->vy_min_speed, chassis_move_control->vy_max_speed);
			
		}
}


//6020
//2	4
//3	1

//3508
//1	4
//3	2
fp32 drct[4] = {1,1,1,1};
static void chassis_vector_to_agv_calculate(fp32 wheel_angle[4],fp32 wheel_speed[4],fp32 vx_set,fp32 vy_set,fp32 wz_set)
{
	//��һ�νǶ�
	static fp32 last_wheel_angle[4];
	
	if(chassis_move.chassis_mode != CHASSIS_VECTOR_FLY)
	{
		//0
		wheel_angle[0]=Angle_Limit(atan2(vy_set - wz_set*0.707f,vx_set - wz_set*0.707f)/PI*180.0,360.0f); //225(-135)(0 -- 360)

		//Ѱ����Сת��Ȼ������3508����ת��Ȼ������pid�Ĺ��㴦���������ȷ��err
		//���磺ref:30   set:130  
		//�����º�����wheel_angle[0] = 130+180 = 310;
		//pid����ecd_zero(set, ref, ecd_range);�����err=-80��
		//Ȼ����������ݾ�����6020��ת80�ȵ���-50�ȣ���130�ȸպù���180�ȣ���ʱ��drctȡ���ˣ��������ӻ���Ч
		if(fabs(Find_min_Angle(chassis_move.course_angle[0],wheel_angle[0]))>90)
		{
				wheel_angle[0] += 180;		
				wheel_angle[0]=Angle_Limit(wheel_angle[0],360);
				drct[0] = 1;
		}
		else
				drct[0]=-1;
		wheel_speed[1] = drct[0]*sqrt(pow((vy_set - wz_set*0.707f),2)+pow(vx_set - wz_set*0.707f,2));
		
		
		
		//1
		wheel_angle[1]=Angle_Limit(atan2(vy_set + wz_set*0.707f,vx_set + wz_set*0.707f)/PI*180.0,360.0f); //45 (0 -- 360)
		if(fabs(Find_min_Angle(chassis_move.course_angle[1],wheel_angle[1]))>90)
		{
				wheel_angle[1] += 180;		
				wheel_angle[1]=Angle_Limit(wheel_angle[1],360);
				drct[1] = -1;
		}
		else
				drct[1]=1;
		wheel_speed[0] = drct[1]*sqrt(pow((vy_set + wz_set*0.707f),2)+pow(vx_set + wz_set*0.707f,2));

		
		
		
		//2
				wheel_angle[2]=Angle_Limit(atan2(vy_set - wz_set*0.707f,vx_set + wz_set*0.707f)/PI*180.0,360.0f);//315(-45)  (0 -- 360)

		if(fabs(Find_min_Angle(chassis_move.course_angle[2],wheel_angle[2]))>90)
		{
				wheel_angle[2] += 180;		
				wheel_angle[2]=Angle_Limit(wheel_angle[2],360);
				drct[2] = -1;
		}
		else
				drct[2]= 1;
		wheel_speed[2] = drct[2]*sqrt(pow((vy_set - wz_set*0.707f),2)+pow(vx_set + wz_set*0.707f,2));


		//3
		wheel_angle[3]=Angle_Limit(atan2(vy_set + wz_set*0.707f,vx_set - wz_set*0.707f)/PI*180.0,360.0f); //135 (0 -- 360)
		if(fabs(Find_min_Angle(chassis_move.course_angle[3],wheel_angle[3]))>90)
		{
				wheel_angle[3] += 180;		
				wheel_angle[3]=Angle_Limit(wheel_angle[3],360);
				drct[3] = -1;
		}
		else
				drct[3]=1;
		wheel_speed[3] = drct[3]*sqrt(pow((vy_set + wz_set*0.707f),2)+pow(vx_set - wz_set*0.707f,2));

		PowerLimit.factor_Course = pow((sin(Find_min_Angle(chassis_move.course_angle[0],wheel_angle[0]))),2);
		PowerLimit.factor_Drive = pow((cos(Find_min_Angle(chassis_move.course_angle[0],wheel_angle[0]))),2);
	}
	else
	{
		//0
		wheel_angle[0]=Angle_Limit(atan2(vy_set - wz_set*0.707f,vx_set - wz_set*0.707f)/PI*180.0,360.0f); //225(-135)(0 -- 360)
		wheel_speed[1] = -sqrt(pow((vy_set - wz_set*0.707f),2)+pow(vx_set - wz_set*0.707f,2));

		//1
		wheel_angle[1]=Angle_Limit(atan2(vy_set + wz_set*0.707f,vx_set + wz_set*0.707f)/PI*180.0,360.0f); //45 (0 -- 360)
		wheel_speed[0] = sqrt(pow((vy_set + wz_set*0.707f),2)+pow(vx_set + wz_set*0.707f,2));

	
		//2
				wheel_angle[2]=Angle_Limit(atan2(vy_set - wz_set*0.707f,vx_set + wz_set*0.707f)/PI*180.0,360.0f);//315(-45)  (0 -- 360)
		wheel_speed[2] = sqrt(pow((vy_set - wz_set*0.707f),2)+pow(vx_set + wz_set*0.707f,2));

		//3
		wheel_angle[3]=Angle_Limit(atan2(vy_set + wz_set*0.707f,vx_set - wz_set*0.707f)/PI*180.0,360.0f); //135 (0 -- 360)
		wheel_speed[3] = sqrt(pow((vy_set + wz_set*0.707f),2)+pow(vx_set - wz_set*0.707f,2));
	}

		
		if(vx_set == 0 && vy_set == 0 && wz_set == 0)//ҡ�˻���ʱ������6020�Ƕ�
		{
			for(int i=0;i<4;i++)//memcpy��������
			wheel_angle[i] = last_wheel_angle[i];
		}
		else
		{
			for(int i=0;i<4;i++)
			{
				last_wheel_angle[i]=wheel_angle[i];
			}
		}
		for(int i=0;i<4;i++)//�����ֶ���� �ͻ������
		{
			if(fabs(wheel_angle[i]-last_wheel_angle[i])<10)//С��10��ά��ԭֵ
			{
				wheel_angle[i]=last_wheel_angle[i];
			}
		}	

	for(uint8_t i=0;i<4;i++)	
	wheel_speed[i] = fp32_constrain(wheel_speed[i], chassis_move.vy_min_speed, chassis_move.vy_max_speed);


}




/**
  * @brief          ����ң����ͨ��ֵ����������ͺ����ٶ�               
  * @param[out]     vx_set: �����ٶ�ָ��
  * @param[out]     vy_set: �����ٶ�ָ��
  * @param[out]     chassis_move_rc_to_vector: "chassis_move" ����ָ��
  * @retval         none
  */
void chassis_rc_to_control_vector(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector)
{
	int16_t vx_channel, vy_channel, wz_channel;
  fp32 rc_vx=0, rc_vy=0,rc_wz=0;
	fp32 key_vx=0,key_vy=0;
	fp32 final_vx=0,final_vy=0,final_wz=0;
	
  //�������ƣ���Ϊң�������ܴ��ڲ��� ҡ�����м䣬��ֵ��Ϊ0
//  rc_deadband_limit(chassis_move_rc_to_vector->chassis_RC->rc.ch[CHASSIS_X_CHANNEL], vx_channel, CHASSIS_RC_DEADLINE);
//  rc_deadband_limit(chassis_move_rc_to_vector->chassis_RC->rc.ch[CHASSIS_Y_CHANNEL], vy_channel, CHASSIS_RC_DEADLINE);
//  rc_deadband_limit(chassis_move_rc_to_vector->chassis_RC->rc.ch[CHASSIS_W_CHANNEL], wz_channel, CHASSIS_RC_DEADLINE);
	
  rc_deadband_limit(chassis_move_rc_to_vector->get_gimbal_data->rc_data.vx_set, vx_channel, CHASSIS_RC_DEADLINE);
  rc_deadband_limit(chassis_move_rc_to_vector->get_gimbal_data->rc_data.vy_set, vy_channel, CHASSIS_RC_DEADLINE);
  rc_deadband_limit(chassis_move_rc_to_vector->get_gimbal_data->rc_data.wz_set, wz_channel, CHASSIS_RC_DEADLINE);

		rc_vx = -vx_channel * CHASSIS_VX_RC_SEN;
    rc_vy = vy_channel * CHASSIS_VY_RC_SEN;
	  rc_wz = wz_channel * CHASSIS_WZ_RC_SEN;

    //���̿���
    if (chassis_move_rc_to_vector->get_gimbal_data->rc_data.rc_key_v & KEY_PRESSED_OFFSET_D)
    {
        key_vx = -MAX_SPEED;
    }
    else if (chassis_move_rc_to_vector->get_gimbal_data->rc_data.rc_key_v  & KEY_PRESSED_OFFSET_A)
    {
        key_vx = MAX_SPEED;
    }

    if (chassis_move_rc_to_vector->get_gimbal_data->rc_data.rc_key_v  & KEY_PRESSED_OFFSET_S)
    {
        key_vy = -MAX_SPEED;
    }
    else if (chassis_move_rc_to_vector->get_gimbal_data->rc_data.rc_key_v  & KEY_PRESSED_OFFSET_W)
    {
        key_vy =MAX_SPEED;
    }
	
	final_vx = key_vx+rc_vx;
	final_vy = key_vy+rc_vy;
	final_wz = rc_wz;		
		
	final_wz = fp32_constrain(final_wz, chassis_move_rc_to_vector->wz_min_speed, chassis_move_rc_to_vector->wz_max_speed);
	final_vx = fp32_constrain(final_vx, chassis_move_rc_to_vector->vx_min_speed, chassis_move_rc_to_vector->vx_max_speed);	
	final_vy = fp32_constrain(final_vy, chassis_move_rc_to_vector->vy_min_speed, chassis_move_rc_to_vector->vy_max_speed);	
		
	//һ�׵�ͨ�˲�����б����Ϊ�����ٶ�����
  first_order_filter_cali(&chassis_move_rc_to_vector->chassis_cmd_slow_set_vx, final_vx);
  first_order_filter_cali(&chassis_move_rc_to_vector->chassis_cmd_slow_set_vy, final_vy);
  first_order_filter_cali(&chassis_move_rc_to_vector->chassis_cmd_slow_set_wz, final_wz);
	
  if (final_vx < CHASSIS_RC_DEADLINE * CHASSIS_VX_RC_SEN && final_vx > -CHASSIS_RC_DEADLINE * CHASSIS_VX_RC_SEN)
  {
      chassis_move_rc_to_vector->chassis_cmd_slow_set_vx.out = 0.0f;
  }

  if (final_vy < CHASSIS_RC_DEADLINE * CHASSIS_VY_RC_SEN && final_vy > -CHASSIS_RC_DEADLINE * CHASSIS_VY_RC_SEN)
  {
      chassis_move_rc_to_vector->chassis_cmd_slow_set_vy.out = 0.0f;
  }
  if (final_wz < CHASSIS_RC_DEADLINE * CHASSIS_WZ_RC_SEN && final_wz > -CHASSIS_RC_DEADLINE * CHASSIS_WZ_RC_SEN)
  {
      chassis_move_rc_to_vector->chassis_cmd_slow_set_wz.out = 0.0f;
  }    
	
	*vx_set = chassis_move_rc_to_vector->chassis_cmd_slow_set_vx.out;
  *vy_set = chassis_move_rc_to_vector->chassis_cmd_slow_set_vy.out;
	*wz_set = chassis_move_rc_to_vector->chassis_cmd_slow_set_wz.out;

}




/**
	* @brief Ѱ����Сת���Ƕ�
	* @param  angle1,angle2 һ��ΪĿ��Ƕȣ�һ��Ϊ��ǰ�Ƕ�
  * @retval �ǶȲ�ֵ
  * @attention	���صĲ�ֵ�Ǿ��������������0-360֮���
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
/* -------------------------------vofa����-------------------------------- */
/* ----------------------------------------------------------------------- */
static void Chassis_Debug_get_data(void)
{
	chassis_move.chassis_debug_data.data1 = chassis_move.motor_chassis[0].speed;
	chassis_move.chassis_debug_data.data2 = chassis_move.motor_chassis[1].speed;
	chassis_move.chassis_debug_data.data3 = chassis_move.motor_chassis[2].speed;
	
	chassis_move.chassis_debug_data.data4 = chassis_move.motor_chassis[3].speed;
	
//	chassis_move.chassis_debug_data.data4 = chassis_move.drive_set_speed[0];
//	chassis_move.chassis_debug_data.data5 = chassis_move.drive_set_speed[1];
//	chassis_move.chassis_debug_data.data6 = chassis_move.drive_set_speed[2];
}
const DebugData* get_chassis_Debug(void)
{
	return &chassis_move.chassis_debug_data;
}


