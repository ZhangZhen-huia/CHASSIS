#ifndef USER_TASK_H
#define USER_TASK_H


#include "main.h"


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
}gimbal_data_t;




void get_gimbal_data(gimbal_data_t *gimbal_data,uint8_t *buf);

bool_t rc_is_error(void);




extern gimbal_data_t gimbal_data;
const gimbal_data_t * get_gimbal_data_point(void);
const rc_data_t * get_gimbal_rc_data_point(void);




#endif


