#include "stm32f4xx_hal.h"
#include "main.h"
#include "Can_receive.h"
#include "usart.h"
#include "chassis_task.h"
#include "detect_task.h"
#include "user_task.h"

//电机数据读取
#define get_motor_measure(ptr, data)                                 \
{                                                                    \
	(ptr)->last_ecd = (ptr)->ecd;                                      \
	(ptr)->ecd = (uint16_t)((data)[0] << 8 | (data)[1]);               \
	(ptr)->rpm = (uint16_t)((data)[2] << 8 | (data)[3]);         			 \
	(ptr)->given_current = (uint16_t)((data)[4] << 8 | (data)[5]);     \
	(ptr)->temperate = (data)[6];                                      \
}




motor_measure_t drive_motor[4], course_motor[4],trigger_motor,yaw_motor;
uint8_t rx_data1[8],rx_data2[8];


void canfilter_init_start(void)
{
		CAN_FilterTypeDef can_filter_st;	                //定义过滤器结构体
    can_filter_st.FilterActivation = ENABLE;			//ENABLE使能过滤器
    can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;	//设置过滤器模式--标识符屏蔽位模式
    can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;	//过滤器的位宽 32 位
    can_filter_st.FilterIdHigh = 0x0000;				//ID高位
    can_filter_st.FilterIdLow = 0x0000;					//ID低位
    can_filter_st.FilterMaskIdHigh = 0x0000;			//过滤器掩码高位
    can_filter_st.FilterMaskIdLow = 0x0000;				//过滤器掩码低位
    
    can_filter_st.FilterBank = 0;						//过滤器组-双CAN可指定0~27
    can_filter_st.FilterFIFOAssignment = CAN_FILTER_FIFO0;	//与过滤器组管理的 FIFO
    HAL_CAN_ConfigFilter(&hcan1, &can_filter_st);		//HAL库配置过滤器函数
	
    HAL_CAN_Start(&hcan1);								//使能CAN1控制器
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);//使能CAN的各种中断
	
	
    can_filter_st.SlaveStartFilterBank = 14;   //双CAN模式下规定CAN的主从模式的过滤器分配，从过滤器为14
    can_filter_st.FilterBank = 14;						//过滤器组-双CAN可指定0~27
    HAL_CAN_ConfigFilter(&hcan2, &can_filter_st);		//HAL库配置过滤器函数
    HAL_CAN_Start(&hcan2);								//使能CAN2控制器
    HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);//使能CAN的各种中断
}



void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef*hcan)//  CAN FIFO0的中断回调函数，在里面完成数据的接收
{
	CAN_RxHeaderTypeDef rx_header1,rx_header2;
	if(hcan->Instance==CAN1)
	{
		HAL_CAN_GetRxMessage(&hcan1,CAN_RX_FIFO0,&rx_header1,rx_data1);
		switch(rx_header1.StdId)
		{
			case CAN_DRIVE_MOTOR1_ID:
				get_motor_measure(&drive_motor[0],rx_data1);
				detect_hook(DRIVE_MOTOR1_TOE);
				break;
			
			case CAN_DRIVE_MOTOR2_ID:
				get_motor_measure(&drive_motor[1],rx_data1);
				detect_hook(DRIVE_MOTOR2_TOE);
				break;
			
			case CAN_DRIVE_MOTOR3_ID:
				get_motor_measure(&drive_motor[2],rx_data1);
				detect_hook(DRIVE_MOTOR3_TOE);
				break;
					
			case CAN_DRIVE_MOTOR4_ID:
				get_motor_measure(&drive_motor[3],rx_data1);
				detect_hook(DRIVE_MOTOR4_TOE);
				break;				
			case GIMBAL_ID:
				get_gimbal_data(&gimbal_data,rx_data1);
				break;
			case 0x209:
				get_motor_measure(&yaw_motor,rx_data1);
			break;

		}
	}
	else if(hcan->Instance==CAN2)
	{
		HAL_CAN_GetRxMessage(&hcan2,CAN_RX_FIFO0,&rx_header2,rx_data2);
		switch(rx_header2.StdId)
		{
			case CAN_COURSE_MOTOR1_ID:
				get_motor_measure(&course_motor[0],rx_data2);
				detect_hook(COURSE_MOTOR1_TOE);
				break;
			
			case CAN_COURSE_MOTOR2_ID:
				get_motor_measure(&course_motor[1],rx_data2);
				detect_hook(COURSE_MOTOR2_TOE);
				break;
			
			case CAN_COURSE_MOTOR3_ID:
				get_motor_measure(&course_motor[2],rx_data2);
				detect_hook(COURSE_MOTOR3_TOE);			
				break;
			
			case CAN_COURSE_MOTOR4_ID:
				get_motor_measure(&course_motor[3],rx_data2);
				detect_hook(COURSE_MOTOR4_TOE);
				break;
			case TRIG_MOTOR_ID:
				get_motor_measure(&trigger_motor,rx_data2);
			break;
		}
	}
}



