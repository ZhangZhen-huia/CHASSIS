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
/* --------------------------------------yaw��-------------------------------------------------*/

#define YAW_ANGLE_PID_KP                 0.3f
#define YAW_ANGLE_PID_KI                 0.0f
#define YAW_ANGLE_PID_KD                 0.0f
#define YAW_PID_MAX_OUT            1.80f //������ֵ
#define YAW_PID_MAX_IOUT           0.0f //����������



/* --------------------------------------����1-------------------------------------------------*/
//��������ٶȻ�
#define DRIVE_MOTOR1_SPEED_PID_KP                 30000.0f
#define DRIVE_MOTOR1_SPEED_PID_KI                 2.0f
#define DRIVE_MOTOR1_SPEED_PID_KD                 0.0f
#define DRIVE_MOTOR1_SPEED_PID_MAX_OUT            16384.0f //������ֵ
#define DRIVE_MOTOR1_SPEED_PID_MAX_IOUT           100.0f //����������




//�������ǶȻ�
#define COURSE_MOTOR1_ANGLE_PID_KP                 	4.0f//105
#define COURSE_MOTOR1_ANGLE_PID_KI                	0.10f
#define COURSE_MOTOR1_ANGLE_PID_KD                 	0.0f
#define COURSE_MOTOR1_ANGLE_PID_MAX_OUT							360.0f //�������Ƕ�
#define COURSE_MOTOR1_ANGLE_PID_MAX_IOUT						10.0f			



//�������ٶȻ�
#define COURSE_MOTOR1_SPEED_PID_KP                 	150.0f
#define COURSE_MOTOR1_SPEED_PID_KI                	0.0f
#define COURSE_MOTOR1_SPEED_PID_KD                 	0.0f
#define COURSE_MOTOR1_SPEED_PID_MAX_OUT							30000.0f//�������ٶ�   -30000  -   30000
#define COURSE_MOTOR1_SPEED_PID_MAX_IOUT						0.0f			



/* --------------------------------------����2-------------------------------------------------*/
//��������ٶȻ�
#define DRIVE_MOTOR2_SPEED_PID_KP                 30000.0f
#define DRIVE_MOTOR2_SPEED_PID_KI                 2.0f
#define DRIVE_MOTOR2_SPEED_PID_KD                 0.0f
#define DRIVE_MOTOR2_SPEED_PID_MAX_OUT            16384.0f //������ֵ
#define DRIVE_MOTOR2_SPEED_PID_MAX_IOUT           100.0f //����������



//�������ǶȻ�
#define COURSE_MOTOR2_ANGLE_PID_KP                 	3.0f//20
#define COURSE_MOTOR2_ANGLE_PID_KI                	0.10f
#define COURSE_MOTOR2_ANGLE_PID_KD                 	0.0f
#define COURSE_MOTOR2_ANGLE_PID_MAX_OUT							360.0f //�������Ƕ�
#define COURSE_MOTOR2_ANGLE_PID_MAX_IOUT						10.0f			



//�������ٶȻ�
#define COURSE_MOTOR2_SPEED_PID_KP                 	100.0f
#define COURSE_MOTOR2_SPEED_PID_KI                	0.0f
#define COURSE_MOTOR2_SPEED_PID_KD                 	0.0f
#define COURSE_MOTOR2_SPEED_PID_MAX_OUT							30000.0f//�������ٶ�   -30000  -   30000
#define COURSE_MOTOR2_SPEED_PID_MAX_IOUT						0.0f			

/* --------------------------------------����3-------------------------------------------------*/
//��������ٶȻ�
#define DRIVE_MOTOR3_SPEED_PID_KP                 30000.0f
#define DRIVE_MOTOR3_SPEED_PID_KI                 2.0f
#define DRIVE_MOTOR3_SPEED_PID_KD                 0.0f
#define DRIVE_MOTOR3_SPEED_PID_MAX_OUT            16384.0f //������ֵ
#define DRIVE_MOTOR3_SPEED_PID_MAX_IOUT           100.0f //����������



//�������ǶȻ�
#define COURSE_MOTOR3_ANGLE_PID_KP                 	3.0f//35
#define COURSE_MOTOR3_ANGLE_PID_KI                	0.10f
#define COURSE_MOTOR3_ANGLE_PID_KD                 	0.0f
#define COURSE_MOTOR3_ANGLE_PID_MAX_OUT							360.0f //�������Ƕ�
#define COURSE_MOTOR3_ANGLE_PID_MAX_IOUT						10.0f			



