#ifndef TRIG_TASK_H
#define TRIG_TASK_H



#include "main.h"
#include "pid.h"
#include "Can_receive.h"
#include "remote_control.h"
#include "communicate_task.h"
#include "referee.h"

//任务初始化时间
#define TRIG_TASK_INIT_TIME    500
#define TRIG_TASK_CONTROL_TIME 500


//串级
#define TRIG_SPEED_PID_KP_CASCADE           2000.0f
#define TRIG_SPEED_PID_KI_CASCADE           0.0f
#define TRIG_SPEED_PID_KD_CASCADE           20.0f
#define TRIG_SPEED_PID_MAX_OUT_CASCADE     10000.0f
#define TRIG_SPEED_PID_MAX_IOUT_CASCADE    0.0f

#define TRIG_ANGLE_PID_KP_CASCADE           20.0f
#define TRIG_ANGLE_PID_KI_CASCADE           0.2f
#define TRIG_ANGLE_PID_KD_CASCADE           2.0f
#define TRIG_ANGLE_PID_MAX_OUT_CASCADE     10000.0f   //输出10000rpm有点大了
#define TRIG_ANGLE_PID_MAX_IOUT_CASCADE    0.0f


//单环
#define TRIG_SPEED_PID_KP_SINGLE           2150.0f
#define TRIG_SPEED_PID_KI_SINGLE           110.0f
#define TRIG_SPEED_PID_KD_SINGLE           0.0f
#define TRIG_SPEED_PID_MAX_OUT_SINGLE     10000.0f
#define TRIG_SPEED_PID_MAX_IOUT_SINGLE    1000.0f


//定义期望基础拨弹盘拨弹速度 弹/s
#define TRIG_BASE_SPEED 30.0f

//最终爆发模式,拼命打弹，能打出来多是是多少
#define TRIG_EXPLOSION_SPEED 					30.0f

#define TRIG_BACK_SPEED		20.0f
//定义默认卡弹转速(rpm)  
#define  STANDARD_NOMOVE_RPM          15
//定义默认卡弹时间 
#define  STANDARD_NOMOVE_TIME         250
//定义默认回转时间
#define  STANDARD_REMOVE_TIME         200
//一发弹丸角度
#define PI_TEN 0.31415926f

#define TIRG_MODE_CHANNEL 1



//定义拨弹轮转速（rpm）转化为弹丸线速（m/s）
//拨弹电机拨弹速度（sps= one shot per seconds）计算
//2006电机减速比36：1
//1/60/36*10= 0.004629629f
//即1rpm/min的拨弹速度每秒发射0.0037ke子弹
#define  TRIG_RPM_TO_SPEED_SEN     						 0.004629629f//0.0001745f





//shoot相关电机信息数据包
typedef struct
{
	const motor_measure_t  *shoot_motor_measure;
	fp32 motor_speed;
  fp32 motor_speed_set;
	fp32 motor_angle;
  int16_t current_set;
	pid_type_def shoot_speed_pid_cascade;
	pid_type_def shoot_angle_pid_cascade;
	pid_type_def shoot_speed_pid_single;
} shoot_motor_t;



typedef enum
{
	Single_fire=0,
	Serial_fire,
}trig_mode_e;



//发射总结构体
typedef struct 
{
    const rc_data_t *rc_data;																		//获取遥控器指针
		shoot_motor_t shoot_trig_motor;	
		trig_mode_e trig_fire_mode;
		Referee_System_t     *trig_referee;   //发射机构裁判系统数据
		uint8_t Fire;
		uint8_t ultimate_explosion;
}shoot_control_t;



extern osTimerId ShootTimerHandle;
extern shoot_control_t trig_control;

#endif
