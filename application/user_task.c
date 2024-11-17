#include "user_task.h"
#include "remote_control.h"
#include "can.h"



gimbal_data_t gimbal_data;


void user_task(void const * argument)
{
	
	while(1)
	{

		osDelay(1);
	}
}







void get_rc_data(gimbal_data_t *gimbal_data,uint8_t *buf)
{
	gimbal_data->rc_data.vx_set 	= ((int16_t*)buf)[0];
	gimbal_data->rc_data.vy_set = ((int16_t*)buf)[1];
	gimbal_data->rc_data.wz_set 		= ((int16_t*)buf)[2];
	
	gimbal_data->rc_data.rc_sl 			= buf[6];
	gimbal_data->rc_data.rc_sr 			= buf[7];
}

void get_gimbal_data(gimbal_data_t *gimbal_data,uint8_t *buf)
{
	gimbal_data->gimbal_yaw = ((int16_t *)buf)[0];
	gimbal_data->gimbal_mode = ((int16_t *)buf)[1];
}


const gimbal_data_t * get_gimbal_data_point(void)
{
	return &gimbal_data;
}
