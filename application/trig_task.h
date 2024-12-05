#ifndef TRIG_TASK_H
#define TRIG_TASK_H



#include "main.h"
#include "pid.h"
#include "Can_receive.h"
#include "remote_control.h"
#include "communicate_task.h"


//�����ʼ��ʱ��
#define TRIG_TASK_INIT_TIME    500
#define TRIG_TASK_CONTROL_TIME 500


//����
#define TRIG_SPEED_PID_KP_CASCADE           15.0f
#define TRIG_SPEED_PID_KI_CASCADE           0.0f
#define TRIG_SPEED_PID_KD_CASCADE           0.0f
#define TRIG_SPEED_PID_MAX_OUT_CASCADE     10000.0f
#define TRIG_SPEED_PID_MAX_IOUT_CASCADE    0.0f

#define TRIG_ANGLE_PID_KP_CASCADE           0.30f
#define TRIG_ANGLE_PID_KI_CASCADE           0.0f
#define TRIG_ANGLE_PID_KD_CASCADE           0.0f
#define TRIG_ANGLE_PID_MAX_OUT_CASCADE     10000.0f   //���10000rpm�е����
#define TRIG_ANGLE_PID_MAX_IOUT_CASCADE    0.0f


//����
#define TRIG_SPEED_PID_KP_SINGLE           2150.0f
#define TRIG_SPEED_PID_KI_SINGLE           110.0f
#define TRIG_SPEED_PID_KD_SINGLE           0.0f
#define TRIG_SPEED_PID_MAX_OUT_SINGLE     10000.0f
#define TRIG_SPEED_PID_MAX_IOUT_SINGLE    1000.0f


#define Single_ARR  2500
#define Serial_ARR 	500

#define TRIG_BASE_SPEED 10.0f

#define TIRG_MODE_CHANNEL 1



//���岦����ת�٣�rpm��ת��Ϊ�������٣�m/s��
//������������ٶȣ�sps= one shot per seconds������
//2006������ٱ�36��1
//1/60/36*8= 0.00370370f
//��1rpm/min�Ĳ����ٶ�ÿ�뷢��0.0037ke�ӵ�
#define  TRIG_RPM_TO_SPEED_SEN     						 0.0037037f//0.0001745f





//shoot��ص����Ϣ���ݰ�
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



//�����ܽṹ��
typedef struct 
{
    const rc_data_t *rc_data;																		//��ȡң����ָ��
		shoot_motor_t shoot_trig_motor;		
		trig_fire_mode_e trig_fire_mode;
		trig_fire_mode_e last_trig_fire_mode;
}shoot_control_t;


extern osTimerId ShootTimerHandle;
extern shoot_control_t trig_control;

#endif
