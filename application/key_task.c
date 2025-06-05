#include "key_task.h"
#include "remote_control.h"
#include "communicate_task.h"


void Key_Scan(void);
Key_Scan_t Key_ScanValue;
void FricSpeeed_Control(void);

fp32 FricSpeed = 18.0f;

//uint8_t Key_Function(uint16_t key);

void key_task(void const * argument)
{
	
	while(1)
	{
		/*-- 5msɨ��һ�ΰ���(��ΪCan�������ӳ٣����������Ƶ��,Ȼ��ʹ��Key_Down) --*/
		Key_Scan();
		FricSpeeed_Control();

		osDelay(5);
    
	}
}

//uint8_t Key_Function(uint16_t key)
//{
//	static uint16_t num = 0;
//	if(key & rc_ctrl.key.v)
//	{
//		osDelay(10);
//		while((key & rc_ctrl.key.v) == 1);
//		return 1;
//	}
//	else return 0;
//}


void Key_Scan(void)
{
	uint32_t Key_Temp,Key_Down,Key_Up;
	static uint32_t Key_Last;
	
	Key_Temp = ((uint32_t)gimbal_data.rc_data.rc_key_v )+((uint32_t)gimbal_data.ImghandleKey<<16);
	Key_Down = Key_Temp & (Key_Last ^ Key_Temp);//������㣬��ͬΪ0����ͬΪ1     
	Key_Up 	= ~Key_Temp & (Key_Last ^ Key_Temp);//������㣬��ͬΪ0����ͬΪ1 
	Key_Last = Key_Temp;
	
	Key_ScanValue.Key_Value_Last = Key_ScanValue.Key_Value;
	
	if(Key_Down == KEY_PRESSED_OFFSET_Q)
	{
		Key_ScanValue.Key_Value.Q = 1;
	}
	else
		Key_ScanValue.Key_Value.Q = 0;

		if(Key_Down == KEY_PRESSED_OFFSET_E)
	{
		Key_ScanValue.Key_Value.E = 1;
	}
	else
		Key_ScanValue.Key_Value.E = 0;
	
		if(Key_Down == KEY_PRESSED_OFFSET_R)
	{
		Key_ScanValue.Key_Value.r = 1;
	}
	else
		Key_ScanValue.Key_Value.r = 0;
	
		if(Key_Down == KEY_PRESSED_OFFSET_F)
	{
		Key_ScanValue.Key_Value.F = 1;
	}
	else
		Key_ScanValue.Key_Value.F = 0;
	
		if(Key_Down == KEY_PRESSED_OFFSET_G)
	{
		Key_ScanValue.Key_Value.G = 1;
	}
	else
		Key_ScanValue.Key_Value.G = 0;
	
		if(Key_Down == KEY_PRESSED_OFFSET_Z)
	{
		Key_ScanValue.Key_Value.Z = 1;
	}
	else
		Key_ScanValue.Key_Value.Z = 0;
	
		if(Key_Down == KEY_PRESSED_OFFSET_X)
	{
		Key_ScanValue.Key_Value.X = 1;
	}
	else
		Key_ScanValue.Key_Value.X = 0;
	
		if(Key_Down == KEY_PRESSED_OFFSET_C)
	{
		Key_ScanValue.Key_Value.C = 1;
	}
	else
		Key_ScanValue.Key_Value.C = 0;
	
		if(Key_Down == KEY_PRESSED_OFFSET_V)
	{
		Key_ScanValue.Key_Value.V = 1;
	}
	else
		Key_ScanValue.Key_Value.V = 0;
	
		if(Key_Down == KEY_PRESSED_OFFSET_B)
	{
		Key_ScanValue.Key_Value.B = 1;
	}
	else
		Key_ScanValue.Key_Value.B = 0;
	
	if(Key_Down == KEY_PRESSED_OFFSET_SHIFT)
	{
		Key_ScanValue.Key_Value.SHIFT = 1;
	}
	else
		Key_ScanValue.Key_Value.SHIFT = 0;
	
	if(Key_Down == KEY_PRESSED_OFFSET_CTRL)
	{
		Key_ScanValue.Key_Value.CTRL = 1;
	}
	else
		Key_ScanValue.Key_Value.CTRL = 0;
	
	if(Key_Up == IMG_TRANSFER_KEY_FN1)
	{
		Key_ScanValue.Key_Value.FN_1 = 1;
	}
	else
		Key_ScanValue.Key_Value.FN_1 = 0;
	if(Key_Up == IMG_TRANSFER_KEY_FN2)
	{
		Key_ScanValue.Key_Value.FN_2 = 1;
	}
	else
		Key_ScanValue.Key_Value.FN_2 = 0;	
	if(Key_Up == IMG_TRANSFER_KEY_PAUSE)
	{
		Key_ScanValue.Key_Value.PAUSE = 1;
	}
	else
		Key_ScanValue.Key_Value.PAUSE = 0;	
	if(Key_Up == IMG_TRANSFER_KEY_TRIGGER)
	{
		Key_ScanValue.Key_Value.TRIGGER = 1;
	}
	else
		Key_ScanValue.Key_Value.TRIGGER = 0;		
	
	Key_ScanValue.Key_Value.CTRL_F = Key_ScanValue.Key_Value.CTRL & Key_ScanValue.Key_Value.F;
	Key_ScanValue.Key_Value.CTRL_B = Key_ScanValue.Key_Value.CTRL & Key_ScanValue.Key_Value.B;
	Key_ScanValue.Key_Value.Z_F = Key_ScanValue.Key_Value.Z & Key_ScanValue.Key_Value.F;
	Key_ScanValue.Key_Value.Z_B = Key_ScanValue.Key_Value.Z & Key_ScanValue.Key_Value.B;
	

}



void FricSpeeed_Control(void)
{
	if(gimbal_data.FricState)
	{
		if(Key_ScanValue.Key_Value.C)
			FricSpeed-=0.5f;
		if(Key_ScanValue.Key_Value.B)
			FricSpeed+=0.5f;
		
		if(FricSpeed > 21.5f)
			FricSpeed = 21.5f;
		if(FricSpeed < 15.5f )
			FricSpeed = 15.5f;
	}

		
}
