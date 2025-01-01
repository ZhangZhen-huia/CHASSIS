#include "communicate_task.h"
#include "remote_control.h"
#include "can.h"
#include "user_lib.h"
#include "chassis_task.h"
#include "CRC8_CRC16.h"
#include "fifo.h"
#include "referee.h"
#include "bsp_usart.h"

gimbal_data_t gimbal_data;
void dispose_gimbal_mode(gimbal_data_t *gimbal_data);


void communicate_task(void const * argument)
{
	  

	while(1)
	{
		dispose_gimbal_mode(&gimbal_data);
		
		osDelay(10);
	}
}





//int speed[3];

void get_gimbal_data(gimbal_data_t *gimbal_data,uint8_t *buf)
{
	gimbal_data->gimbal_mode 			= buf[6];
	if(chassis_move.chassis_mode == CHASSIS_VECTOR_RADAR)
	{
		#ifdef RADAR
//		speed[0]=buf[0];
//		speed[1]=buf[1];
//		speed[2]=buf[2];

		
		gimbal_data->rc_data.vx_set 	= buf[0]*0.02-2.0f;//(float)buf[0]/100.0f - 1.0f;
		gimbal_data->rc_data.vy_set 	= buf[1]*0.02-2.0f;//(float)buf[1]/100.0f - 1.0f;
		gimbal_data->rc_data.wz_set 	= buf[2]*0.02-2.0f;//(float)buf[2]/100.0f - 1.0f;
		#else
		gimbal_data->rc_data.vx_set 	= 0.0f;
		gimbal_data->rc_data.vy_set 	= 0.0f;
		gimbal_data->rc_data.wz_set 	= 0.0f;
		#endif
	}
	else//底盘不是雷达模式
	{
		//并且云台也是雷达模式
		if(gimbal_data->gimbal_mode & 0x04)
		{
			gimbal_data->rc_data.vx_set 	= 0;
			gimbal_data->rc_data.vy_set 	= 0;
			gimbal_data->rc_data.wz_set 	= 0;

		}
		//云台不是雷达模式
		else
		{
			gimbal_data->rc_data.vx_set 	= (buf[0]-33)*20;
			gimbal_data->rc_data.vy_set 	= (buf[1]-33)*20;
			gimbal_data->rc_data.wz_set 	= (buf[2]-33)*20;

		}

	}
	gimbal_data->rc_err 					= buf[3];
	gimbal_data->rc_data.rc_sl 		= buf[4];
	gimbal_data->rc_data.rc_sr 		= buf[5];
}


void dispose_gimbal_mode(gimbal_data_t *gimbal_data)
{
	gimbal_data->FireFlag = gimbal_data->gimbal_mode & 0x01;
//	gimbal_data->Radar_mode = gimbal_data->gimbal_mode & 0x04;
//	gimbal_data->Build_map_mode = gimbal_data->gimbal_mode & 0x08;
	gimbal_data->Gimbal_init = gimbal_data->gimbal_mode & 0x02;
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



