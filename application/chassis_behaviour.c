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
  * @brief          ͨ���߼��жϣ���ֵ"chassis_behaviour_mode"������ģʽ
  * @param[in]      chassis_move_mode: ��������
  * @retval         none
  */
void chassis_behaviour_mode_set(chassis_move_t *chassis_move_mode)
{
    if (chassis_move_mode == NULL)
    {
        return;
    }


//    //remote control  set chassis behaviour mode
//    //ң��������ģʽ
//		//��������̨
//    if (switch_is_mid(chassis_move_mode->chassis_RC->rc.s[CHASSIS_MODE_CHANNEL]))
//    {
//        chassis_behaviour_mode = CHASSIS_NO_FOLLOW_YAW;
//    }
//		//���̸������yaw
//		    if (switch_is_down(chassis_move_mode->chassis_RC->rc.s[CHASSIS_MODE_CHANNEL]))
//    {
//        chassis_behaviour_mode = CHASSIS_AGV_FOLLOW_CHASSIS_YAW;
//    }
//		//������̨
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
		//������̨
    else if (chassis_move_mode->get_gimbal_data->rc_data.rc_sl==1)
    {
        chassis_behaviour_mode = CHASSIS_INFANTRY_FOLLOW_GIMBAL_YAW;
    }
		//��������
    else if (chassis_move_mode->get_gimbal_data->rc_data.rc_sl==3)
    {
        chassis_behaviour_mode = CHASSIS_ZERO_FORCE;
    }
		
//    //when gimbal in some mode, such as init mode, chassis must's move
//    //����̨��ĳЩģʽ�£����ʼ���� ���̲���
//    if (gimbal_cmd_to_chassis_stop())
//    {
//        chassis_behaviour_mode = CHASSIS_NO_MOVE;
//    }


    //add your own logic to enter the new mode
    //����Լ����߼��жϽ�����ģʽ


    //accord to beheviour mode, choose chassis control mode
    //������Ϊģʽѡ��һ�����̿���ģʽ
		
		//��������
    if (chassis_behaviour_mode == CHASSIS_ZERO_FORCE)
    {
        chassis_move_mode->chassis_mode = CHASSIS_VECTOR_ZERO_FORCE; 
    }	
		//�������
		else if(chassis_behaviour_mode == CHASSIS_AGV_FOLLOW_CHASSIS_YAW)
		{
			  chassis_move_mode->chassis_mode = CHASSIS_VECTOR_FOLLOW_CHASSIS_YAW;

		}
		//������̨
    else if (chassis_behaviour_mode == CHASSIS_INFANTRY_FOLLOW_GIMBAL_YAW)
    {
        chassis_move_mode->chassis_mode = CHASSIS_VECTOR_FOLLOW_GIMBAL_YAW; 
    }
		//������̨
    else if (chassis_behaviour_mode == CHASSIS_FOLLOW_RADAR)
    {
        chassis_move_mode->chassis_mode = CHASSIS_VECTOR_RADAR; 
    }		
	
}





/**
  * @brief          ���ÿ�����.���ݲ�ͬ���̿���ģʽ��������������Ʋ�ͬ�˶�.������������棬����ò�ͬ�Ŀ��ƺ���.
  * @param[out]     vx_set, ͨ�����������ƶ�.
  * @param[out]     vy_set, ͨ�����ƺ����ƶ�.
  * @param[out]     wz_set, ͨ��������ת�˶�.
  * @param[in]      chassis_move_rc_to_vector,  ��������������Ϣ.
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
  * @brief          ������������Ϊ״̬���£�����ģʽ��raw���ʶ��趨ֵ��ֱ�ӷ��͵�can�����Ϲʶ����趨ֵ������Ϊ0
  * @author         RM
  * @param[in]      vx_set,vy_set,wz_set�趨ֵ��ֱ�ӷ��͵�can������
  * @param[in]      chassis_move_rc_to_vector��������
  * @retval         ���ؿ�
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
  * @brief          ���̸�����̨����Ϊ״̬���£�����ģʽ�Ǹ�����̨�Ƕȣ�������ת�ٶȻ���ݽǶȲ���������ת�Ľ��ٶ�
  * @author         RM
  * @param[in]      vx_setǰ�����ٶ�,��ֵ ǰ���ٶȣ� ��ֵ �����ٶ�
  * @param[in]      vy_set���ҵ��ٶ�,��ֵ �����ٶȣ� ��ֵ �����ٶ�
  * @param[in]      angle_set��������̨���Ƶ�����ԽǶ�
  * @param[in]      chassis_move_rc_to_vector��������
  * @retval         ���ؿ�
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
  * @brief          ���̸������yaw����Ϊ״̬���£�����ģʽ�Ǹ�����̽Ƕȣ�������ת�ٶȻ���ݽǶȲ���������ת�Ľ��ٶ�
  * @author         RM
  * @param[in]      vx_setǰ�����ٶ�,��ֵ ǰ���ٶȣ� ��ֵ �����ٶ�
  * @param[in]      vy_set���ҵ��ٶ�,��ֵ �����ٶȣ� ��ֵ �����ٶ�
  * @param[in]      angle_set�������õ�yaw����Χ -PI��PI
  * @param[in]      chassis_move_rc_to_vector��������
  * @retval         ���ؿ�
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
  * @brief          ���̲�����Ƕȵ���Ϊ״̬���£�����ģʽ�ǲ�����Ƕȣ�������ת�ٶ��ɲ���ֱ���趨
  * @author         RM
  * @param[in]      vx_setǰ�����ٶ�,��ֵ ǰ���ٶȣ� ��ֵ �����ٶ�
  * @param[in]      vy_set���ҵ��ٶ�,��ֵ �����ٶȣ� ��ֵ �����ٶ�
  * @param[in]      wz_set�������õ���ת�ٶ�,��ֵ ��ʱ����ת����ֵ ˳ʱ����ת
  * @param[in]      chassis_move_rc_to_vector��������
  * @retval         ���ؿ�
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
