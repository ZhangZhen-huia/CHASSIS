#ifndef PID_H
#define PID_H
#include "stm32f4xx_hal.h"
#include "main.h"

typedef enum errorType_e
{
    PID_ERROR_NONE = 0x00U,
    Motor_Blocked = 0x01U
} ErrorType_e;

typedef struct
{
    uint64_t ERRORCount;
    ErrorType_e ERRORType;
} PID_ErrorHandler_t;

typedef enum pid_Improvement_e
{
    NONE = 0X00,                        //0000 0000
    Integral_Limit = 0x01,              //0000 0001
    Derivative_On_Measurement = 0x02,   //0000 0010
    Trapezoid_Intergral = 0x04,         //0000 0100
    Proportional_On_Measurement = 0x08, //0000 1000
    OutputFilter = 0x10,                //0001 0000
    ChangingIntegralRate = 0x20,        //0010 0000
    DerivativeFilter = 0x40,            //0100 0000
    ErrorHandle = 0x80,                 //1000 0000
} PID_Improvement_e;



//enum PID_MODE
//{
//    PID_POSITION = 0,
//    PID_DELTA,
//};

enum DATA_MODE
{
    DATA_GYRO = 0,
    DATA_NORMAL,
};

typedef struct 
{
    //uint8_t mode;
		uint8_t data_mode;
    //PID ������
    float Kp;
    float Ki;
    float Kd;

    float max_out;  //������
    float max_iout; //���������

    float set;
    float fdb;

    float out;
    float Pout;
    float Iout;
    float Dout;
    float Dbuf[3];  //΢���� 0���� 1��һ�� 2���ϴ�
    float error[3]; //����� 0���� 1��һ�� 2���ϴ�
		fp32 error_all;
		float K_ff_static;
		float K_ff_dynamic;
		float last_aim;
		float feedforward_static_out;
		float feedforward_dynamic_out;
		
		uint8_t function;
		PID_ErrorHandler_t ERRORHandler;
} pid_type_def;


///*********ģ��pid����*/
//typedef struct
//{
//		float SetPoint;			//�趨Ŀ��ֵ
//	
//		float ActualValue;  //ʵ��ֵ

//    float DeadZone;
//		
//		float LastError;		//ǰ�����
//		float PreError;			//��ǰ���
//		float SumError;			//�������
//	
//		float IMax;					//��������
//		
//		float POut;					//�������
//		float IOut;					//�������
//		float DOut;					//΢�����
//	  float DOut_last;    //��һ��΢�����
//		float OutMax;       //�޷�
//	  float Out;          //�����
//		float Out_last;     //��һ�����
//		
//		float I_U;          //���ٻ�������
//		float I_L;          //���ٻ�������
//		
//		float RC_DM;        //΢�������˲�ϵ��
//		float RC_DF;        //����ȫ΢���˲�ϵ��
//	
//	  float Kp0;          //PID��ֵ
//	  float Ki0;
//  	float Kd0;
//	
//	  float dKp;          //PID�仯��
//	  float dKi;
//  	float dKd;
//	
//    float stair ;	      //��̬�����ݶ�   //0.25f
//	  float Kp_stair;                      //0.015f
//	  float Ki_stair;                      //0.0005f
//	  float Kd_stair;                      //0.001f
//	  
//}FuzzyPID;


void PID_init(pid_type_def *pid,uint8_t data_mode, const float PID[3], float max_out, float max_iout,uint8_t function);


extern float PID_calc(pid_type_def *pid, float ref, float set);

extern void PID_clear(pid_type_def *pid);


fp32 PID_Calc_Ecd(pid_type_def *pid, fp32 ref, fp32 set,uint16_t ecd_range);
static fp32 ecd_zero(uint16_t ecd, uint16_t offset_ecd, uint16_t ecd_range);
void K_FF_init(pid_type_def *pid,const float PID[3], float max_out, float max_iout,float K_ff_static,float K_ff_dynamic,uint8_t function);
float K_FF_Cal(pid_type_def *pid, float ref, float set);

#endif
