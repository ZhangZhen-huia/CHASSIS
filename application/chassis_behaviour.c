#include "stdlib.h"
#include "chassis_behaviour.h"
#include "chassis_task.h"
#include "communicate_task.h"
#include "time.h"
static void chassis_infantry_follow_gimbal_yaw_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector);
static void chassis_agv_top_follow_gimbal_yaw_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector);
static void chassis_follow_radar_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set);
static void chassis_build_map_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector);



chassis_behaviour_e chassis_behaviour_mode = CHASSIS_ZERO_FORCE;



/**
  * @brief          通过逻辑判断，赋值"chassis_behaviour_mode"成哪种模式
  * @param[in]      chassis_move_mode: 底盘数据
  * @retval         none
  */
void chassis_behaviour_mode_set(chassis_move_t *chassis_move_mode)
{
    if (chassis_move_mode == NULL)
    {
        return;
    }

		

		//跟随云台
    if (chassis_move_mode->get_gimbal_data->rc_data.rc_sl==1)
    {
        chassis_behaviour_mode = CHASSIS_INFANTRY_FOLLOW_GIMBAL_YAW;
    }
		
#ifdef RADAR
		
		//建图模式，为机器人坐标系,左中
		else if(chassis_move_mode->get_gimbal_data->rc_data.rc_sl==3)
		{
        chassis_behaviour_mode = CHASSIS_BUILD_MAP;
		}
		//雷达模式，左下
    else if (chassis_move_mode->get_gimbal_data->rc_data.rc_sl==2)
    {
        chassis_behaviour_mode = CHASSIS_FOLLOW_RADAR;
    }
		
#else
		//底盘小陀螺，不跟云台
		else if (chassis_move_mode->get_gimbal_data->rc_data.rc_sl==2)
    {
        chassis_behaviour_mode = CHASSIS_AGV_TOP_FOLLOW_GIMBAL_YAW;
    }
		//底盘无力
    else if (chassis_move_mode->get_gimbal_data->rc_data.rc_sl==3)
    {
        chassis_behaviour_mode = CHASSIS_ZERO_FORCE;
    }
//		//飞坡
//    else if (chassis_move_mode->get_gimbal_data->rc_data.rc_sl==3)
//    {
//        chassis_behaviour_mode = CHASSIS_FLY;
//    }
#endif
		
		
		
		
		//底盘无力
    if (chassis_behaviour_mode == CHASSIS_ZERO_FORCE  || gimbal_data.Gimbal_init)
    {
        chassis_move_mode->chassis_mode = CHASSIS_VECTOR_ZERO_FORCE; 
    }
		//跟随底盘
		else if(chassis_behaviour_mode == CHASSIS_AGV_TOP_FOLLOW_GIMBAL_YAW)
		{
			  chassis_move_mode->chassis_mode = CHASSIS_VECTOR_AGV_FOLLOW_GIMBAL_YAW;

		}
		//跟随云台
    else if (chassis_behaviour_mode == CHASSIS_INFANTRY_FOLLOW_GIMBAL_YAW)
    {
        chassis_move_mode->chassis_mode = CHASSIS_VECTOR_FOLLOW_GIMBAL_YAW; 
    }
		//建图模式，机器人坐标
    else if (chassis_behaviour_mode == CHASSIS_BUILD_MAP)
    {
        chassis_move_mode->chassis_mode = CHASSIS_VECTOR_BUILD_MAP; 
    }	
		//跟随雷达
    else if (chassis_behaviour_mode == CHASSIS_FOLLOW_RADAR)
    {
        chassis_move_mode->chassis_mode = CHASSIS_VECTOR_RADAR; 
    }
		//跟随雷达
    else if (chassis_behaviour_mode == CHASSIS_FLY)
    {
        chassis_move_mode->chassis_mode = CHASSIS_VECTOR_FLY; 
    }
		
		
	
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
		

    if (chassis_behaviour_mode == CHASSIS_INFANTRY_FOLLOW_GIMBAL_YAW || chassis_behaviour_mode == CHASSIS_FLY )
    {
        chassis_infantry_follow_gimbal_yaw_control(vx_set, vy_set, angle_set, chassis_move_rc_to_vector);
    }
    else if (chassis_behaviour_mode == CHASSIS_AGV_TOP_FOLLOW_GIMBAL_YAW)
    {
        chassis_agv_top_follow_gimbal_yaw_control(vx_set, vy_set, angle_set, chassis_move_rc_to_vector);
    }

		else if (chassis_behaviour_mode == CHASSIS_BUILD_MAP)
		{
			chassis_build_map_control(vx_set, vy_set, angle_set,chassis_move_rc_to_vector);
		}
	  else if (chassis_behaviour_mode == CHASSIS_FOLLOW_RADAR)
    {
        chassis_follow_radar_control(vx_set, vy_set, angle_set);
    }

}





//底盘跟随云台
static void chassis_infantry_follow_gimbal_yaw_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector)
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

}


//建图模式
static void chassis_build_map_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector)
{
    if (vx_set == NULL || vy_set == NULL || wz_set == NULL || chassis_move_rc_to_vector == NULL)
    {
        return;
    }

			chassis_rc_to_control_vector(vx_set, vy_set, wz_set, chassis_move_rc_to_vector);
}


//雷达模式
static void chassis_follow_radar_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set)
{
    if (vx_set == NULL || vy_set == NULL || wz_set == NULL )
    {
        return;
    }
//    *vx_set = 0;
//		*vy_set = 0;
//		*wz_set = 0;
    *vx_set = gimbal_data.rc_data.vx_set/42.0f-5.0f;
		*vy_set = gimbal_data.rc_data.vy_set/42.0f-5.0f;
		*wz_set = 0;
		
		
		
}
