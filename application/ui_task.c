#include "ui_task.h"
#include "ui.h"
#include "chassis_task.h"
#include "trig_task.h"
#include "communicate_task.h"
#include "key_task.h"
#include "chassis_power_control.h"
#include "referee.h"
#include "Can_receive.h"


void FricMode_Update(void);
static void EnemyColor_Update(void);
static void CapBuffer_Update(void);
//static void TrigMode_Update(void);
static void ChassisStatus_Update(void);
static void ControlMode_Update(void);

void ui_init(void);


void zero_to_one_loop(int16_t *angle)
{
  *angle =(*angle > 360) ? *angle - 360 : *angle;
	*angle =(*angle < 0) ? *angle + 360 : *angle ;
}

uint8_t i=0;
void ui_task(void const * argument)
{
	osDelay(1000);
	ui_init();

	while(1)
	{		

		FricMode_Update();
		EnemyColor_Update();
		CapBuffer_Update();
		ChassisStatus_Update();
//		TrigMode_Update();
		ControlMode_Update();
		if(gimbal_data.rc_data.rc_key_v & KEY_PRESSED_OFFSET_B)
			ui_init();
		osDelay(30);
	}
	
}



void FricMode_Update(void)
{
	static uint8_t mode,last_mode,state,last_state,mode1,last_mode1;
	last_mode = mode;
	last_state = state;
	last_mode1 = mode1;
	if(gimbal_data.FricState && Referee_System.ext_game_robot_state.power_management_shooter_output == 1)
	{
		ui_default_Shoot_FricStatusR->rx = 300;
		ui_default_Shoot_FricStatusR->ry = 300;
																			
		ui_default_Shoot_FricStatusL->rx = 300;
		ui_default_Shoot_FricStatusL->ry = 300;

		mode = 0;
	}
	else if(Referee_System.ext_game_robot_state.power_management_shooter_output == 0 || gimbal_data.FricState == 0)
	{
		ui_default_Shoot_FricStatusR->rx = 0;
		ui_default_Shoot_FricStatusR->ry = 0;
		
		ui_default_Shoot_FricStatusL->rx = 0;
		ui_default_Shoot_FricStatusL->ry = 0;
		mode = 1;
	}
		
	if(gimbal_data.IS_AIMBOT)
	{
		mode1 = 0;
		ui_default_Shoot_FricStatusR->color = 0;
		ui_default_Shoot_FricStatusL->color = 0;
	}
	else
	{
		mode1 = 1;
		ui_default_Shoot_FricStatusR->color = 8;
		ui_default_Shoot_FricStatusL->color = 8;
	}
	if(chassis_move.chassis_mode == CHASSIS_VECTOR_TOP_FOLLOW_GIMBAL_YAW)
	{
		ui_default_Shoot_TrigStatusL->color = 2;
		ui_default_Shoot_TrigStatusR->color = 2;
		state = 0;
	}
	else
	{
		ui_default_Shoot_TrigStatusL->color = 7;
		ui_default_Shoot_TrigStatusR->color = 7;
		state = 1;
	}
	
	if((mode != last_mode) || (state != last_state) || (mode1 != last_mode1))
	_ui_update_default_Shoot_0();
		
}

static void EnemyColor_Update(void)
{
	if(gimbal_data.EnemyColor == BLUE)
	{
		ui_default_AIMBOT_EnemyColor->color = 6;
	}
	else
	{
		ui_default_AIMBOT_EnemyColor->color = 5;
	}
	
	ui_default_AIMBOT_EnemyColor->end_y = ui_default_AIMBOT_EnemyColor->start_y + FricSpeed*5.0f;
		_ui_update_default_AIMBOT_0();
}

static void CapBuffer_Update(void)
{
	if(SuperPowerState == OPEN)
	{	
		ui_default_CAP_Buffer->color = 1;
	}
	else
	{
		ui_default_CAP_Buffer->color = 8;
	}
	ui_default_CAP_Buffer->end_x = ((SuperPower_data.voltage-4.5f)/10.7f)*712 + ui_default_CAP_Buffer->start_x;
	_ui_update_default_CAP_0();
}


extern fp32 yaw_diff;
static void ChassisStatus_Update(void)
{
	static int16_t start_angle;
	static int16_t end_angle;
	
	start_angle = (int16_t)(330 - yaw_diff);
	zero_to_one_loop(&start_angle);
	ui_default_ChassisStatus_Rotate->start_angle = start_angle;

	end_angle = (int16_t)(30 - yaw_diff);
	zero_to_one_loop(&end_angle);
	ui_default_ChassisStatus_Rotate ->end_angle =end_angle;

	if(chassis_move.chassis_mode == CHASSIS_VECTOR_FLY)
	{
		ui_default_ChassisStatus_ChassisRound->color = 5;
		ui_default_ChassisStatus_ChassisRound->width = 15;
	}
	else
	{
		ui_default_ChassisStatus_ChassisRound->color = 7;
		ui_default_ChassisStatus_ChassisRound->width = 3;
	}

	_ui_update_default_ChassisStatus_0();
}

static void ControlMode_Update(void)
{
	static uint8_t state,last_state;
	last_state = state;
	
	if(gimbal_data.ControlMode == Rc)
	{
		ui_default_Control_ControlMode->width = 100;
		state = 1;
	}
	else if(gimbal_data.ControlMode == ImageTransfer)
	{
		state = 0;
		ui_default_Control_ControlMode->width = 0;
	}
		
	
	if(state != last_state)
	_ui_update_default_Control_0();
}


	
	
void ui_init(void)
{
	ui_self_id = Referee_System.ext_game_robot_state.robot_id;
	ui_init_default_Mode();

}