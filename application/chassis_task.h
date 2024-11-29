#ifndef __CHASSIS_TASK__
#define __CHASSIS_TASK__

#include "main.h"
#include "Can_receive.h"
#include "cmsis_os.h"
#include "remote_control.h"
#include "math.h"
#include "INS_task.h"
#include "detect_task.h"
#include "pid.h"
#include "user_lib.h"
#include "vofa_task.h"
#include "user_task.h"
/* --------------------------------------yaw轴-------------------------------------------------*/

#define YAW_ANGLE_PID_KP                 0.3f
#define YAW_ANGLE_PID_KI                 0.0f
#define YAW_ANGLE_PID_KD                 0.0f
#define YAW_PID_MAX_OUT            1.80f //最大输出值
#define YAW_PID_MAX_IOUT           0.0f //最大输出电流



/* --------------------------------------舵轮1-------------------------------------------------*/
//驱动电机速度环
#define DRIVE_MOTOR1_SPEED_PID_KP                 30000.0f
#define DRIVE_MOTOR1_SPEED_PID_KI                 2.0f
#define DRIVE_MOTOR1_SPEED_PID_KD                 0.0f
#define DRIVE_MOTOR1_SPEED_PID_MAX_OUT            16384.0f //最大输出值
#define DRIVE_MOTOR1_SPEED_PID_MAX_IOUT           100.0f //最大输出电流




//航向电机角度环
#define COURSE_MOTOR1_ANGLE_PID_KP                 	4.0f//105
#define COURSE_MOTOR1_ANGLE_PID_KI                	0.10f
#define COURSE_MOTOR1_ANGLE_PID_KD                 	0.0f
#define COURSE_MOTOR1_ANGLE_PID_MAX_OUT							360.0f //最大输出角度
#define COURSE_MOTOR1_ANGLE_PID_MAX_IOUT						10.0f			



//航向电机速度环
#define COURSE_MOTOR1_SPEED_PID_KP                 	150.0f
#define COURSE_MOTOR1_SPEED_PID_KI                	0.0f
#define COURSE_MOTOR1_SPEED_PID_KD                 	0.0f
#define COURSE_MOTOR1_SPEED_PID_MAX_OUT							30000.0f//最大输出速度   -30000  -   30000
#define COURSE_MOTOR1_SPEED_PID_MAX_IOUT						0.0f			



/* --------------------------------------舵轮2-------------------------------------------------*/
//驱动电机速度环
#define DRIVE_MOTOR2_SPEED_PID_KP                 30000.0f
#define DRIVE_MOTOR2_SPEED_PID_KI                 2.0f
#define DRIVE_MOTOR2_SPEED_PID_KD                 0.0f
#define DRIVE_MOTOR2_SPEED_PID_MAX_OUT            16384.0f //最大输出值
#define DRIVE_MOTOR2_SPEED_PID_MAX_IOUT           100.0f //最大输出电流



//航向电机角度环
#define COURSE_MOTOR2_ANGLE_PID_KP                 	3.0f//20
#define COURSE_MOTOR2_ANGLE_PID_KI                	0.10f
#define COURSE_MOTOR2_ANGLE_PID_KD                 	0.0f
#define COURSE_MOTOR2_ANGLE_PID_MAX_OUT							360.0f //最大输出角度
#define COURSE_MOTOR2_ANGLE_PID_MAX_IOUT						10.0f			



//航向电机速度环
#define COURSE_MOTOR2_SPEED_PID_KP                 	100.0f
#define COURSE_MOTOR2_SPEED_PID_KI                	0.0f
#define COURSE_MOTOR2_SPEED_PID_KD                 	0.0f
#define COURSE_MOTOR2_SPEED_PID_MAX_OUT							30000.0f//最大输出速度   -30000  -   30000
#define COURSE_MOTOR2_SPEED_PID_MAX_IOUT						0.0f			

/* --------------------------------------舵轮3-------------------------------------------------*/
//驱动电机速度环
#define DRIVE_MOTOR3_SPEED_PID_KP                 30000.0f
#define DRIVE_MOTOR3_SPEED_PID_KI                 2.0f
#define DRIVE_MOTOR3_SPEED_PID_KD                 0.0f
#define DRIVE_MOTOR3_SPEED_PID_MAX_OUT            16384.0f //最大输出值
#define DRIVE_MOTOR3_SPEED_PID_MAX_IOUT           100.0f //最大输出电流



//航向电机角度环
#define COURSE_MOTOR3_ANGLE_PID_KP                 	3.0f//35
#define COURSE_MOTOR3_ANGLE_PID_KI                	0.10f
#define COURSE_MOTOR3_ANGLE_PID_KD                 	0.0f
#define COURSE_MOTOR3_ANGLE_PID_MAX_OUT							360.0f //最大输出角度
#define COURSE_MOTOR3_ANGLE_PID_MAX_IOUT						10.0f			



