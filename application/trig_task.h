#ifndef TRIG_TASK_H
#define TRIG_TASK_H



#include "main.h"
#include "pid.h"
#include "Can_receive.h"
#include "remote_control.h"
#include "communicate_task.h"


//任务初始化时间
#define TRIG_TASK_INIT_TIME    500
#define TRIG_TASK_CONTROL_TIME 500


//串级
#define TRIG_SPEED_PID_KP_CASCADE           15.0f
#define TRIG_SPEED_PID_KI_CASCADE           0.0f
#define TRIG_SPEED_PID_KD_CASCADE           0.0f
#define TRIG_SPEED_PID_MAX_OUT_CASCADE     10000.0f
#define TRIG_SPEED_PID_MAX_IOUT_CASCADE    0.0f

#define TRIG_ANGLE_PID_KP_CASCADE           0.30f
#define TRIG_ANGLE_PID_KI_CASCADE           0.0f
#define TRIG_ANGLE_PID_KD_CASCADE           0.0f
#define TRIG_ANGLE_PID_MAX_OUT_CASCADE     10000.0f   //输出10000rpm有点大了
#define TRIG_ANGLE_PID_MAX_IOUT_CASCADE    0.0f


//单环
#define TRIG_SPEED_PID_KP_SINGLE           2150.0f
#define TRIG_SPEED_PID_KI_SINGLE           110.0f
#define TRIG_SPEED_PID_KD_SINGLE           0.0f
#define TRIG_SPEED_PID_MAX_OUT_SINGLE     10000.0f
#define TRIG_SPEED_PID_MAX_IOUT_SINGLE    1000.0f


#define Single_ARR  2500
#define Serial_ARR 	500

#define TRIG_BASE_SPEED 10.0f

#define TIRG_MODE_CHANNEL 1



//定义拨弹轮转速（rpm）转化为弹丸线速（m/s）
//拨弹电机拨弹速度（sps= one shot per seconds）计算
//2006电机减速比36：1
//1/60/36*8= 0.00370370f
//即1rpm/min的拨弹速度每秒发射0.0037ke子弹
#define  TRIG_RPM_TO_SPEED_SEN     						 0.0037037f//0.0001745f





//shoot相关电机信息数据包
typedef struct
{
	const motor_measure_t  *shoot_motor_measure;
	fp32 motor_speed;
  fp32 motor_speed_set;
  int16_t current_set;
	pid_type_def shoot_speed_pid_cascade;
	pid_type_def shoot_angle_pid_cascade;
	pid_type_def shoot_speed_pid_single;
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


extern osTimerId ShootTimerHandle;
extern shoot_control_t trig_control;

#endif
