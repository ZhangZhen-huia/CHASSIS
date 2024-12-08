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
static void can_cmd_to_gimbal(CAN_HandleTypeDef*hcan,int16_t can_id,uint8_t *buf,uint8_t num);
void dispose_gimbal_mode(gimbal_data_t *gimbal_data);


void communicate_task(void const * argument)
{
	  

	while(1)
	{
		dispose_gimbal_mode(&gimbal_data);
		osDelay(10);
	}
}






void Chassis_data_transfer(void)
{

	uint8_t buf[1];
	fp32 pitch;
	
	pitch = chassis_move.chassis_bmi088_data->INS_angle[INS_ROLL_ADDRESS_OFFSET]/57.2957795f;
	
	buf[0] = (pitch+3.1415926f)*40.0f;
	
	can_cmd_to_gimbal(&hcan1,CHASSIS_ID,buf,1);
}

void get_gimbal_data(gimbal_data_t *gimbal_data,uint8_t *buf)
{
	gimbal_data->rc_data.vx_set 	= (buf[0]-33)*20;
	gimbal_data->rc_data.vy_set 	= (buf[1]-33)*20;
	gimbal_data->rc_data.wz_set 	= (buf[2]-33)*20;
	gimbal_data->rc_err 					= buf[3];
	gimbal_data->rc_data.rc_sl 		= buf[4];
	gimbal_data->rc_data.rc_sr 		= buf[5];
	gimbal_data->gimbal_mode 			= buf[6];
}


void dispose_gimbal_mode(gimbal_data_t *gimbal_data)
{
	gimbal_data->FireFlag = gimbal_data->gimbal_mode & 0x01;
	gimbal_data->Radar_mode = gimbal_data->gimbal_mode & 0x04;
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


static void can_cmd_to_gimbal(CAN_HandleTypeDef*hcan,int16_t can_id,uint8_t *buf,uint8_t num)
{
	uint32_t send_mail_box;
	CAN_TxHeaderTypeDef chassis_tx_message;
	uint8_t    chassis_can_send_data[num];
	
	chassis_tx_message.StdId = can_id;
	chassis_tx_message.DLC = num;
	chassis_tx_message.IDE = CAN_ID_STD;
	chassis_tx_message.RTR = CAN_RTR_DATA;
	for(uint8_t i=0;i<num;i++)
	chassis_can_send_data[i] = buf[i];



	HAL_CAN_AddTxMessage(hcan,&chassis_tx_message,chassis_can_send_data, &send_mail_box);

}

