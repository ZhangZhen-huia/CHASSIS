#ifndef COMMUNICATE_TASK_H
#define COMMUNICATE_TASK_H


#include "main.h"

#define USART_RX_BUF_LENGHT     512
#define REFEREE_FIFO_BUF_LENGTH 1024


typedef struct
{
	fp32 vx_set;
	fp32 vy_set;
	fp32 wz_set;
	uint8_t rc_sl;
	uint8_t rc_sr;
	uint16_t rc_key_v;
	uint16_t rc_key_v_last;
}rc_data_t;



typedef enum
{
	Rc,
	ImageTransfer
}ControlMode_e;


typedef enum
{
	BLUE = 0,
	RED	 = 1
}EnemyColor_e;

typedef struct
{
	int16_t gimbal_mode;
	rc_data_t rc_data;
	uint8_t rc_err;
	uint8_t FireFlag;
	uint8_t FricState;
	uint8_t GimbalInit;
	ControlMode_e ControlMode;
	uint8_t Toe_is_errRc;
	uint8_t Toe_is_errImageTransfer;
	EnemyColor_e EnemyColor;
}gimbal_data_t;



typedef struct
{
	uint16_t bullet_speed;
}chassis_data_t;


void get_gimbal_data(gimbal_data_t *gimbal_data,uint8_t *buf);

#define TOE_IS_ERR_IMAGETRANSFER		gimbal_data.Toe_is_errImageTransfer
#define TOE_IS_ERR_RC				gimbal_data.Toe_is_errRc
#define POWER_OFF		(gimbal_data.ControlMode == Rc && TOE_IS_ERR_RC) || (gimbal_data.ControlMode == ImageTransfer && TOE_IS_ERR_IMAGETRANSFER)



extern gimbal_data_t gimbal_data;
const gimbal_data_t * get_gimbal_data_point(void);
const rc_data_t * get_gimbal_rc_data_point(void);
const uint8_t * get_autofire_flag_point(void);

void Chassis_data_transfer(void);



#endif


