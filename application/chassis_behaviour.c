#include "stdlib.h"
#include "chassis_behaviour.h"
#include "chassis_task.h"
#include "communicate_task.h"
#include "time.h"
#include "key_task.h"
static void chassis_agv_direction_follow_gimbal_yaw_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector);
static void chassis_agv_top_follow_gimbal_yaw_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector);



chassis_behaviour_e chassis_behaviour_mode = CHASSIS_DIRECTION_FOLLOW_GIMBAL_YAW;



/**
  * @brief          通过逻辑判断，赋值"chassis_behaviour_mode"成哪种模式
  * @param[in]      chassis_move_mode: 底盘数据
  * @retval         none
  */
void chassis_behaviour_mode_set(chassis_move_t *chassis_move_mode)
{
	static uint8_t flag=0;
    if (chassis_move_mode == NULL)
    {
        return;
    }
		

//		//跟随云台
//    if (chassis_move_mode->get_gimbal_data->rc_data.rc_sl==1)
//    {
//        chassis_behaviour_mode = CHASSIS_DIRECTION_FOLLOW_GIMBAL_YAW;
//    }
//		
		//键鼠操作
		if(TOE_IS_ERR_RC && !TOE_IS_ERR_IMAGETRANSFER)
		{
				if(chassis_move.get_gimbal_data->rc_data.rc_key_v & KEY_PRESSED_OFFSET_F)
					 chassis_behaviour_mode = CHASSIS_FLY; 
				else
				{
					if(flag == 1)
						chassis_behaviour_mode = CHASSIS_TOP_FOLLOW_GIMBAL_YAW;
					else if(flag == 0)
						chassis_behaviour_mode = CHASSIS_DIRECTION_FOLLOW_GIMBAL_YAW;
					
					if(Key_ScanValue.Key_Value.CTRL && !Key_ScanValue.Key_Value_Last.CTRL)
					{
						if(chassis_behaviour_mode == CHASSIS_DIRECTION_FOLLOW_GIMBAL_YAW)
						{
							chassis_behaviour_mode = CHASSIS_TOP_FOLLOW_GIMBAL_YAW;
							flag = 1;
						}
						else if(chassis_behaviour_mode == CHASSIS_TOP_FOLLOW_GIMBAL_YAW)
						{
							chassis_behaviour_mode = CHASSIS_DIRECTION_FOLLOW_GIMBAL_YAW;
							flag = 0;
						}
					}
				}
		}
    else if ( !TOE_IS_ERR_RC && chassis_move_mode->get_gimbal_data->rc_data.rc_sl==1)
    {
				if(chassis_move.get_gimbal_data->rc_data.rc_key_v & KEY_PRESSED_OFFSET_F)
					 chassis_behaviour_mode = CHASSIS_FLY; 
				else
				{
					if(flag == 1)
						chassis_behaviour_mode = CHASSIS_TOP_FOLLOW_GIMBAL_YAW;
					else if(flag == 0)
						chassis_behaviour_mode = CHASSIS_DIRECTION_FOLLOW_GIMBAL_YAW;
					
					if(Key_ScanValue.Key_Value.CTRL && !Key_ScanValue.Key_Value_Last.CTRL)
					{
						if(chassis_behaviour_mode == CHASSIS_DIRECTION_FOLLOW_GIMBAL_YAW)
						{
							chassis_behaviour_mode = CHASSIS_TOP_FOLLOW_GIMBAL_YAW;
							flag = 1;
						}
						else if(chassis_behaviour_mode == CHASSIS_TOP_FOLLOW_GIMBAL_YAW)
						{
							chassis_behaviour_mode = CHASSIS_DIRECTION_FOLLOW_GIMBAL_YAW;
							flag = 0;
						}
					}
				}
			}
		//底盘无力
    else if (chassis_move_mode->get_gimbal_data->rc_data.rc_sl==3)
    {
        chassis_behaviour_mode = CHASSIS_ZERO_FORCE;
    }
		//底盘小陀螺，不跟云台
		else if (chassis_move_mode->get_gimbal_data->rc_data.rc_sl==2)
    {
        chassis_behaviour_mode = CHASSIS_TOP_FOLLOW_GIMBAL_YAW;
    }
//		//底盘小陀螺，不跟云台
//		else if (chassis_move_mode->get_gimbal_data->rc_data.rc_sl==2)
//    {
//        chassis_behaviour_mode = CHASSIS_FLY;
//    }
//		//飞坡
//    else if (chassis_move_mode->get_gimbal_data->rc_data.rc_sl==3)
//    {
//        chassis_behaviour_mode = CHASSIS_FLY;
//    }

		
		
		
////				else if(chassis_move.get_gimbal_data->rc_data.rc_key_v & KEY_PRESSED_OFFSET_CTRL)
////						chassis_move_mode->chassis_mode = CHASSIS_VECTOR_TOP_FOLLOW_GIMBAL_YAW;
//				else
//					chassis_move_mode->chassis_mode = CHASSIS_VECTOR_DIRECTION_FOLLOW_GIMBAL_YAW; 
			
    

		
		//底盘无力
    if (chassis_behaviour_mode == CHASSIS_ZERO_FORCE)
    {
        chassis_move_mode->chassis_mode = CHASSIS_VECTOR_ZERO_FORCE; 
    }
		//小陀螺不跟随云台
		else if(chassis_behaviour_mode == CHASSIS_TOP_FOLLOW_GIMBAL_YAW)
		{

			  chassis_move_mode->chassis_mode = CHASSIS_VECTOR_TOP_FOLLOW_GIMBAL_YAW;

		}
	//飞坡
    else if (chassis_behaviour_mode == CHASSIS_FLY)
    {
        chassis_move_mode->chassis_mode = CHASSIS_VECTOR_FLY; 
    }
		//跟随云台
		else if(chassis_behaviour_mode == CHASSIS_DIRECTION_FOLLOW_GIMBAL_YAW)
		{
			chassis_move_mode->chassis_mode = CHASSIS_VECTOR_DIRECTION_FOLLOW_GIMBAL_YAW; 
		}
		
	chassis_move_mode->last_chassis_mode = chassis_move_mode->chassis_mode;
}





