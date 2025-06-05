#ifndef __CHASSIA_BEHAVIOR__H
#define __CHASSIA_BEHAVIOR__H

#include "main.h"

#include "struct_typedef.h"

#include "chassis_task.h"





typedef enum
{
  CHASSIS_ZERO_FORCE,                   //chassis will be like no power,底盘无力, 跟没上电那样
  CHASSIS_DIRECTION_FOLLOW_GIMBAL_YAW,   //chassis will follow gimbal, usually in infantry,正常步兵底盘跟随云台
	CHASSIS_TOP_FOLLOW_GIMBAL_YAW,				//底盘跟随底盘yaw
  CHASSIS_ALONE,                        //底盘不跟随yaw 
	CHASSIS_FLY														//飞坡
} chassis_behaviour_e;



void chassis_behaviour_mode_set(chassis_move_t *chassis_move_mode);
void chassis_behaviour_control_set(fp32 *vx_set, fp32 *vy_set, fp32 *angle_set, chassis_move_t *chassis_move_rc_to_vector);





#endif
