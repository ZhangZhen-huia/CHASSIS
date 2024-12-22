#ifndef _Can_Receice_H
#define _Can_Receice_H

#include "stm32f4xx_hal.h"
#include "can.h"

/*****************������ݻ��սṹ��********************/ 
typedef struct 
{
		uint16_t ecd; //ת�ӻ�е�Ƕ� 0-8191
    int16_t rpm; // ת��ת��ֵ r/min
    int16_t given_current; //��ǰ����ֵ -16384-16384
    uint8_t temperate; //Temp/��
    int16_t last_ecd; //��һ�εĻ�е�Ƕ�
}motor_measure_t;



typedef enum
{
/*----------can1����id----------*/
	
	/*---3508---*/
	CAN_CHASSIS_DRIVE_ALL_ID = 0x200,
	
	/*---6020---*/
	CAN_CHASSIS_COURSE_ALL_ID_I = 0x1FF,
	CAN_CHASSIS_COURSE_ALL_ID_U = 0x1FE,
	
	CAN_TRIG_CMD_ID = 0x200,
	
/*----------can1����id----------*/
	CAN_DRIVE_MOTOR1_ID = 0x201,
	CAN_DRIVE_MOTOR2_ID = 0x202,
	CAN_DRIVE_MOTOR3_ID = 0x203,
	CAN_DRIVE_MOTOR4_ID = 0x204,
	CAN_YAW_MOTOR_ID = 0x209,
/*----------can2����id----------*/
	CAN_COURSE_MOTOR1_ID = 0x205,
	CAN_COURSE_MOTOR2_ID = 0x206,
	CAN_COURSE_MOTOR3_ID = 0x207,
	CAN_COURSE_MOTOR4_ID = 0x208,
	TRIG_MOTOR_ID = 0x201,
	
	
/*----------can1˫��ͨ�Ž���id---------*/
	GIMBAL_ID = 0x111,
	CHASSIS_ID = 0x222,
} can_msg_id_e;



extern motor_measure_t yaw_motor;

void canfilter_init_start(void);
const motor_measure_t *get_gimbal_trigger_motor_measure_point(void);
void CAN_cmd_trig(int16_t current);

const motor_measure_t *get_chassis_drive_motor_measure_point(uint8_t ID);
const motor_measure_t *get_chassis_course_motor_measure_point(uint8_t ID);
void CAN_cmd_course(int16_t M1, int16_t M2, int16_t M3, int16_t M4);
void CAN_cmd_drive(int16_t M1, int16_t M2, int16_t M3, int16_t M4);

#endif