//航向电机速度环
#define COURSE_MOTOR3_SPEED_PID_KP                 	250.0f
#define COURSE_MOTOR3_SPEED_PID_KI                	0.0f
#define COURSE_MOTOR3_SPEED_PID_KD                 	0.0f
#define COURSE_MOTOR3_SPEED_PID_MAX_OUT							30000.0f//最大输出速度   -30000  -   30000
#define COURSE_MOTOR3_SPEED_PID_MAX_IOUT						0.0f			



/* --------------------------------------舵轮4-------------------------------------------------*/
//驱动电机速度环
#define DRIVE_MOTOR4_SPEED_PID_KP                 30000.0f
#define DRIVE_MOTOR4_SPEED_PID_KI                 0.0f
#define DRIVE_MOTOR4_SPEED_PID_KD                 0.0f
#define DRIVE_MOTOR4_SPEED_PID_MAX_OUT            16384.0f //最大输出值
#define DRIVE_MOTOR4_SPEED_PID_MAX_IOUT           0.0f //最大输出电流



//航向电机角度环
#define COURSE_MOTOR4_ANGLE_PID_KP                 	3.0f//11
#define COURSE_MOTOR4_ANGLE_PID_KI                	0.10f
#define COURSE_MOTOR4_ANGLE_PID_KD                 	0.0f
#define COURSE_MOTOR4_ANGLE_PID_MAX_OUT							360.0f //最大输出角度
#define COURSE_MOTOR4_ANGLE_PID_MAX_IOUT						10.0f			



//航向电机速度环
#define COURSE_MOTOR4_SPEED_PID_KP                 	200.0f
#define COURSE_MOTOR4_SPEED_PID_KI                	0.0f
#define COURSE_MOTOR4_SPEED_PID_KD                 	0.0f
#define COURSE_MOTOR4_SPEED_PID_MAX_OUT							30000.0f//最大输出速度   -30000  -   30000
#define COURSE_MOTOR4_SPEED_PID_MAX_IOUT						0.0f			





typedef enum
{
  CHASSIS_VECTOR_FOLLOW_GIMBAL_YAW,   //chassis will follow yaw gimbal motor relative angle.底盘会跟随云台相对角度
  CHASSIS_VECTOR_FOLLOW_CHASSIS_YAW,  //chassis will have yaw angle(chassis_yaw) close-looped control.底盘有底盘角度控制闭环
  CHASSIS_VECTOR_NO_FOLLOW_YAW,       //chassis will have rotation speed control. 底盘有旋转速度控制
  CHASSIS_VECTOR_ZERO_FORCE,                 //control-current will be sent to CAN bus derectly.

} chassis_mode_e;



typedef struct
{
  const motor_measure_t *chassis_motor_measure;//电机数据反馈值
  fp32 accel;
  fp32 speed;
  fp32 speed_set;
  int16_t give_current;
} chassis_motor_t;



typedef struct
{
	const RC_ctrl_t *chassis_RC;               					//底盘使用的遥控器指针, the point to remote control
  chassis_motor_t motor_chassis[8];          					//chassis motor data.底盘电机数据
	const bmi088_real_data_t *chassis_bmi088_data;     										//获取陀螺仪解算出的欧拉角指针
	const gimbal_data_t *get_gimbal_data;								//底盘获得云台数据
  pid_type_def chassis_drive_speed_pid[4];             //驱动电机速度pid
  pid_type_def chassis_course_angle_pid[4];            //航向电机角度pid
  pid_type_def chassis_course_speed_pid[4];						 //航向电机速度pid	
	pid_type_def yaw_pid;

  first_order_filter_type_t chassis_cmd_slow_set_vx;  //一阶低通滤波减缓设定值
  first_order_filter_type_t chassis_cmd_slow_set_vy;  //一阶低通滤波减缓设定值
  first_order_filter_type_t chassis_cmd_slow_set_wz;  //一阶低通滤波减缓设定值
	
	fp32 course_angle[4];																//6020角度 
  fp32 course_set_angle[4];       										//6020最终设定角度
	fp32 course_set_last_angle[4];       								//6020上次设定角度
  fp32 drive_set_speed[4]; 														//3508最终计算出的速度	

	
  chassis_mode_e chassis_mode;               					//底盘控制状态机
  chassis_mode_e last_chassis_mode;          					//底盘上次控制状态机
	int8_t drct;
	fp32 vx_set;																				// m/s
	fp32 vy_set;																				// m/s
	fp32 wz_set;																				// m/s
	fp32 wz_rad;																				// rad/s
	
	
	fp32 vx_max_speed;  //前进方向最大速度 单位m/s
  fp32 vx_min_speed;  //后退方向最大速度 单位m/s
  fp32 vy_max_speed;  //左方向最大速度 单位m/s
  fp32 vy_min_speed;  //右方向最大速度 单位m/s
  fp32 wz_max_speed;  //左方向最大速度 单位m/s
  fp32 wz_min_speed;  //右方向最大速度 单位m/s
	
	fp32 chassis_yaw;   //陀螺仪和云台电机叠加的yaw角度
  fp32 chassis_pitch; //陀螺仪和云台电机叠加的pitch角度
  fp32 chassis_roll;  //陀螺仪和云台电机叠加的roll角度

	DebugData chassis_debug_data;

	
} chassis_move_t;


