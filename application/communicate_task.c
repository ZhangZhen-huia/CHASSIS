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
#include "key_task.h"

gimbal_data_t gimbal_data;

static void dispose_gimbal_mode(gimbal_data_t *gimbal_data);
static void SuperPower_Control(void);
static void ChassisModeTransfer(void);


void communicate_task(void const * argument)
{
	  

	while(1)
	{
		dispose_gimbal_mode(&gimbal_data);
		SuperPower_Control();
		ChassisModeTransfer();

		osDelay(10);
	}
}



void get_gimbal_data(gimbal_data_t *gimbal_data,uint8_t *buf)
{
	
	gimbal_data->gimbal_mode 			= buf[5];
	gimbal_data->rc_data.vx_set 	= (buf[0]-66)*20;
	gimbal_data->rc_data.vy_set 	= (buf[1]-66)*20;
	gimbal_data->rc_data.wz_set		= 0;
	gimbal_data->rc_err 					= buf[2];
	
	if(gimbal_data->rc_err & 0x01)
	{
		gimbal_data->ControlMode = Rc;
		gimbal_data->rc_data.rc_sl 		= buf[3];
		gimbal_data->rc_data.rc_sr 		= buf[4];
	}
	else if(gimbal_data->rc_err & 0x02)
	{
		gimbal_data->ControlMode = ImageTransfer;
		memcpy(&gimbal_data->ImghandleKey,&buf[3],2);
	}


	gimbal_data->rc_data.rc_key_v_last = gimbal_data->rc_data.rc_key_v;
	memcpy(&gimbal_data->rc_data.rc_key_v,&buf[6],2);
}


static void dispose_gimbal_mode(gimbal_data_t *gimbal_data)
{
	gimbal_data->FireFlag = gimbal_data->gimbal_mode & 0x01;
	gimbal_data->GimbalInit = gimbal_data->gimbal_mode & 0x02;
	gimbal_data->FricState = gimbal_data->gimbal_mode & 0x04;
	
	if(gimbal_data->gimbal_mode & 0x08)
		gimbal_data->EnemyColor = RED;
	else if(gimbal_data->gimbal_mode & 0x10)
		gimbal_data->EnemyColor = BLUE;
	
	
	if(gimbal_data->rc_err & 0x04)
		gimbal_data->Toe_is_errRc = 1;
	else 
		gimbal_data->Toe_is_errRc = 0;		
	
	if(gimbal_data->rc_err & 0x08)
		gimbal_data->Toe_is_errImageTransfer = 1;
	else 
		gimbal_data->Toe_is_errImageTransfer = 0;		
	
	if(gimbal_data->gimbal_mode & 0x20)
		gimbal_data->IS_AIMBOT = 1;
	else
		gimbal_data->IS_AIMBOT = 0;
	

}

static void SuperPower_Control(void)
{
	uint8_t PowerMode = 2;

	
	CAN_cmd_SuperPower(	Referee_System.ext_game_robot_state.chassis_power_limit,
											Referee_System.ext_power_heat_data.chassis_power_buffer,
											PowerMode,
											Referee_System.ext_power_heat_data.chassis_volt,
											Referee_System.ext_power_heat_data.chassis_current);
}


extern uint8_t Dir;
uint8_t RotateGrade=0;
static void ChassisModeTransfer(void)
{
	uint8_t Mode;
	if(chassis_move.chassis_mode == CHASSIS_VECTOR_TOP_FOLLOW_GIMBAL_YAW)
	{
		Mode |= 0x01;
	}
	else
	{
		Mode &= 0xFE;
	}
	if(Dir == 1)
		Mode |= 0x02;
	else
		Mode &= 0xFD;
	
	if(RotateGrade)
		Mode |= 0x04;
	else
		Mode &= 0xFB;//1111 1011
	
	
	CAN_cmd_ChassisMode(Mode);
}


const gimbal_data_t * get_gimbal_data_point(void)
{
	return &gimbal_data;
}


const rc_data_t * get_gimbal_rc_data_point(void)
{
	return &gimbal_data.rc_data;
}



