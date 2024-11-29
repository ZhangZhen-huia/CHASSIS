#include "user_task.h"
#include "remote_control.h"
#include "can.h"
#include "user_lib.h"


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
	gimbal_data->rc_data.vx_set 	= (buf[0]-33)*20;
	gimbal_data->rc_data.vy_set 	= (buf[1]-33)*20;
	gimbal_data->rc_data.wz_set 	= (buf[2]-33)*20;
	gimbal_data->rc_err 					= buf[3];
	gimbal_data->rc_data.rc_sl 		= buf[4];
	gimbal_data->rc_data.rc_sr 		= buf[5];
	gimbal_data->gimbal_yaw 			= theta_format(buf[6]*255.0f-180);///100.0f*4.0f-3.14f)*57.2957795f;//*57.2957795f*4.0f/200.0f;
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


const rc_data_t * get_gimbal_rc_data_point(void)
{
	return &gimbal_data.rc_data;
}