/**
  * @brief          设置控制量.根据不同底盘控制模式，三个参数会控制不同运动.在这个函数里面，会调用不同的控制函数.
  * @param[out]     vx_set, 通常控制纵向移动.
  * @param[out]     vy_set, 通常控制横向移动.
  * @param[out]     wz_set, 通常控制旋转运动.
  * @param[in]      chassis_move_rc_to_vector,  包括底盘所有信息.
  * @retval         none
  */

void chassis_behaviour_control_set(fp32 *vx_set, fp32 *vy_set, fp32 *angle_set, chassis_move_t *chassis_move_rc_to_vector)
{
    if (vx_set == NULL || vy_set == NULL || angle_set == NULL || chassis_move_rc_to_vector == NULL)
    {
        return;
    }
		
		
		//飞坡或者跟随云台模式
    if (chassis_behaviour_mode == CHASSIS_DIRECTION_FOLLOW_GIMBAL_YAW || chassis_behaviour_mode == CHASSIS_FLY )
    {
        chassis_agv_direction_follow_gimbal_yaw_control(vx_set, vy_set, angle_set, chassis_move_rc_to_vector);
    }
		//底盘小陀螺模式
    else if (chassis_behaviour_mode == CHASSIS_TOP_FOLLOW_GIMBAL_YAW)
    {
        chassis_agv_top_follow_gimbal_yaw_control(vx_set, vy_set, angle_set, chassis_move_rc_to_vector);
    }



}





//底盘跟随云台
static void chassis_agv_direction_follow_gimbal_yaw_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector)
{
    if (vx_set == NULL || vy_set == NULL || wz_set == NULL || chassis_move_rc_to_vector == NULL)
    {
        return;
    }

    chassis_rc_to_control_vector(vx_set,vy_set,wz_set,chassis_move_rc_to_vector);
		*wz_set = 0;
		
}



//小陀螺跟随云台正方向
static void chassis_agv_top_follow_gimbal_yaw_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector)
{
    if (vx_set == NULL || vy_set == NULL || wz_set == NULL || chassis_move_rc_to_vector == NULL)
    {
        return;
    }

    chassis_rc_to_control_vector(vx_set,vy_set,wz_set,chassis_move_rc_to_vector);
		*wz_set = 0;
}





