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
    //PID 三参数
    float Kp;
    float Ki;
    float Kd;

    float max_out;  //最大输出
    float max_iout; //最大积分输出

    float set;
    float fdb;

    float out;
    float Pout;
    float Iout;
    float Dout;
    float Dbuf[3];  //微分项 0最新 1上一次 2上上次
    float error[3]; //误差项 0最新 1上一次 2上上次
		fp32 error_all;
		float K_ff_static;
		float K_ff_dynamic;
		float last_aim;
		float feedforward_static_out;
		float feedforward_dynamic_out;
		
		uint8_t function;
		PID_ErrorHandler_t ERRORHandler;
} pid_type_def;


///*********模糊pid部分*/
//typedef struct
//{
//		float SetPoint;			//设定目标值
//	
//		float ActualValue;  //实际值

//    float DeadZone;
//		
//		float LastError;		//前次误差
//		float PreError;			//当前误差
//		float SumError;			//积分误差
//	
//		float IMax;					//积分限制
//		
//		float POut;					//比例输出
//		float IOut;					//积分输出
//		float DOut;					//微分输出
//	  float DOut_last;    //上一次微分输出
//		float OutMax;       //限幅
//	  float Out;          //总输出
//		float Out_last;     //上一次输出
//		
//		float I_U;          //变速积分上限
//		float I_L;          //变速积分下限
//		
//		float RC_DM;        //微分先行滤波系数
//		float RC_DF;        //不完全微分滤波系数
//	
//	  float Kp0;          //PID初值
//	  float Ki0;
//  	float Kd0;
//	
//	  float dKp;          //PID变化量
//	  float dKi;
//  	float dKd;
//	
//    float stair ;	      //动态调整梯度   //0.25f
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