//�������ٶȻ�
#define COURSE_MOTOR3_SPEED_PID_KP                 	250.0f
#define COURSE_MOTOR3_SPEED_PID_KI                	0.0f
#define COURSE_MOTOR3_SPEED_PID_KD                 	0.0f
#define COURSE_MOTOR3_SPEED_PID_MAX_OUT							30000.0f//�������ٶ�   -30000  -   30000
#define COURSE_MOTOR3_SPEED_PID_MAX_IOUT						0.0f			



/* --------------------------------------����4-------------------------------------------------*/
//��������ٶȻ�
#define DRIVE_MOTOR4_SPEED_PID_KP                 30000.0f
#define DRIVE_MOTOR4_SPEED_PID_KI                 0.0f
#define DRIVE_MOTOR4_SPEED_PID_KD                 0.0f
#define DRIVE_MOTOR4_SPEED_PID_MAX_OUT            16384.0f //������ֵ
#define DRIVE_MOTOR4_SPEED_PID_MAX_IOUT           0.0f //����������



//�������ǶȻ�
#define COURSE_MOTOR4_ANGLE_PID_KP                 	3.0f//11
#define COURSE_MOTOR4_ANGLE_PID_KI                	0.10f
#define COURSE_MOTOR4_ANGLE_PID_KD                 	0.0f
#define COURSE_MOTOR4_ANGLE_PID_MAX_OUT							360.0f //�������Ƕ�
#define COURSE_MOTOR4_ANGLE_PID_MAX_IOUT						10.0f			



//�������ٶȻ�
#define COURSE_MOTOR4_SPEED_PID_KP                 	200.0f
#define COURSE_MOTOR4_SPEED_PID_KI                	0.0f
#define COURSE_MOTOR4_SPEED_PID_KD                 	0.0f
#define COURSE_MOTOR4_SPEED_PID_MAX_OUT							30000.0f//�������ٶ�   -30000  -   30000
#define COURSE_MOTOR4_SPEED_PID_MAX_IOUT						0.0f			





typedef enum
{
  CHASSIS_VECTOR_FOLLOW_GIMBAL_YAW,   //chassis will follow yaw gimbal motor relative angle.���̻������̨��ԽǶ�
  CHASSIS_VECTOR_FOLLOW_CHASSIS_YAW,  //chassis will have yaw angle(chassis_yaw) close-looped control.�����е��̽Ƕȿ��Ʊջ�
  CHASSIS_VECTOR_NO_FOLLOW_YAW,       //chassis will have rotation speed control. ��������ת�ٶȿ���
  CHASSIS_VECTOR_ZERO_FORCE,                 //control-current will be sent to CAN bus derectly.

} chassis_mode_e;



typedef struct
{
  const motor_measure_t *chassis_motor_measure;//������ݷ���ֵ
  fp32 accel;
  fp32 speed;
  fp32 speed_set;
  int16_t give_current;
} chassis_motor_t;



typedef struct
{
	const RC_ctrl_t *chassis_RC;               					//����ʹ�õ�ң����ָ��, the point to remote control
  chassis_motor_t motor_chassis[8];          					//chassis motor data.���̵������
	const bmi088_real_data_t *chassis_bmi088_data;     										//��ȡ�����ǽ������ŷ����ָ��
	const gimbal_data_t *get_gimbal_data;								//���̻����̨����
  pid_type_def chassis_drive_speed_pid[4];             //��������ٶ�pid
  pid_type_def chassis_course_angle_pid[4];            //�������Ƕ�pid
  pid_type_def chassis_course_speed_pid[4];						 //�������ٶ�pid	
	pid_type_def yaw_pid;

  first_order_filter_type_t chassis_cmd_slow_set_vx;  //һ�׵�ͨ�˲������趨ֵ
  first_order_filter_type_t chassis_cmd_slow_set_vy;  //һ�׵�ͨ�˲������趨ֵ
  first_order_filter_type_t chassis_cmd_slow_set_wz;  //һ�׵�ͨ�˲������趨ֵ
	
	fp32 course_angle[4];																//6020�Ƕ� 
  fp32 course_set_angle[4];       										//6020�����趨�Ƕ�
	fp32 course_set_last_angle[4];       								//6020�ϴ��趨�Ƕ�
  fp32 drive_set_speed[4]; 														//3508���ռ�������ٶ�	

	
  chassis_mode_e chassis_mode;               					//���̿���״̬��
  chassis_mode_e last_chassis_mode;          					//�����ϴο���״̬��
	int8_t drct;
	fp32 vx_set;																				// m/s
	fp32 vy_set;																				// m/s
	fp32 wz_set;																				// m/s
	fp32 wz_rad;																				// rad/s
	
	
	fp32 vx_max_speed;  //ǰ����������ٶ� ��λm/s
  fp32 vx_min_speed;  //���˷�������ٶ� ��λm/s
  fp32 vy_max_speed;  //��������ٶ� ��λm/s
  fp32 vy_min_speed;  //�ҷ�������ٶ� ��λm/s
  fp32 wz_max_speed;  //��������ٶ� ��λm/s
  fp32 wz_min_speed;  //�ҷ�������ٶ� ��λm/s
	
	fp32 chassis_yaw;   //�����Ǻ���̨������ӵ�yaw�Ƕ�
  fp32 chassis_pitch; //�����Ǻ���̨������ӵ�pitch�Ƕ�
  fp32 chassis_roll;  //�����Ǻ���̨������ӵ�roll�Ƕ�

	DebugData chassis_debug_data;

	
} chassis_move_t;


