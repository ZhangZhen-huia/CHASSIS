#include "vofa_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "chassis_power_control.h"
#include "chassis_task.h"
#include "referee.h"


const DebugData *shoot_debug;
const DebugData *chassis_debug;
const DebugData *chassis_power_debug;

float Vofa_Data[6];

void vofa_justfloat(UART_HandleTypeDef* huart, float* fdata, uint16_t count);
void VofaInit(void);


void vofa_task(void const * argument)
{
	#ifndef Vofa_Debug
	vTaskDelete(NULL);
	#endif
	VofaInit();
	
	while(1)
	{
		#ifdef Power_Debug
		Vofa_Send(chassis_power_debug->data1, chassis_power_debug->data2, chassis_power_debug->data3,chassis_power_debug->data4, chassis_power_debug->data5, chassis_power_debug->data6);
		#endif
		
		#ifdef SHOOT_DEBUG
		Vofa_Send(Referee_System.ext_shoot_data.initial_speed,23,25,0,0,0);
		#endif
		
		#ifdef CHASSIS_DEBUG
		Vofa_Send(chassis_debug->data1,chassis_debug->data2,chassis_debug->data3,chassis_debug->data4,chassis_debug->data5,chassis_debug->data6);
		#endif
		osDelay(10);
	}
	

}




const DebugData* get_chassis_Debug(void);
const DebugData* get_chassis_power(void);

void VofaInit(void)
{

	chassis_debug = get_chassis_Debug();
	chassis_power_debug = get_chassis_power();
}









void Vofa_Send(float data1, float data2, float data3, float data4, float data5, float data6)
{
  //数据绑定	
	Vofa_Data[0] = data1;
	Vofa_Data[1] = data2;
	Vofa_Data[2] = data3;
	Vofa_Data[3] = data4;	
	Vofa_Data[4] = data5;
	Vofa_Data[5] = data6;
	//数据发送
	vofa_justfloat(&huart1,Vofa_Data,6);
}


void vofa_justfloat(UART_HandleTypeDef* huart, float* fdata, uint16_t count)
{
	// 最多支持长度为6的float数组
	uint8_t data[40];
	int i = 0;
	for (i=0;i<count*4;i++)
	data[i] = ((uint8_t*)fdata)[i];
	data[i++] = 0;
	data[i++] = 0;
	data[i++] = 0x80;
	data[i++] = 0x7f;
	HAL_UART_Transmit(huart, data, count*4+4, 1000);
}






