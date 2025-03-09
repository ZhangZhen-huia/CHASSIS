#ifndef TRIG_TASK_H
#define TRIG_TASK_H



#include "main.h"
#include "pid.h"
#include "Can_receive.h"
#include "remote_control.h"
#include "communicate_task.h"
#include "referee.h"

//�����ʼ��ʱ��
#define TRIG_TASK_INIT_TIME    500
#define TRIG_TASK_CONTROL_TIME 500


//����
#define TRIG_SPEED_PID_KP_CASCADE           2000.0f
#define TRIG_SPEED_PID_KI_CASCADE           0.0f
#define TRIG_SPEED_PID_KD_CASCADE           20.0f
#define TRIG_SPEED_PID_MAX_OUT_CASCADE     10000.0f
#define TRIG_SPEED_PID_MAX_IOUT_CASCADE    0.0f

#define TRIG_ANGLE_PID_KP_CASCADE           20.0f
#define TRIG_ANGLE_PID_KI_CASCADE           0.2f
#define TRIG_ANGLE_PID_KD_CASCADE           2.0f
#define TRIG_ANGLE_PID_MAX_OUT_CASCADE     10000.0f   //���10000rpm�е����
#define TRIG_ANGLE_PID_MAX_IOUT_CASCADE    0.0f


//����
#define TRIG_SPEED_PID_KP_SINGLE           2150.0f
#define TRIG_SPEED_PID_KI_SINGLE           110.0f
#define TRIG_SPEED_PID_KD_SINGLE           0.0f
#define TRIG_SPEED_PID_MAX_OUT_SINGLE     10000.0f
#define TRIG_SPEED_PID_MAX_IOUT_SINGLE    1000.0f


//�����������������̲����ٶ� ��/s
#define TRIG_BASE_SPEED 30.0f

//���ձ���ģʽ,ƴ���򵯣��ܴ���������Ƕ���
#define TRIG_EXPLOSION_SPEED 					30.0f

#define TRIG_BACK_SPEED		20.0f
//����Ĭ�Ͽ���ת��(rpm)  
#define  STANDARD_NOMOVE_RPM          15
//����Ĭ�Ͽ���ʱ�� 
#define  STANDARD_NOMOVE_TIME         250
//����Ĭ�ϻ�תʱ��
#define  STANDARD_REMOVE_TIME         200
//һ������Ƕ�
#define PI_TEN 0.31415926f

#define TIRG_MODE_CHANNEL 1



//���岦����ת�٣�rpm��ת��Ϊ�������٣�m/s��
//������������ٶȣ�sps= one shot per seconds������
//2006������ٱ�36��1
//1/60/36*10= 0.004629629f
//��1rpm/min�Ĳ����ٶ�ÿ�뷢��0.0037ke�ӵ�
#define  TRIG_RPM_TO_SPEED_SEN     						 0.004629629f//0.0001745f





//shoot��ص����Ϣ���ݰ�
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



//�����ܽṹ��
typedef struct 
{
    const rc_data_t *rc_data;																		//��ȡң����ָ��
		shoot_motor_t shoot_trig_motor;	
		trig_mode_e trig_fire_mode;
		Referee_System_t     *trig_referee;   //�����������ϵͳ����
		uint8_t Fire;
		uint8_t ultimate_explosion;
}shoot_control_t;



extern osTimerId ShootTimerHandle;
extern shoot_control_t trig_control;

#endif