extern chassis_move_t chassis_move;//�����˶�����

/*******************************һ�ڵ�ͨ�˲�����************************/
#define CHASSIS_CONTROL_TIME 0.01f   //x��y�������β���
#define CHASSIS_CONTROL_TIME_W 0.01f  //z�������β���

//������һ�β���ռ��
#define CHASSIS_ACCEL_X_NUM 0.99f
#define CHASSIS_ACCEL_Y_NUM 0.99f
#define CHASSIS_ACCEL_W_NUM 0.99f

/*******************************��������*******************************/
#define R       MOTOR_DISTANCE_TO_CENTER
#define PI       3.1415926f

//�������ֵ����Լ���ֵ
#define HALF_ECD_RANGE  4096
#define ECD_RANGE       8191

//1:14���ٱ�
#define M3508_MOTOR_RPM_TO_VECTOR 	WHEEL_CIRCUMFERENCE/60.0f/14	
#define CHASSIS_MOTOR_RPM_TO_VECTOR_SEN M3508_MOTOR_RPM_TO_VECTOR


//�־�300mm 0.3m
#define TRACK_WIDTH							0.3f
//���300mm 0.3m
#define WHEEL_BASE							0.3f

//С���ݰ뾶 m ��������������ֱ�ӡ�1.414
#define MOTOR_DISTANCE_TO_CENTER 	WHEEL_BASE/2.0f*1.414f

//С�����ܳ�
#define SMALL_TOP_CIRCUMFERENCE	 	MOTOR_DISTANCE_TO_CENTER*2*3.1415926f			

//���Ӱ뾶  m 
#define WHEEL_HALF_SIZE 	0.054f

//�����ܳ�	m
#define WHEEL_CIRCUMFERENCE				WHEEL_HALF_SIZE*2*3.1415926f  	

//ǰ������ٶ�  3.597498/s   --8911
//									0						0
#define NORMAL_MAX_CHASSIS_SPEED_X 3.597498f
#define NORMAL_MAX_CHASSIS_SPEED_Y 3.597498f
#define NORMAL_MAX_CHASSIS_SPEED_W 3.597498f

//���ٱ�14��rpm: Ȧ/min
//ң����ǰ��ҡ�ˣ�max 660��ת���ɳ���ǰ���ٶȣ�m/s���ı���	
#define CHASSIS_VX_RC_SEN            0.0054507f

//ң��������ҡ�ˣ�max 660��ת���ɳ��������ٶȣ�m/s���ı���
#define CHASSIS_VY_RC_SEN 					 0.0054507f

//��������̨��ʱ�� ң������yawң�ˣ�max 660��ת���ɳ�����ת�ٶȵı���
#define CHASSIS_WZ_RC_SEN 					 0.0054507f


#define CHASSIS_TASK_INIT_TIME 1000
#define CHASSIS_CONTROL_TIME_MS 1000
#define CHASSIS_X_CHANNEL	2
#define CHASSIS_Y_CHANNEL 3
#define CHASSIS_W_CHANNEL 0

//y��
#define GIM_Y_ECD_1				8116.0f
#define GIM_Y_ECD_2				3213.0f
#define GIM_Y_ECD_3				7614.0f
#define GIM_Y_ECD_4				3843.0f


//ҡ������
#define CHASSIS_RC_DEADLINE 50


//ѡ�����״̬ ����ͨ����
#define CHASSIS_MODE_CHANNEL 0

//�������ֵת���ɽǶ�ֵ
#ifndef MOTOR_ECD_TO_RAD
#define MOTOR_ECD_TO_RAD 0.0439453125f //      2*  PI  /8192
#endif
/**
  * @brief          �������񣬼�� CHASSIS_CONTROL_TIME_MS 2ms
  * @param[in]      pvParameters: ��
  * @retval         none
  */
void chassis_task(void const * argument);
void chassis_rc_to_control_vector(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector);
const DebugData* get_chassis_PID_Debug(void);



#endif



