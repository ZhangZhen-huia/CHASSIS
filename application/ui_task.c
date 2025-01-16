#include "ui_task.h"
#include "ui.h"
#include "chassis_task.h"
#include "trig_task.h"
#include "communicate_task.h"
#include "key_task.h"
#include "chassis_power_control.h"
#include "referee.h"



void ChassisMode_Update(void);
void FricMode_Update(void);
void SuperPowerState_Update(void);
void ui_init(void);

void ui_task(void const * argument)
{
	osDelay(1000);
	ui_init();

	while(1)
	{		
		ChassisMode_Update();
		FricMode_Update();
		SuperPowerState_Update();
		
		if(gimbal_data.rc_data.rc_key_v & KEY_PRESSED_OFFSET_B)
			ui_init();
		osDelay(100);
	}
	
}

void ChassisMode_Update(void)
{
	static uint8_t mode,last_mode;
	last_mode = mode;
	if(chassis_move.chassis_mode == CHASSIS_VECTOR_DIRECTION_FOLLOW_GIMBAL_YAW)
	{
		strcpy(ui_default_Mode_ChassisMode->string,"CHASSIS:NORMAL");
		mode = 0;
	}
	else	if(chassis_move.chassis_mode == CHASSIS_VECTOR_TOP_FOLLOW_GIMBAL_YAW)
	{
		strcpy(ui_default_Mode_ChassisMode->string,"CHASSIS:ROTATE");
		mode = 1;
	}
	else	if(chassis_move.chassis_mode == CHASSIS_VECTOR_FLY)
	{
		strcpy(ui_default_Mode_ChassisMode->string,"CHASSIS:FLYING");
		mode = 2;
	}
	
	if(mode != last_mode)
		_ui_update_default_Mode_0();
		
}

void FricMode_Update(void)
{
	static uint8_t mode,last_mode;
	last_mode = mode;
	if(gimbal_data.FricState)
	{
		strcpy(ui_default_Mode_FricMode->string,"FRIC:ON ");
		mode = 0;
	}
	else
	{
		strcpy(ui_default_Mode_FricMode->string,"FRIC:OFF");
		mode = 1;
	}
	
	if(mode != last_mode)
		_ui_update_default_Mode_1();
		
}

void SuperPowerState_Update(void)
{
	static uint8_t state,last_state;
	last_state = state;
	if(SuperPowerData.Cap_State == OPEN)
	{
		strcpy(ui_default_Mode_SuperPower->string,"CAP:OPEN ");
		state = 0;
	}
	else
	{
		strcpy(ui_default_Mode_SuperPower->string,"CAP:CLOSE");
		state = 1;
	}
	
	if(state != last_state)
		_ui_update_default_Mode_2();
		
}

void ui_init(void)
{
	ui_self_id = Referee_System.ext_game_robot_state.robot_id;
	ui_init_default_Mode();

}