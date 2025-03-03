#include "referee.h"
#include "detect_task.h"
#include "string.h"
#include "usart.h"
#include "crc8_crc16.h"
#include "bsp_usart.h"
#include "vofa_task.h"

static void Referee_Data_Process(uint8_t *data);
static void Referee_Data_Receive(uint8_t *data);
static void Referee_0x0301Data_Receive(uint8_t *data);

//static void Referee_Data_sentry_process();
/*------------------------------------------------------*/	
/*                       裁判系统结构体变量             */	
/*------------------------------------------------------*/	
Referee_System_t  Referee_System;


/*------------------------------------------------------*/	
/*                       裁判系统接收初始化             */	
/*------------------------------------------------------*/
void Referee_Sys_Init(void)
{
    referee_init(Referee_System.RS_rx_buf[0], Referee_System.RS_rx_buf[1], RS_RX_BUF_NUM, Referee_System.RS_tx_buf, RS_TX_BUF_NUM);	
}

/*------------------------------------------------------*/	
/*                       获取裁判系统数据结构           */	
/*------------------------------------------------------*/	

Referee_System_t *get_referee_data_point(void)
{
    return &Referee_System;
}


/*------------------------------------------------------*/	
/*                         裁判系统串口中断             */	
/*------------------------------------------------------*/	

void USART6_IRQHandler(void)
{
	if(USART6->SR & UART_FLAG_IDLE)
	{
		__HAL_UART_CLEAR_PEFLAG(&huart6);


		if ((huart6.hdmarx->Instance->CR & DMA_SxCR_CT) == RESET)
		{
			__HAL_DMA_DISABLE(huart6.hdmarx);
			Referee_System.this_time_rx_len = RS_RX_BUF_NUM - __HAL_DMA_GET_COUNTER(huart6.hdmarx);
			__HAL_DMA_SET_COUNTER(huart6.hdmarx, RS_RX_BUF_NUM);
			huart6.hdmarx->Instance->CR |= DMA_SxCR_CT;
			__HAL_DMA_ENABLE(huart6.hdmarx);
			
			Referee_Data_Process(Referee_System.RS_rx_buf[0]);
			detect_hook(REFEREE_TOE);
		}
		else
		{
			__HAL_DMA_DISABLE(huart6.hdmarx);
			Referee_System.this_time_rx_len = RS_RX_BUF_NUM - __HAL_DMA_GET_COUNTER(huart6.hdmarx);
			__HAL_DMA_SET_COUNTER(huart6.hdmarx, RS_RX_BUF_NUM);
			huart6.hdmarx->Instance->CR &= ~(DMA_SxCR_CT);
			__HAL_DMA_ENABLE(huart6.hdmarx);
		
		Referee_Data_Process(Referee_System.RS_rx_buf[1]);
		detect_hook(REFEREE_TOE);
		}
	}   
}
/*------------------------------------------------------*/	
/*                   裁判系统帧数据处理函数             */	
/*------------------------------------------------------*/	
void Referee_Data_Process(uint8_t *data)
{
	//系统帧头指针指向第一个字节
	Referee_System.RS_frame_point = (RS_frame_point_t*)data;
	
	while( Referee_System.this_time_rx_len > 0 )
	{
		if(Referee_System.RS_frame_point->frame_header.SOF ==0xA5)
		{
			if(Verify_CRC8_Check_Sum(&(Referee_System.RS_frame_point->frame_header.SOF),5) == 1)
			{
                 //数据赋值函数
			     Referee_Data_Receive(&Referee_System.RS_frame_point->data_0);
			     
		         //获取剩余帧长度
			     Referee_System.this_time_rx_len -= (Referee_System.RS_frame_point->frame_header.data_length + 9);
			
			     //切换帧头指针
			     Referee_System.RS_frame_point = (RS_frame_point_t*)((uint8_t*)Referee_System.RS_frame_point +
				(Referee_System.RS_frame_point->frame_header.data_length + 9));
			}
			else
			{
			   break;
			}
		}
		else
		{
		   break;
		}
	}
}

/*------------------------------------------------------*/	
/*                       数据赋值函数                   */	
/*------------------------------------------------------*/

void Referee_Data_Receive(uint8_t *data)
{
	switch(Referee_System.RS_frame_point->cmd_id)
	{
		case 0x0001:
		  memmove(&Referee_System.ext_game_status ,data, 11);
		  break;
		  		
		case 0x0003:
		  memmove(&Referee_System.ext_game_robot_HP, data, 32);	
		  break;


		case 0x0201:
		  memmove(&Referee_System.ext_game_robot_state ,data,13);
		  break;
			
		case 0x0202:
		  memmove(&Referee_System.ext_power_heat_data ,data,16);
		  break;
	
		case 0x0206:
		  memmove(&Referee_System.ext_robot_hurt ,data,1);
		  break;
		
		case 0x0207:
		  memmove(&Referee_System.ext_shoot_data ,data,7);
		  break;

		case 0x0209:
		  memmove(&Referee_System.ext_rfid_status ,data,4);
		  break;
		


		case 0x0301:
		  Referee_0x0301Data_Receive(data);
		  break;
		
//		case 0x0309:
//					  memmove(&Referee_System.robot_custom_data ,data,4);

		default: break;
		  
	} 
}

/*------------------------------------------------------*/	
/*             数据赋值函数(0x0301数据)                  */	
/*------------------------------------------------------*/

void Referee_0x0301Data_Receive(uint8_t *data)
{
	//数据段帧头指向数据段第一个字节
   	Referee_System.Data_frame_point = (Data_frame_point_t*)data;
	
//	//判断接收者ID是否自己
//	if( (Referee_System.Data_frame_point->receiver_ID == Hero_R) || (Referee_System.Data_frame_point->receiver_ID == Hero_B))
//	{   
//	    switch(Referee_System.Data_frame_point->data_cmd_id)
//		{
//			
//			default: break;
//		}
//	}
}


//获取底盘功率和缓冲功率
void get_chassis_power_and_buffer(fp32 *power, fp32 *buffer)
{
    *power =  Referee_System.ext_power_heat_data.chassis_power;
    *buffer =  Referee_System.ext_power_heat_data.chassis_power_buffer;

}


//获得机器人id
uint8_t get_robot_id(void)
{
    return Referee_System.ext_game_robot_state.robot_id;
}






