/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       test_task.c/h
  * @brief      buzzer warning task.蜂鸣器报警任务
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Nov-11-2019     RM              1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2019 DJI****************************
  */

#include "test_task.h"
#include "main.h"
#include "cmsis_os.h"
#include "INS_task.h"
#include "tim.h"
#include "bsp_buzzer.h"
#include "detect_task.h"
//uint8_t Bee_flag=1;
uint16_t psc = 1;
uint16_t pwm = 5000;

//static void Motor_test(void);



/*-- 检测底盘C板是否正常工作 --*/
void test_task(void const * argument)
{

    while(1)
    {
			//Motor_test();
//			if(Bee_flag==1)
//			{	
//					Bee_flag=0;
					buzzer_on(psc, pwm);
					osDelay(500);
					buzzer_off();
				
					vTaskDelete(NULL);
		
    }
}

///*-- 检测在运动中底盘电机是否掉线 --*/
//static void Motor_test(void)
//{
//	if(toe_is_error(DRIVE_MOTOR1_TOE))
//	{
//		buzzer_on(psc, pwm);
//		osDelay(100);
//		buzzer_off();
//	}
//	if(toe_is_error(DRIVE_MOTOR2_TOE))
//	{
//		for(uint8_t i=0;i<2;i++)
//		{
//			buzzer_on(psc, pwm);
//			osDelay(100);
//			buzzer_off();
//		}
//	}
//	if(toe_is_error(DRIVE_MOTOR3_TOE))
//	{
//		for(uint8_t i=0;i<3;i++)
//		{
//			buzzer_on(psc, pwm);
//			osDelay(100);
//			buzzer_off();
//		}
//	}
//	if(toe_is_error(DRIVE_MOTOR4_TOE))
//	{
//		for(uint8_t i=0;i<4;i++)
//		{
//			buzzer_on(psc, pwm);
//			osDelay(100);
//			buzzer_off();
//		}
//	}
//	
//	if(toe_is_error(COURSE_MOTOR1_TOE))
//	{
//		buzzer_on(psc+1, pwm);
//		osDelay(100);
//		buzzer_off();
//	}
//	if(toe_is_error(COURSE_MOTOR2_TOE))
//	{
//		for(uint8_t i=0;i<2;i++)
//		{
//			buzzer_on(psc+1, pwm);
//			osDelay(100);
//			buzzer_off();
//		}
//	}
//	if(toe_is_error(COURSE_MOTOR3_TOE))
//	{
//		for(uint8_t i=0;i<3;i++)
//		{
//			buzzer_on(psc+1, pwm);
//			osDelay(100);
//			buzzer_off();
//		}
//	}
//	if(toe_is_error(COURSE_MOTOR4_TOE))
//	{
//		for(uint8_t i=0;i<4;i++)
//		{
//			buzzer_on(psc+1, pwm);
//			osDelay(100);
//			buzzer_off();
//		}
//	}
//	if(toe_is_error(TRIGGER_MOTOR_TOE))
//	{
//		buzzer_on(psc+1, pwm);
//		osDelay(500);
//		buzzer_off();
//	}
//}