extern chassis_move_t chassis_move;//底盘运动数据

/*******************************一节低通滤波参数************************/
#define CHASSIS_CONTROL_TIME 0.01f   //x和y本次信任参数
#define CHASSIS_CONTROL_TIME_W 0.01f  //z本次信任参数

//信任上一次参数占比
#define CHASSIS_ACCEL_X_NUM 0.99f
#define CHASSIS_ACCEL_Y_NUM 0.99f
#define CHASSIS_ACCEL_W_NUM 0.99f

/*******************************轮组数据*******************************/
#define R       MOTOR_DISTANCE_TO_CENTER
#define PI       3.1415926f

//电机码盘值最大以及中值
#define HALF_ECD_RANGE  4096
#define ECD_RANGE       8191

//1:14减速比
#define M3508_MOTOR_RPM_TO_VECTOR 	WHEEL_CIRCUMFERENCE/60.0f/14	
#define CHASSIS_MOTOR_RPM_TO_VECTOR_SEN M3508_MOTOR_RPM_TO_VECTOR


//轮距300mm 0.3m
#define TRACK_WIDTH							0.3f
//轴距300mm 0.3m
#define WHEEL_BASE							0.3f

//小陀螺半径 m 车体是正方形则直接×1.414
#define MOTOR_DISTANCE_TO_CENTER 	WHEEL_BASE/2.0f*1.414f

//小陀螺周长
#define SMALL_TOP_CIRCUMFERENCE	 	MOTOR_DISTANCE_TO_CENTER*2*3.1415926f			

//轮子半径  m 
#define WHEEL_HALF_SIZE 	0.054f

//轮子周长	m
#define WHEEL_CIRCUMFERENCE				WHEEL_HALF_SIZE*2*3.1415926f  	

//前进最大速度  3.597498/s   --8911
//									0						0
#define NORMAL_MAX_CHASSIS_SPEED_X 3.597498f
#define NORMAL_MAX_CHASSIS_SPEED_Y 3.597498f
#define NORMAL_MAX_CHASSIS_SPEED_W 3.597498f

//减速比14，rpm: 圈/min
//遥控器前进摇杆（max 660）转化成车体前进速度（m/s）的比例	
#define CHASSIS_VX_RC_SEN            0.0054507f

//遥控器左右摇杆（max 660）转化成车体左右速度（m/s）的比例
#define CHASSIS_VY_RC_SEN 					 0.0054507f

//不跟随云台的时候 遥控器的yaw遥杆（max 660）转化成车体旋转速度的比例
#define CHASSIS_WZ_RC_SEN 					 0.0054507f


#define CHASSIS_TASK_INIT_TIME 1000
#define CHASSIS_CONTROL_TIME_MS 1000
#define CHASSIS_X_CHANNEL	2
#define CHASSIS_Y_CHANNEL 3
#define CHASSIS_W_CHANNEL 0

//y轴
#define GIM_Y_ECD_1				8116.0f
#define GIM_Y_ECD_2				3213.0f
#define GIM_Y_ECD_3				7614.0f
#define GIM_Y_ECD_4				3843.0f


//摇杆死区
#define CHASSIS_RC_DEADLINE 50


//选择底盘状态 开关通道号
#define CHASSIS_MODE_CHANNEL 0

//电机编码值转化成角度值
#ifndef MOTOR_ECD_TO_RAD
#define MOTOR_ECD_TO_RAD 0.0439453125f //      2*  PI  /8192
#endif
/**
  * @brief          底盘任务，间隔 CHASSIS_CONTROL_TIME_MS 2ms
  * @param[in]      pvParameters: 空
  * @retval         none
  */
void chassis_task(void const * argument);
void chassis_rc_to_control_vector(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector);
const DebugData* get_chassis_PID_Debug(void);



#endif



