#ifndef _Can_Receice_H
#define _Can_Receice_H

#include "stm32f4xx_hal.h"
#include "can.h"

/*****************电机数据回收结构体********************/ 
typedef struct 
{
		uint16_t ecd; //转子机械角度 0-8191
    int16_t rpm; // 转子转速值 r/min
    int16_t given_current; //当前电流值 -16384-16384
    uint8_t temperate; //Temp/度
    int16_t last_ecd; //上一次的机械角度
}motor_measure_t;

//超级电容数据结构体
typedef struct
{
   fp32      voltage;
	uint8_t   ouput_type;

}SuperPower_data_t;



////超级电容数据结构体
//typedef struct
//{
//   fp32      	voltage;
//	 fp32   		current_power;

//}SuperPower_data_t;


typedef enum
{
/*----------can1发送id----------*/
	
	/*---3508---*/
	CAN_CHASSIS_DRIVE_ALL_ID = 0x200,
	
	/*---6020---*/
	CAN_CHASSIS_COURSE_ALL_ID_I = 0x1FE,
	CAN_CHASSIS_COURSE_ALL_ID_U = 0x1FF,
	
	CAN_TRIG_CMD_ID = 0x1FF	,
	
/*----------can2id----------*/
	CAN_DRIVE_MOTOR1_ID = 0x201,
	CAN_DRIVE_MOTOR2_ID = 0x202,
	CAN_DRIVE_MOTOR3_ID = 0x203,
	CAN_DRIVE_MOTOR4_ID = 0x204,
	TRIG_MOTOR_ID = 0x205,
	SUPERPOWER_RX_ID	= 0x211,
	SUPERPOWER_TX_ID	= 0x210,
	
/*----------can1id----------*/
	CAN_COURSE_MOTOR1_ID = 0x205,
	CAN_COURSE_MOTOR2_ID = 0x206,
	CAN_COURSE_MOTOR3_ID = 0x207,
	CAN_COURSE_MOTOR4_ID = 0x208,
	CAN_YAW_MOTOR_ID = 0x209,
	GIMBAL_ID = 0x111,
	CHASSIS_ID = 0x222,
	
} can_msg_id_e;



extern motor_measure_t yaw_motor;
extern SuperPower_data_t SuperPower_data;

void canfilter_init_start(void);
const motor_measure_t *get_gimbal_trigger_motor_measure_point(void);
void CAN_cmd_trig(int16_t current);
void CAN_cmd_ChassisMode(uint8_t mode);
void CAN_cmd_SuperPower(uint8_t max_chassis_power, uint16_t power_buffer ,uint8_t mode, uint16_t voletage, uint16_t current );

const motor_measure_t *get_chassis_drive_motor_measure_point(uint8_t ID);
const motor_measure_t *get_chassis_course_motor_measure_point(uint8_t ID);
void CAN_cmd_course(int16_t M1, int16_t M2, int16_t M3, int16_t M4);
void CAN_cmd_drive(int16_t M1, int16_t M2, int16_t M3, int16_t M4);

#endif
