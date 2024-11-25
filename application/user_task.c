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







void get_gimbal_data(gimbal_data_t *gimbal_data,uint8_t *buf)
{
	gimbal_data->rc_data.vx_set 	= (buf[0]-66)*10;
	gimbal_data->rc_data.vy_set 	= (buf[1]-66)*10;
	gimbal_data->rc_data.wz_set 	= (buf[2]-66)*10;
	gimbal_data->rc_err 					= buf[3];
	gimbal_data->rc_data.rc_sl 		= buf[4];
	gimbal_data->rc_data.rc_sr 		= buf[5];
	gimbal_data->gimbal_yaw 			= buf[6]*57.2957795f*4.0f/200.0f;
	gimbal_data->gimbal_mode 			= buf[7];
}


bool_t rc_is_error(void)
{
		return gimbal_data.rc_err;
}

const gimbal_data_t * get_gimbal_data_point(void)
{
	return &gimbal_data;
}
