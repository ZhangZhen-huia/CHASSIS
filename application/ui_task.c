#include "ui_task.h"
#include "ui.h"
#include "chassis_task.h"
#include "trig_task.h"
#include "communicate_task.h"
#include "key_task.h"
#include "chassis_power_control.h"
#include "referee.h"
#include "Can_receive.h"


void ChassisMode_Update(void);
void FricMode_Update(void);
void SuperPowerState_Update(void);
static void EnemyColor_Update(void);
static void CapBuffer_Update(void);
static void TrigMode_Update(void);
static void Rotate_Update(uint8_t i);
void ui_init(void);

uint8_t i=0;
void ui_task(void const * argument)
{
	osDelay(1000);
	ui_init();

	while(1)
	{		

		ChassisMode_Update();
		FricMode_Update();
		SuperPowerState_Update();
		EnemyColor_Update();
		CapBuffer_Update();
		TrigMode_Update();
		if(chassis_move.chassis_mode == CHASSIS_VECTOR_TOP_FOLLOW_GIMBAL_YAW)
		{
			Rotate_Update(i);
			i++;
			i%=8;
		}
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
		ui_default_Mode_FricMode->start_x = 25;
    ui_default_Mode_FricMode->start_y = 790;
				ui_default_Mode_FricMode->color = 8;
		ui_default_Mode_FricMode->font_size = 20;
		strcpy(ui_default_Mode_FricMode->string,"FRIC:1");
		mode = 0;
	}
	else
	{
		ui_default_Mode_FricMode->start_x = 735;
    ui_default_Mode_FricMode->start_y = 790;
		ui_default_Mode_FricMode->color = 5;
		ui_default_Mode_FricMode->font_size = 80;
		strcpy(ui_default_Mode_FricMode->string,"FRIC:0");
		mode = 1;
	}
	
	if(mode != last_mode)
		_ui_update_default_Mode_1();
		
}

void SuperPowerState_Update(void)
{
	static uint8_t state,last_state;
	last_state = state;
	if(SuperPowerState == OPEN)
	{
		ui_default_Mode_SuperPower->font_size = 80;
    ui_default_Mode_SuperPower->start_x = 725;
    ui_default_Mode_SuperPower->start_y = 700;
    ui_default_Mode_SuperPower->color = 6;
		strcpy(ui_default_Mode_SuperPower->string,"CAP:OPEN");

		state = 0;
	}
	else
	{
		ui_default_Mode_SuperPower->font_size = 20;
    ui_default_Mode_SuperPower->start_x = 25;
    ui_default_Mode_SuperPower->start_y = 750;
    ui_default_Mode_SuperPower->color = 8;
		strcpy(ui_default_Mode_SuperPower->string,"CAP:CLOSE");
		state = 1;
	}
	
	if(state != last_state)
		_ui_update_default_Mode_2();
		
}

static void EnemyColor_Update(void)
{
	static uint8_t state,last_state;
	last_state = state;
	if(gimbal_data.EnemyColor == BLUE)
	{
		strcpy(ui_default_AIMBOT_EnemyColor->string,"Enemy:BLUE");
		state = 0;
	}
	else
	{
		strcpy(ui_default_AIMBOT_EnemyColor->string,"Enemy:RED ");
		state = 1;
	}
	
	if(state != last_state)
		_ui_update_default_AIMBOT_0();
}

static void CapBuffer_Update(void)
{
	ui_default_SuperPower_CapBuffer->number = SuperPower_data.voltage*1000;
	
	_ui_update_default_SuperPower_1();
}


static void Rotate_Update(uint8_t i)
{
	Armour_Func(i);
	_ui_update_default_Armour_0();
}

static void TrigMode_Update(void)
{
		static uint8_t state,last_state;
	last_state = state;
	if(trig_control.trig_fire_mode == Single_fire)
	{
		strcpy(ui_default_ShootMode_ShootMode->string, "Single");
		state = 0;
	}
	else
	{
		strcpy(ui_default_ShootMode_ShootMode->string, "Serial");
		state = 1;
	}
	
	if(state != last_state)
		_ui_update_default_ShootMode_0();
}
void ui_init(void)
{
	ui_self_id = Referee_System.ext_game_robot_state.robot_id;
	ui_init_default_Mode();

}