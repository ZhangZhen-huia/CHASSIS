#ifndef TRIG_TASK_H
#define TRIG_TASK_H



#include "main.h"
#include "pid.h"
#include "Can_receive.h"
#include "remote_control.h"
#include "user_task.h"


//任务初始化时间
#define TRIG_TASK_INIT_TIME    500


//拨弹盘速度pid
#define TRIG_SPEED_PID_KP           15.0f
#define TRIG_SPEED_PID_KI           0.0f
#define TRIG_SPEED_PID_KD           0.0f
#define TRIG_SPEED_PID_MAX_OUT     10000.0f
#define TRIG_SPEED_PID_MAX_IOUT    0.0f

#define TRIG_SPEED_KF_STATIC        0.0f
#define TRIG_SPEED_KF_DYNAMIC       0.0f


//拨弹盘角度pid
#define TRIG_ANGLE_PID_KP           0.30f
#define TRIG_ANGLE_PID_KI           0.0f
#define TRIG_ANGLE_PID_KD           0.0f
#define TRIG_ANGLE_PID_MAX_OUT     10000.0f   //输出10000rpm有点大了
#define TRIG_ANGLE_PID_MAX_IOUT    0.0f

#define TRIG_ANGLE_KF_STATIC        0.0f
#define TRIG_ANGLE_KF_DYNAMIC       0.0f


#define Single_ARR  2500
#define Serial_ARR 	385

#define TIRG_MODE_CHANNEL 1


//shoot相关电机信息数据包
typedef struct
{
	const motor_measure_t  *shoot_motor_measure;
	fp32 motor_speed;
  fp32 motor_speed_set;
  int16_t current_set;
	pid_type_def shoot_speed_pid;
	pid_type_def shoot_angle_pid;

} shoot_motor_t;



typedef enum
{
	Single_fire=0,
	Serial_fire,
	Cease_fire,

}trig_fire_mode_e;



//发射总结构体
typedef struct 
{
    const rc_data_t *rc_data;																		//获取遥控器指针
		shoot_motor_t shoot_trig_motor;		
		trig_fire_mode_e trig_fire_mode;
		trig_fire_mode_e last_trig_fire_mode;
}shoot_control_t;



extern shoot_control_t trig_control;

#endif