void CAN_cmd_drive(int16_t M1, int16_t M2, int16_t M3, int16_t M4)
{
	uint32_t send_mail_box;
	CAN_TxHeaderTypeDef chassis_tx_message;
	static uint8_t    chassis_can_send_data[8];
	
	chassis_tx_message.StdId = CAN_CHASSIS_DRIVE_ALL_ID;
	chassis_tx_message.DLC   = 0x08;
	chassis_tx_message.RTR	 = CAN_RTR_DATA;
	chassis_tx_message.IDE 	 = CAN_ID_STD;
	
	chassis_can_send_data[0]=M1>>8;
	chassis_can_send_data[1]=M1;
	
	chassis_can_send_data[2]=M2>>8;
	chassis_can_send_data[3]=M2;
	
	chassis_can_send_data[4]=M3>>8;
	chassis_can_send_data[5]=M3;
	
	chassis_can_send_data[6]=M4>>8;
	chassis_can_send_data[7]=M4;
	
	HAL_CAN_AddTxMessage(&hcan1,&chassis_tx_message,chassis_can_send_data,&send_mail_box);
}


void CAN_cmd_course(int16_t M1, int16_t M2, int16_t M3, int16_t M4)
{
		uint32_t send_mail_box;
		CAN_TxHeaderTypeDef chassis_tx_message;
		static uint8_t    chassis_can_send_data[8];
	
    chassis_tx_message.StdId = CAN_CHASSIS_COURSE_ALL_ID;
    chassis_tx_message.IDE = CAN_ID_STD;
    chassis_tx_message.RTR = CAN_RTR_DATA;
    chassis_tx_message.DLC = 0x08;
    chassis_can_send_data[0] = (M1 >> 8);
    chassis_can_send_data[1] = M1;
	
    chassis_can_send_data[2] = (M2 >> 8);
    chassis_can_send_data[3] = M2;
	
    chassis_can_send_data[4] = (M3 >> 8);
    chassis_can_send_data[5] = M3;
	
    chassis_can_send_data[6] = (M4 >> 8);
    chassis_can_send_data[7] = M4;
    HAL_CAN_AddTxMessage(&hcan2, &chassis_tx_message, chassis_can_send_data, &send_mail_box);
}


void CAN_cmd_trig(int16_t current)
{
	uint32_t send_mail_box;
	CAN_TxHeaderTypeDef chassis_tx_message;
	uint8_t    chassis_can_send_data[2];
	
  chassis_tx_message.StdId = CAN_TRIG_CMD_ID;
  chassis_tx_message.IDE = CAN_ID_STD;
  chassis_tx_message.RTR = CAN_RTR_DATA;
  chassis_tx_message.DLC = 0x02;
	
	chassis_can_send_data[0] = (current >> 8);
  chassis_can_send_data[1] = current;
	
  HAL_CAN_AddTxMessage(&hcan2, &chassis_tx_message, chassis_can_send_data, &send_mail_box);

	
}


/**
  * @brief          返回驱动电机数据指针
  * @param[in]      i: 电机编号,范围[0,3]
  * @retval         电机数据指针
  */
const motor_measure_t *get_chassis_drive_motor_measure_point(uint8_t i)
{
	return &drive_motor[(i&0x03)];
}

/**
  * @brief          返回航向电机数据指针
* @param[in]     	  i: 电机编号，范围[0,3]
  * @retval         电机数据指针
  */
const motor_measure_t *get_chassis_course_motor_measure_point(uint8_t i)
{
    return &course_motor[(i&0x03)];
}


//trigger
const motor_measure_t *get_gimbal_trigger_motor_measure_point(void)
{
    return &trigger_motor;
}

