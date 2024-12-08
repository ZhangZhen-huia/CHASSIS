#include "stdlib.h"
#include "chassis_behaviour.h"
#include "chassis_task.h"
#include "communicate_task.h"
#include "time.h"
static void chassis_zero_force_control(fp32 *vx_can_set, fp32 *vy_can_set, fp32 *wz_can_set, chassis_move_t *chassis_move_rc_to_vector);
static void chassis_infantry_follow_gimbal_yaw_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector);
static void chassis_agv_follow_chassis_yaw_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector);
static void chassis_no_follow_yaw_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector);
static void chassis_follow_radar_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set);



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


//    //remote control  set chassis behaviour mode
//    //遥控器设置模式
//		//不跟随云台
//    if (switch_is_mid(chassis_move_mode->chassis_RC->rc.s[CHASSIS_MODE_CHANNEL]))
//    {
//        chassis_behaviour_mode = CHASSIS_NO_FOLLOW_YAW;
//    }
//		//底盘跟随底盘yaw
//		    if (switch_is_down(chassis_move_mode->chassis_RC->rc.s[CHASSIS_MODE_CHANNEL]))
//    {
//        chassis_behaviour_mode = CHASSIS_AGV_FOLLOW_CHASSIS_YAW;
//    }
//		//跟随云台
//    else if (switch_is_up(chassis_move_mode->chassis_RC->rc.s[CHASSIS_MODE_CHANNEL]))
//    {
//        chassis_behaviour_mode = CHASSIS_INFANTRY_FOLLOW_GIMBAL_YAW;
//    }
		if(gimbal_data.Radar_mode)
		{
        chassis_behaviour_mode = CHASSIS_FOLLOW_RADAR;
		}
    else if (chassis_move_mode->get_gimbal_data->rc_data.rc_sl==2)
    {
        chassis_behaviour_mode = CHASSIS_AGV_FOLLOW_CHASSIS_YAW;
    }
		//跟随云台
    else if (chassis_move_mode->get_gimbal_data->rc_data.rc_sl==1)
    {
        chassis_behaviour_mode = CHASSIS_INFANTRY_FOLLOW_GIMBAL_YAW;
    }
		//底盘无力
    else if (chassis_move_mode->get_gimbal_data->rc_data.rc_sl==3)
    {
        chassis_behaviour_mode = CHASSIS_ZERO_FORCE;
    }
		
//    //when gimbal in some mode, such as init mode, chassis must's move
//    //当云台在某些模式下，像初始化， 底盘不动
//    if (gimbal_cmd_to_chassis_stop())
//    {
//        chassis_behaviour_mode = CHASSIS_NO_MOVE;
//    }


    //add your own logic to enter the new mode
    //添加自己的逻辑判断进入新模式


    //accord to beheviour mode, choose chassis control mode
    //根据行为模式选择一个底盘控制模式
		
		//底盘无力
    if (chassis_behaviour_mode == CHASSIS_ZERO_FORCE)
    {
        chassis_move_mode->chassis_mode = CHASSIS_VECTOR_ZERO_FORCE; 
    }	
		//跟随底盘
		else if(chassis_behaviour_mode == CHASSIS_AGV_FOLLOW_CHASSIS_YAW)
		{
			  chassis_move_mode->chassis_mode = CHASSIS_VECTOR_FOLLOW_CHASSIS_YAW;

		}
		//跟随云台
    else if (chassis_behaviour_mode == CHASSIS_INFANTRY_FOLLOW_GIMBAL_YAW)
    {
        chassis_move_mode->chassis_mode = CHASSIS_VECTOR_FOLLOW_GIMBAL_YAW; 
    }
		//跟随云台
    else if (chassis_behaviour_mode == CHASSIS_FOLLOW_RADAR)
    {
        chassis_move_mode->chassis_mode = CHASSIS_VECTOR_RADAR; 
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
		
    if (chassis_behaviour_mode == CHASSIS_ZERO_FORCE)
    {
        chassis_zero_force_control(vx_set, vy_set, angle_set, chassis_move_rc_to_vector);
    }
    else if (chassis_behaviour_mode == CHASSIS_NO_MOVE)
    {
        chassis_zero_force_control(vx_set, vy_set, angle_set, chassis_move_rc_to_vector);
        //chassis_no_move_control(vx_set, vy_set, angle_set, chassis_move_rc_to_vector);
    }
    else if (chassis_behaviour_mode == CHASSIS_INFANTRY_FOLLOW_GIMBAL_YAW)
    {
				
        chassis_infantry_follow_gimbal_yaw_control(vx_set, vy_set, angle_set, chassis_move_rc_to_vector);
    }
    else if (chassis_behaviour_mode == CHASSIS_AGV_FOLLOW_CHASSIS_YAW)
    {
        chassis_agv_follow_chassis_yaw_control(vx_set, vy_set, angle_set, chassis_move_rc_to_vector);
    }
    else if (chassis_behaviour_mode == CHASSIS_NO_FOLLOW_YAW)
    {
        chassis_no_follow_yaw_control(vx_set, vy_set, angle_set, chassis_move_rc_to_vector);
    }
	  else if (chassis_behaviour_mode == CHASSIS_FOLLOW_RADAR)
    {
        chassis_follow_radar_control(vx_set, vy_set, angle_set);
    }
//    else if (chassis_behaviour_mode == CHASSIS_OPEN)
//    {
//        chassis_open_set_control(vx_set, vy_set, angle_set, chassis_move_rc_to_vector);
//    }
}


