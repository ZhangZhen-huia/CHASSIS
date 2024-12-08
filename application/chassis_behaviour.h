#ifndef __CHASSIA_BEHAVIOR__H
#define __CHASSIA_BEHAVIOR__H

#include "main.h"

#include "struct_typedef.h"

#include "chassis_task.h"

#define MAX_SPEED   			 1.84f
#define HALF_MAX_SPEED   	 MAX_SPEED*0.5f
#define QUTR_MAX_SPEED    	HALF_MAX_SPEED*0.5f



typedef enum
{
  CHASSIS_ZERO_FORCE,                   //chassis will be like no power,底盘无力, 跟没上电那样
  CHASSIS_NO_MOVE,                      //chassis will be stop,底盘保持不动
  CHASSIS_INFANTRY_FOLLOW_GIMBAL_YAW,   //chassis will follow gimbal, usually in infantry,正常步兵底盘跟随云台
  CHASSIS_NO_FOLLOW_YAW,                //chassis does not follow angle, angle is open-loop,but wheels have closed-loop speed
                                        //底盘不跟随角度，角度是开环的，但轮子是有速度环
	CHASSIS_FOLLOW_RADAR,									//底盘跟随雷达数据
	CHASSIS_AGV_FOLLOW_CHASSIS_YAW,				//底盘跟随底盘yaw
  CHASSIS_OPEN                          //the value of remote control will mulitiply a value, get current value that will be sent to can bus
                                        // 遥控器的值乘以比例成电流值 直接发送到can总线上
} chassis_behaviour_e;



void chassis_behaviour_mode_set(chassis_move_t *chassis_move_mode);
void chassis_behaviour_control_set(fp32 *vx_set, fp32 *vy_set, fp32 *angle_set, chassis_move_t *chassis_move_rc_to_vector);





#endif
