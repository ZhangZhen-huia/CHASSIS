#include "user_task.h"
#include "remote_control.h"
#include "can.h"


rc_data_t rc_data;

void user_task(void const * argument)
{
	
	while(1)
	{

		osDelay(1);
	}
}







void get_rc_data(rc_data_t *rc_data,uint8_t *buf)
{
	rc_data->vx_set 	= ((int16_t*)buf)[0];
	rc_data->vy_set = ((int16_t*)buf)[1];
	rc_data->wz_set 		= ((int16_t*)buf)[2];
	
	rc_data->rc_sl 			= buf[6];
	rc_data->rc_sr 			= buf[7];

	
	
}


