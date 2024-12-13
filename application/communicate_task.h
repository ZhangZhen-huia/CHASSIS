#ifndef COMMUNICATE_TASK_H
#define COMMUNICATE_TASK_H


#include "main.h"

#define USART_RX_BUF_LENGHT     512
#define REFEREE_FIFO_BUF_LENGTH 1024


typedef struct
{
	int16_t vx_set;
	int16_t vy_set;
	int16_t wz_set;
	uint8_t rc_sl;
	uint8_t rc_sr;
	
}rc_data_t;



typedef struct
{
	fp32 gimbal_yaw;
	int16_t gimbal_mode;
	rc_data_t rc_data;
	uint8_t rc_err;
	uint8_t FireFlag;
//	uint8_t Radar_mode;
//	uint8_t Build_map_mode;
	uint8_t Gimbal_init;
}gimbal_data_t;



void get_gimbal_data(gimbal_data_t *gimbal_data,uint8_t *buf);

bool_t rc_is_error(void);




extern gimbal_data_t gimbal_data;
const gimbal_data_t * get_gimbal_data_point(void);
const rc_data_t * get_gimbal_rc_data_point(void);
const uint8_t * get_autofire_flag_point(void);

void Chassis_data_transfer(void);



#endif


