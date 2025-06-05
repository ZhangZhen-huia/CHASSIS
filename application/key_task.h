#ifndef __KEY_TASK_H
#define __KEY_TASK_H


#include "main.h"
#include "remote_control.h"


#define IMG_TRANSFER_KEY_FN1	(uint32_t)(1<<16)
#define IMG_TRANSFER_KEY_FN2	(uint32_t)(1<<17)
#define IMG_TRANSFER_KEY_TRIGGER	(uint32_t)(1<<18)
#define IMG_TRANSFER_KEY_PAUSE	(uint32_t)(1<<19)

#define IMG_TRANSFER_SW_RIGHT	(uint32_t)(1<<24)
#define IMG_TRANSFER_SW_LEFT	(uint32_t)(1<<25)

typedef struct
{
	uint8_t W;
	uint8_t A;
	uint8_t S;
	uint8_t D;
	uint8_t SHIFT;
	uint8_t CTRL;
	uint8_t Q;
	uint8_t E;
	uint8_t r;
	uint8_t F;
	uint8_t G;
	uint8_t Z;
	uint8_t X;
	uint8_t C;
	uint8_t V;
	uint8_t B;
	uint8_t CTRL_F;
	uint8_t CTRL_B;
	uint8_t Z_F;
	uint8_t Z_B;
	uint8_t FN_1;
	uint8_t FN_2;
	uint8_t TRIGGER;
	uint8_t PAUSE;	
}Key_Value_t;

typedef struct
{
	Key_Value_t Key_Value;
	Key_Value_t Key_Value_Last;
}Key_Scan_t;




extern Key_Scan_t Key_ScanValue;
extern fp32 FricSpeed;

#endif