/**
  * @brief          底盘无力的行为状态机下，底盘模式是raw，故而设定值会直接发送到can总线上故而将设定值都设置为0
  * @author         RM
  * @param[in]      vx_set,vy_set,wz_set设定值将直接发送到can总线上
  * @param[in]      chassis_move_rc_to_vector底盘数据
  * @retval         返回空
  */

static void chassis_zero_force_control(fp32 *vx_can_set, fp32 *vy_can_set, fp32 *wz_can_set, chassis_move_t *chassis_move_rc_to_vector)
{
    if (vx_can_set == NULL || vy_can_set == NULL || wz_can_set == NULL || chassis_move_rc_to_vector == NULL)
    {
        return;
    }
    *vx_can_set = 0.0f;
    *vy_can_set = 0.0f;
    *wz_can_set = 0.0f;
}



/**
  * @brief          底盘跟随云台的行为状态机下，底盘模式是跟随云台角度，底盘旋转速度会根据角度差计算底盘旋转的角速度
  * @author         RM
  * @param[in]      vx_set前进的速度,正值 前进速度， 负值 后退速度
  * @param[in]      vy_set左右的速度,正值 左移速度， 负值 右移速度
  * @param[in]      angle_set底盘与云台控制到的相对角度
  * @param[in]      chassis_move_rc_to_vector底盘数据
  * @retval         返回空
  */

static void chassis_infantry_follow_gimbal_yaw_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector)
{
    if (vx_set == NULL || vy_set == NULL || wz_set == NULL || chassis_move_rc_to_vector == NULL)
    {
        return;
    }

    chassis_rc_to_control_vector(vx_set,vy_set,wz_set,chassis_move_rc_to_vector);
		*wz_set = 0;
		
}



/**
  * @brief          底盘跟随底盘yaw的行为状态机下，底盘模式是跟随底盘角度，底盘旋转速度会根据角度差计算底盘旋转的角速度
  * @author         RM
  * @param[in]      vx_set前进的速度,正值 前进速度， 负值 后退速度
  * @param[in]      vy_set左右的速度,正值 左移速度， 负值 右移速度
  * @param[in]      angle_set底盘设置的yaw，范围 -PI到PI
  * @param[in]      chassis_move_rc_to_vector底盘数据
  * @retval         返回空
  */

static void chassis_agv_follow_chassis_yaw_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector)
{
    if (vx_set == NULL || vy_set == NULL || wz_set == NULL || chassis_move_rc_to_vector == NULL)
    {
        return;
    }

    chassis_rc_to_control_vector(vx_set,vy_set,wz_set,chassis_move_rc_to_vector);
//		*wz_set =rand();
//		chassis_move_rc_to_vector->absolute_chassis_speed.Vx = *vx_set;
//		chassis_move_rc_to_vector->absolute_chassis_speed.Vy = *vy_set;
//		chassis_move_rc_to_vector->absolute_chassis_speed.Wz = *wz_set;
}


/**
  * @brief          底盘不跟随角度的行为状态机下，底盘模式是不跟随角度，底盘旋转速度由参数直接设定
  * @author         RM
  * @param[in]      vx_set前进的速度,正值 前进速度， 负值 后退速度
  * @param[in]      vy_set左右的速度,正值 左移速度， 负值 右移速度
  * @param[in]      wz_set底盘设置的旋转速度,正值 逆时针旋转，负值 顺时针旋转
  * @param[in]      chassis_move_rc_to_vector底盘数据
  * @retval         返回空
  */

static void chassis_no_follow_yaw_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector)
{
    if (vx_set == NULL || vy_set == NULL || wz_set == NULL || chassis_move_rc_to_vector == NULL)
    {
        return;
    }

    //chassis_rc_to_control_vector(vx_set, vy_set, wz_set, chassis_move_rc_to_vector);
}



static void chassis_follow_radar_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set)
{
    if (vx_set == NULL || vy_set == NULL || wz_set == NULL )
    {
        return;
    }
//    *vx_set = 0;
//		*vy_set = 0;
//		*wz_set = 0;
    *vx_set = (gimbal_data.rc_data.vx_set/20.0f+33)/100.0f;
		*vy_set = (gimbal_data.rc_data.vy_set/20.0f+33)/100.0f;
		*wz_set = (gimbal_data.rc_data.wz_set/20.0f+33)/100.0f;
		
		
		
}
