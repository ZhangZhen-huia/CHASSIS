#include "communicate_task.h"
#include "remote_control.h"
#include "can.h"
#include "user_lib.h"
#include "chassis_task.h"
#include "CRC8_CRC16.h"
#include "fifo.h"
#include "referee.h"
#include "bsp_usart.h"
#include "Can_receive.h"
#include "referee.h"

gimbal_data_t gimbal_data;

void dispose_gimbal_mode(gimbal_data_t *gimbal_data);
void Can_CmdTo_Gimbal(void);


void communicate_task(void const * argument)
{
	  

	while(1)
	{
		dispose_gimbal_mode(&gimbal_data);
//		Can_CmdTo_Gimbal();
//		CAN_cmd_shootdata(Referee_System.ext_shoot_data.initial_speed,Referee_System.ext_power_heat_data.shooter_id1_17mm_cooling_heat);
		osDelay(10);
	}
}





//int speed[3];

void get_gimbal_data(gimbal_data_t *gimbal_data,uint8_t *buf)
{
	
	gimbal_data->rc_data.vx_set 	= (buf[0]-33)*20;
	gimbal_data->rc_data.vy_set 	= (buf[1]-33)*20;
	gimbal_data->rc_data.wz_set		= 0;
	//gimbal_data->rc_data.wz_set 	= (buf[2]-33)*20;

	gimbal_data->rc_err 					= buf[2];
	gimbal_data->rc_data.rc_sl 		= buf[3];
	gimbal_data->rc_data.rc_sr 		= buf[4];
	gimbal_data->gimbal_mode 			= buf[5];
	gimbal_data->rc_data.rc_key_v_last = gimbal_data->rc_data.rc_key_v;
	memcpy(&gimbal_data->rc_data.rc_key_v,&buf[6],2);
}


void dispose_gimbal_mode(gimbal_data_t *gimbal_data)
{
	gimbal_data->FireFlag = gimbal_data->gimbal_mode & 0x01;
	gimbal_data->AimBot = gimbal_data->gimbal_mode & 0x02;
	gimbal_data->FricState = gimbal_data->gimbal_mode & 0x04;
	
	if(gimbal_data->rc_err & 0x01)
		gimbal_data->ControlMode = Rc;
	else if(gimbal_data->rc_err & 0x02)
		gimbal_data->ControlMode = ImageTransfer;
	
	if(gimbal_data->rc_err & 0x04)
		gimbal_data->Toe_is_errRc = 1;
	else 
		gimbal_data->Toe_is_errRc = 0;		
	
	if(gimbal_data->rc_err & 0x08)
		gimbal_data->Toe_is_errImageTransfer = 1;
	else 
		gimbal_data->Toe_is_errImageTransfer = 0;		
	
}

void SuperPower_Control(SuperPower_data_t *control)
{
	
}

void Can_CmdTo_Gimbal(void)
{
	uint32_t send_mail_box;
	CAN_TxHeaderTypeDef chassis_tx_message;
	static uint8_t    chassis_can_send_data[1];
	
	chassis_tx_message.StdId = CHASSIS_ID;
	chassis_tx_message.DLC   = 0x01;
	chassis_tx_message.RTR	 = CAN_RTR_DATA;
	chassis_tx_message.IDE 	 = CAN_ID_STD;
	
	chassis_can_send_data[0]=(uint8_t)(Referee_System.ext_shoot_data.initial_speed);
//	chassis_can_send_data[1]=(uint16_t)(Referee_System.ext_shoot_data.initial_speed);
	
//	chassis_can_send_data[2]=M2>>8;
//	chassis_can_send_data[3]=M2;
//	
//	chassis_can_send_data[4]=M3>>8;
//	chassis_can_send_data[5]=M3;
//	
//	chassis_can_send_data[6]=M4>>8;
//	chassis_can_send_data[7]=M4;
	
	HAL_CAN_AddTxMessage(&hcan1,&chassis_tx_message,chassis_can_send_data,&send_mail_box);
}



const gimbal_data_t * get_gimbal_data_point(void)
{
	return &gimbal_data;
}


const rc_data_t * get_gimbal_rc_data_point(void)
{
	return &gimbal_data.rc_data;
}



