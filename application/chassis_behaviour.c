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

		

		//������̨
    if (chassis_move_mode->get_gimbal_data->rc_data.rc_sl==1)
    {
        chassis_behaviour_mode = CHASSIS_INFANTRY_FOLLOW_GIMBAL_YAW;
    }
		
#ifdef RADAR
		
		//��ͼģʽ��Ϊ����������ϵ,����
		else if(chassis_move_mode->get_gimbal_data->rc_data.rc_sl==3)
		{
        chassis_behaviour_mode = CHASSIS_BUILD_MAP;
		}
		//�״�ģʽ������
    else if (chassis_move_mode->get_gimbal_data->rc_data.rc_sl==2)
    {
        chassis_behaviour_mode = CHASSIS_FOLLOW_RADAR;
    }
		
#else
		//����С���ݣ�������̨
		else if (chassis_move_mode->get_gimbal_data->rc_data.rc_sl==2)
    {
        chassis_behaviour_mode = CHASSIS_AGV_TOP_FOLLOW_GIMBAL_YAW;
    }
		//��������
    else if (chassis_move_mode->get_gimbal_data->rc_data.rc_sl==3)
    {
        chassis_behaviour_mode = CHASSIS_ZERO_FORCE;
    }
//		//����
//    else if (chassis_move_mode->get_gimbal_data->rc_data.rc_sl==3)
//    {
//        chassis_behaviour_mode = CHASSIS_FLY;
//    }
#endif
		
		
		
		
		//��������
    if (chassis_behaviour_mode == CHASSIS_ZERO_FORCE  || gimbal_data.Gimbal_init)
    {
        chassis_move_mode->chassis_mode = CHASSIS_VECTOR_ZERO_FORCE; 
    }
		//�������
		else if(chassis_behaviour_mode == CHASSIS_AGV_TOP_FOLLOW_GIMBAL_YAW)
		{
			  chassis_move_mode->chassis_mode = CHASSIS_VECTOR_AGV_FOLLOW_GIMBAL_YAW;

		}
		//������̨
    else if (chassis_behaviour_mode == CHASSIS_INFANTRY_FOLLOW_GIMBAL_YAW)
    {
        chassis_move_mode->chassis_mode = CHASSIS_VECTOR_FOLLOW_GIMBAL_YAW; 
    }
		//��ͼģʽ������������
    else if (chassis_behaviour_mode == CHASSIS_BUILD_MAP)
    {
        chassis_move_mode->chassis_mode = CHASSIS_VECTOR_BUILD_MAP; 
    }	
		//�����״�
    else if (chassis_behaviour_mode == CHASSIS_FOLLOW_RADAR)
    {
        chassis_move_mode->chassis_mode = CHASSIS_VECTOR_RADAR; 
    }
		//�����״�
    else if (chassis_behaviour_mode == CHASSIS_FLY)
    {
        chassis_move_mode->chassis_mode = CHASSIS_VECTOR_FLY; 
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





//���̸�����̨
static void chassis_infantry_follow_gimbal_yaw_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector)
{
    if (vx_set == NULL || vy_set == NULL || wz_set == NULL || chassis_move_rc_to_vector == NULL)
    {
        return;
    }

    chassis_rc_to_control_vector(vx_set,vy_set,wz_set,chassis_move_rc_to_vector);
		*wz_set = 0;
		
}



//С���ݸ�����̨������
static void chassis_agv_top_follow_gimbal_yaw_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector)
{
    if (vx_set == NULL || vy_set == NULL || wz_set == NULL || chassis_move_rc_to_vector == NULL)
    {
        return;
    }

    chassis_rc_to_control_vector(vx_set,vy_set,wz_set,chassis_move_rc_to_vector);

}


//��ͼģʽ
static void chassis_build_map_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector)
{
    if (vx_set == NULL || vy_set == NULL || wz_set == NULL || chassis_move_rc_to_vector == NULL)
    {
        return;
    }

			chassis_rc_to_control_vector(vx_set, vy_set, wz_set, chassis_move_rc_to_vector);
}


//�״�ģʽ
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
