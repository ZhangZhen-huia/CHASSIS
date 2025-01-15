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
  * @brief          ͨ���߼��жϣ���ֵ"chassis_behaviour_mode"������ģʽ
  * @param[in]      chassis_move_mode: ��������
  * @retval         none
  */
void chassis_behaviour_mode_set(chassis_move_t *chassis_move_mode)
{
	static uint8_t flag=0;
    if (chassis_move_mode == NULL)
    {
        return;
    }
		

//		//������̨
//    if (chassis_move_mode->get_gimbal_data->rc_data.rc_sl==1)
//    {
//        chassis_behaviour_mode = CHASSIS_DIRECTION_FOLLOW_GIMBAL_YAW;
//    }
//		
		//�������
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
		//��������
    else if (chassis_move_mode->get_gimbal_data->rc_data.rc_sl==3)
    {
        chassis_behaviour_mode = CHASSIS_ZERO_FORCE;
    }
		//����С���ݣ�������̨
		else if (chassis_move_mode->get_gimbal_data->rc_data.rc_sl==2)
    {
        chassis_behaviour_mode = CHASSIS_TOP_FOLLOW_GIMBAL_YAW;
    }
//		//����С���ݣ�������̨
//		else if (chassis_move_mode->get_gimbal_data->rc_data.rc_sl==2)
//    {
//        chassis_behaviour_mode = CHASSIS_FLY;
//    }
//		//����
//    else if (chassis_move_mode->get_gimbal_data->rc_data.rc_sl==3)
//    {
//        chassis_behaviour_mode = CHASSIS_FLY;
//    }

		
		
		
////				else if(chassis_move.get_gimbal_data->rc_data.rc_key_v & KEY_PRESSED_OFFSET_CTRL)
////						chassis_move_mode->chassis_mode = CHASSIS_VECTOR_TOP_FOLLOW_GIMBAL_YAW;
//				else
//					chassis_move_mode->chassis_mode = CHASSIS_VECTOR_DIRECTION_FOLLOW_GIMBAL_YAW; 
			
    

		
		//��������
    if (chassis_behaviour_mode == CHASSIS_ZERO_FORCE)
    {
        chassis_move_mode->chassis_mode = CHASSIS_VECTOR_ZERO_FORCE; 
    }
		//С���ݲ�������̨
		else if(chassis_behaviour_mode == CHASSIS_TOP_FOLLOW_GIMBAL_YAW)
		{

			  chassis_move_mode->chassis_mode = CHASSIS_VECTOR_TOP_FOLLOW_GIMBAL_YAW;

		}
	//����
    else if (chassis_behaviour_mode == CHASSIS_FLY)
    {
        chassis_move_mode->chassis_mode = CHASSIS_VECTOR_FLY; 
    }
		//������̨
		else if(chassis_behaviour_mode == CHASSIS_DIRECTION_FOLLOW_GIMBAL_YAW)
		{
			chassis_move_mode->chassis_mode = CHASSIS_VECTOR_DIRECTION_FOLLOW_GIMBAL_YAW; 
		}
		
	chassis_move_mode->last_chassis_mode = chassis_move_mode->chassis_mode;
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
		
		
		//���»��߸�����̨ģʽ
    if (chassis_behaviour_mode == CHASSIS_DIRECTION_FOLLOW_GIMBAL_YAW || chassis_behaviour_mode == CHASSIS_FLY )
    {
        chassis_agv_direction_follow_gimbal_yaw_control(vx_set, vy_set, angle_set, chassis_move_rc_to_vector);
    }
		//����С����ģʽ
    else if (chassis_behaviour_mode == CHASSIS_TOP_FOLLOW_GIMBAL_YAW)
    {
        chassis_agv_top_follow_gimbal_yaw_control(vx_set, vy_set, angle_set, chassis_move_rc_to_vector);
    }



}





//���̸�����̨
static void chassis_agv_direction_follow_gimbal_yaw_control(fp32 *vx_set, fp32 *vy_set, fp32 *wz_set, chassis_move_t *chassis_move_rc_to_vector)
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
		*wz_set = 0;
}





