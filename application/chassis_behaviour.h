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
  CHASSIS_ZERO_FORCE,                   //chassis will be like no power,��������, ��û�ϵ�����
  CHASSIS_NO_MOVE,                      //chassis will be stop,���̱��ֲ���
  CHASSIS_INFANTRY_FOLLOW_GIMBAL_YAW,   //chassis will follow gimbal, usually in infantry,�����������̸�����̨
	CHASSIS_FOLLOW_RADAR,									//���̸����״�����
	CHASSIS_BUILD_MAP,
	CHASSIS_AGV_TOP_FOLLOW_GIMBAL_YAW,				//���̸������yaw
  CHASSIS_OPEN                          //the value of remote control will mulitiply a value, get current value that will be sent to can bus
                                        // ң������ֵ���Ա����ɵ���ֵ ֱ�ӷ��͵�can������
} chassis_behaviour_e;



void chassis_behaviour_mode_set(chassis_move_t *chassis_move_mode);
void chassis_behaviour_control_set(fp32 *vx_set, fp32 *vy_set, fp32 *angle_set, chassis_move_t *chassis_move_rc_to_vector);





#endif
