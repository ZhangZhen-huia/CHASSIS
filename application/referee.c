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
/*                       ����ϵͳ�ṹ�����             */	
/*------------------------------------------------------*/	
Referee_System_t  Referee_System;


/*------------------------------------------------------*/	
/*                       ����ϵͳ���ճ�ʼ��             */	
/*------------------------------------------------------*/
void Referee_Sys_Init(void)
{
    referee_init(Referee_System.RS_rx_buf[0], Referee_System.RS_rx_buf[1], RS_RX_BUF_NUM, Referee_System.RS_tx_buf, RS_TX_BUF_NUM);	
}

/*------------------------------------------------------*/	
/*                       ��ȡ����ϵͳ���ݽṹ           */	
/*------------------------------------------------------*/	

Referee_System_t *get_referee_data_point(void)
{
    return &Referee_System;
}


/*------------------------------------------------------*/	
/*                         ����ϵͳ�����ж�             */	
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
/*                   ����ϵͳ֡���ݴ�����             */	
/*------------------------------------------------------*/	
void Referee_Data_Process(uint8_t *data)
{
	//ϵͳ֡ͷָ��ָ���һ���ֽ�
	Referee_System.RS_frame_point = (RS_frame_point_t*)data;
	
	while( Referee_System.this_time_rx_len > 0 )
	{
		if(Referee_System.RS_frame_point->frame_header.SOF ==0xA5)
		{
			if(Verify_CRC8_Check_Sum(&(Referee_System.RS_frame_point->frame_header.SOF),5) == 1)
			{
                 //���ݸ�ֵ����
			     Referee_Data_Receive(&Referee_System.RS_frame_point->data_0);
			     
		         //��ȡʣ��֡����
			     Referee_System.this_time_rx_len -= (Referee_System.RS_frame_point->frame_header.data_length + 9);
			
			     //�л�֡ͷָ��
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
/*                       ���ݸ�ֵ����                   */	
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
		   Referee_System.sentry_gun_count = sentry_info & GUN_COUNT_MASK;           // ��ȡλ 0-10 ������
		   Referee_System.sentry_gun_exchange_count = (sentry_info & GUN_EXCHANGE_MASK) >> 11;  // ��ȡλ 11-14 ������
		   Referee_System.sentry_hp_exchange_count = (sentry_info & HP_EXCHANGE_MASK) >> 15;    // ��ȡλ 15-18 ������
		  break;

		case 0x0301:
		  Referee_0x0301Data_Receive(data);
		  break;
		
		default: break;
		  
	} 
}

/*------------------------------------------------------*/	
/*             ���ݸ�ֵ����(0x0301����)                  */	
/*------------------------------------------------------*/

void Referee_0x0301Data_Receive(uint8_t *data)
{
	//���ݶ�֡ͷָ�����ݶε�һ���ֽ�
   	Referee_System.Data_frame_point = (Data_frame_point_t*)data;
	
	//�жϽ�����ID�Ƿ��Լ�
	if( (Referee_System.Data_frame_point->receiver_ID == Hero_R) || (Referee_System.Data_frame_point->receiver_ID == Hero_B))
	{   
	    switch(Referee_System.Data_frame_point->data_cmd_id)
		{
			
			default: break;
		}
	}
}

/*------------------------------------------------------*/	
/*            ����ϵͳ���ݹ����ͷ���                    */	
/*------------------------------------------------------*/
//void   Referee_TX_send(uint32_t cmd,uint8_t *data, uint8_t num)
//{
//		//֡ͷ
//		Referee_System.RS_tx_buf[0] = 0xA5;

//		//���ݳ���
//		*(uint16_t*)(&Referee_System.RS_tx_buf[1]) = num + 6;

//		//�����
//		static uint8_t seq = 0;
//		seq++;
//		Referee_System.RS_tx_buf[3] = seq;

//		//CRC8У��
//		Append_CRC8_Check_Sum(Referee_System.RS_tx_buf,5);

//		//cmd-id
//		*(uint16_t*)(&Referee_System.RS_tx_buf[5]) = 0x0301;//cmd id:0x0301//������

//		//��������ID
//		*(uint16_t*)(&Referee_System.RS_tx_buf[7]) = cmd;//������

//		//������ID
//		*(uint16_t*)(&Referee_System.RS_tx_buf[9]) = (uint16_t)(Referee_System.ext_game_robot_state.robot_id);

//		//������ID
//		
//		*(uint16_t*)(&Referee_System.RS_tx_buf[11]) = (uint16_t)0x8080;
//		
//	

//		//ͼ������
//		memcpy(&Referee_System.RS_tx_buf[13],data,num);

//		//CRC16У��
//		Append_CRC16_Check_Sum(Referee_System.RS_tx_buf,15+num);

//		Referee_DMA_TX(15+num);
//}
