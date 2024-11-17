#ifndef USER_TASK_H
#define USER_TASK_H


#include "main.h"

typedef struct
{
	int16_t vx_set;
	int16_t vy_set;
	int16_t wz_set;
	uint8_t rc_sl;
	uint8_t rc_sr;
	
}rc_data_t;








void get_rc_data(rc_data_t *rc_data,uint8_t *buf);





extern rc_data_t rc_data;




#endif


