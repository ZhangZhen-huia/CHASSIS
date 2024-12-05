#include "referee.h"
#include "detect_task.h"
#include "string.h"
#include "usart.h"
#include "crc8_crc16.h"
#include "bsp_usart.h"

static void Referee_Data_Process(uint8_t *data);
static void Referee_Data_Receive(uint8_t *data);
static void Referee_0x0301Data_Receive(uint8_t *data);
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
//			DetectHook(REFEREE_TOE);
		}
		else
		{
			__HAL_DMA_DISABLE(huart6.hdmarx);
			Referee_System.this_time_rx_len = RS_RX_BUF_NUM - __HAL_DMA_GET_COUNTER(huart6.hdmarx);
			__HAL_DMA_SET_COUNTER(huart6.hdmarx, RS_RX_BUF_NUM);
			huart6.hdmarx->Instance->CR &= ~(DMA_SxCR_CT);
			__HAL_DMA_ENABLE(huart6.hdmarx);
		
		Referee_Data_Process(Referee_System.RS_rx_buf[1]);
//		DetectHook(REFEREE_TOE);
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
	static uint32_t sentry_info=0;
	switch(Referee_System.RS_frame_point->cmd_id)
	{
		case 0x0001:
		  memmove(&Referee_System.ext_game_status ,data, 3);
		  break;
		  		
		case 0x003:
		  memmove(&Referee_System.ext_game_robot_HP, data, 32);	
		  break;

		case 0x0102:
		  memmove(&Referee_System.ext_supply_projectile_action ,data,4);
		  break;

		case 0x0201:
		  memmove(&Referee_System.ext_game_robot_state ,data,27);
		  break;
			
		case 0x0202:
		  memmove(&Referee_System.ext_power_heat_data ,data,16);
		  break;
	
		case 0x0206:
		  memmove(&Referee_System.ext_robot_hurt ,data,6);
		  break;
		
		case 0x0207:
		  memmove(&Referee_System.ext_shoot_data ,data,7);
		  break;

		case 0x0209:
		  memmove(&Referee_System.ext_rfid_status ,data,4);
		  break;
		
		case 0x020D:
		  memmove(&sentry_info ,data,4);
		   Referee_System.sentry_gun_count = sentry_info & GUN_COUNT_MASK;           // 提取位 0-10 的数据
		   Referee_System.sentry_gun_exchange_count = (sentry_info & GUN_EXCHANGE_MASK) >> 11;  // 提取位 11-14 的数据
		   Referee_System.sentry_hp_exchange_count = (sentry_info & HP_EXCHANGE_MASK) >> 15;    // 提取位 15-18 的数据
		  break;

		case 0x0301:
		  Referee_0x0301Data_Receive(data);
		  break;
		
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
	
	//判断接收者ID是否自己
	if( (Referee_System.Data_frame_point->receiver_ID == Hero_R) || (Referee_System.Data_frame_point->receiver_ID == Hero_B))
	{   
	    switch(Referee_System.Data_frame_point->data_cmd_id)
		{
			
			default: break;
		}
	}
}

/*------------------------------------------------------*/	
/*            裁判系统数据规整和发送                    */	
/*------------------------------------------------------*/
//void   Referee_TX_send(uint32_t cmd,uint8_t *data, uint8_t num)
//{
//		//帧头
//		Referee_System.RS_tx_buf[0] = 0xA5;

//		//数据长度
//		*(uint16_t*)(&Referee_System.RS_tx_buf[1]) = num + 6;

//		//包序号
//		static uint8_t seq = 0;
//		seq++;
//		Referee_System.RS_tx_buf[3] = seq;

//		//CRC8校验
//		Append_CRC8_Check_Sum(Referee_System.RS_tx_buf,5);

//		//cmd-id
//		*(uint16_t*)(&Referee_System.RS_tx_buf[5]) = 0x0301;//cmd id:0x0301//命令码

//		//数据内容ID
//		*(uint16_t*)(&Referee_System.RS_tx_buf[7]) = cmd;//子内容

//		//发送者ID
//		*(uint16_t*)(&Referee_System.RS_tx_buf[9]) = (uint16_t)(Referee_System.ext_game_robot_state.robot_id);

//		//接收者ID
//		
//		*(uint16_t*)(&Referee_System.RS_tx_buf[11]) = (uint16_t)0x8080;
//		
//	

//		//图形数据
//		memcpy(&Referee_System.RS_tx_buf[13],data,num);

//		//CRC16校验
//		Append_CRC16_Check_Sum(Referee_System.RS_tx_buf,15+num);

//		Referee_DMA_TX(15+num);
//}
