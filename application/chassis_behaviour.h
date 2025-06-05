#ifndef __CHASSIA_BEHAVIOR__H
#define __CHASSIA_BEHAVIOR__H

#include "main.h"

#include "struct_typedef.h"

#include "chassis_task.h"





typedef enum
{
  CHASSIS_ZERO_FORCE,                   //chassis will be like no power,��������, ��û�ϵ�����
  CHASSIS_DIRECTION_FOLLOW_GIMBAL_YAW,   //chassis will follow gimbal, usually in infantry,�����������̸�����̨
	CHASSIS_TOP_FOLLOW_GIMBAL_YAW,				//���̸������yaw
  CHASSIS_ALONE,                        //���̲�����yaw 
	CHASSIS_FLY														//����
} chassis_behaviour_e;



void chassis_behaviour_mode_set(chassis_move_t *chassis_move_mode);
void chassis_behaviour_control_set(fp32 *vx_set, fp32 *vy_set, fp32 *angle_set, chassis_move_t *chassis_move_rc_to_vector);





#endif
