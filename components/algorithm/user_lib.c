#include "user_lib.h"
#include "arm_math.h"

/**
  * @brief          б��������ʼ��(�������ʱ�����ܽ�0/1ֱ�ӱ仯�ٶȼ�������Ϊб��)
  * @author         RM
  * @param[in]      б�������ṹ��
  * @param[in]      �����ʱ�䣬��λ s
  * @param[in]      ���ֵ
  * @param[in]      ��Сֵ
  * @retval         ���ؿ�
  */
void ramp_init(ramp_function_source_t *ramp_source_type, fp32 frame_period, fp32 max, fp32 min)
{
    ramp_source_type->frame_period = frame_period;
    ramp_source_type->max_value = max;
    ramp_source_type->min_value = min;
    ramp_source_type->input = 0.0f;
    ramp_source_type->out = 0.0f;
}

/**
  * @brief          б���������㣬���������ֵ���е��ӣ� ���뵥λΪ /s ��һ������������ֵ
  * @author         RM
  * @param[in]      б�������ṹ��
  * @param[in]      ����ֵ
  * @param[in]      �˲�����
  * @retval         ���ؿ�
  */
void ramp_calc(ramp_function_source_t *ramp_source_type, fp32 input)
{
    ramp_source_type->input = input;
    ramp_source_type->out += ramp_source_type->input * ramp_source_type->frame_period;
    if (ramp_source_type->out > ramp_source_type->max_value)
    {
        ramp_source_type->out = ramp_source_type->max_value;
    }
    else if (ramp_source_type->out < ramp_source_type->min_value)
    {
        ramp_source_type->out = ramp_source_type->min_value;
    }
}
/*
	б���������ڼ�һ�κ���
	�������һ��ϵ�����������ڣ������ۼ���Ϊ���
	ֱ�����������С��ֵ
*/


/**
  * @brief          һ�׵�ͨ�˲���ʼ��
  * @author         RM
  * @param[in]      һ�׵�ͨ�˲��ṹ��
  * @param[in]      �����ʱ�䣬��λ s
  * @param[in]      �˲�����
  * @retval         ���ؿ�
  */
void first_order_filter_init(first_order_filter_type_t *first_order_filter_type, fp32 frame_period, const fp32 num[1])
{
    first_order_filter_type->frame_period = frame_period;
    first_order_filter_type->num[0] = num[0];
    first_order_filter_type->input = 0.0f;
    first_order_filter_type->out = 0.0f;
}

/**
  * @brief          һ�׵�ͨ�˲�����
  * @author         RM
  * @param[in]      һ�׵�ͨ�˲��ṹ��
  * @param[in]      �����ʱ�䣬��λ s
  * @retval         ���ؿ�
  */
void first_order_filter_cali(first_order_filter_type_t *first_order_filter_type, fp32 input)
{
    first_order_filter_type->input = input;
    first_order_filter_type->out =
    first_order_filter_type->num[0] / (first_order_filter_type->num[0] + first_order_filter_type->frame_period) * \
			first_order_filter_type->out + first_order_filter_type->frame_period / (first_order_filter_type->num[0] + first_order_filter_type->frame_period) * first_order_filter_type->input;
}
//out = ��һ�ε�out * ������ʱ��/������ʱ��+����ʱ�䣩�� + ���ε�input*������ʱ��/������ʱ��+����ʱ�䣩��
/* 
һ�׵�ͨ�˲�����һ�ּ򵥵��˲���������˥����Ƶ�źųɷ֣�ͬʱ������Ƶ�ɷ֡�����ѧģ�Ϳ��������²�ַ��̱�ʾ��
	y[n]=��*y[n-1]+(1-��)*x[n]
	y[n] �ǵ�ǰʱ�̵����ֵ
	y[n-1] ����һ��ʱ�̵����ֵ	
	x[n] �ǵ�ǰʱ�̵�����ֵ
	�� ���˲�ϵ�����������˲�����ƽ���̶ȣ��� ��ֵ�� 0 �� 1 ֮�䣬Խ�ӽ� 1���˲�������Ӧ�ٶ�Խ�����˲�Ч��Խǿ
	�� = num[0]/(num[0]+frame_period)
	1-�� = frame_period/(num[0]+frame_period)
	num[0]��ͨ����ʾ�˲�����ʱ�䳣�������˲����Ľ�ֹƵ����ء��ϴ��num[0]ֵ�ᵼ���˲����������źŵı仯��Ӧ�������˲�Ч����ǿ��
	frame_period���������ڣ���ʾ���β���֮���ʱ�������������Ƶ���й�
*/


//��������
void abs_limit(fp32 *num, fp32 Limit)
{
    if (*num > Limit)
    {
        *num = Limit;
    }
    else if (*num < -Limit)
    {
        *num = -Limit;
    }
}

//�жϷ���λ
fp32 sign(fp32 value)
{
    if (value >= 0.0f)
    {
        return 1.0f;
    }
    else
    {
        return -1.0f;
    }
}

//��������
fp32 fp32_deadline(fp32 Value, fp32 minValue, fp32 maxValue)
{
    if (Value < maxValue && Value > minValue)
    {
        Value = 0.0f;
    }
    return Value;
}

//int26����
int16_t int16_deadline(int16_t Value, int16_t minValue, int16_t maxValue)
{
    if (Value < maxValue && Value > minValue)
    {
        Value = 0;
    }
    return Value;
}

//�޷�����
fp32 fp32_constrain(fp32 Value, fp32 minValue, fp32 maxValue)
{
    if (Value < minValue)
        return minValue;
    else if (Value > maxValue)
        return maxValue;
    else
        return Value;
}

//�޷�����
int16_t int16_constrain(int16_t Value, int16_t minValue, int16_t maxValue)
{
    if (Value < minValue)
        return minValue;
    else if (Value > maxValue)
        return maxValue;
    else
        return Value;
}

//ѭ���޷�����
fp32 loop_fp32_constrain(fp32 Input, fp32 minValue, fp32 maxValue)
{
    if (maxValue < minValue)
    {
        return Input;
    }

    if (Input > maxValue)
    {
        fp32 len = maxValue - minValue;
        while (Input > maxValue)
        {
            Input -= len;
        }
    }
    else if (Input < minValue)
    {
        fp32 len = maxValue - minValue;
        while (Input < minValue)
        {
            Input += len;
        }
    }
    return Input;
}

//���ȸ�ʽ��Ϊ-PI~PI

//�Ƕȸ�ʽ��Ϊ-180~180
fp32 theta_format(fp32 Ang)
{
    return loop_fp32_constrain(Ang, -180.0f, 180.0f);
}


/**
	* @brief ���Ƕȿ�����0 - max
	* @param  angle,max һ��Ϊ���Ƕ�һ��Ϊ��ǰ�Ƕ�
  * @retval ������ĽǶ�ֵ
  * @attention	�����Ǿ��������������0-max֮���
  */
float Angle_Limit (float angle ,float max)
{
		if(angle > max)
			angle -= max;
		if(angle < 0)
			angle += max; 
		return angle;
}

fp32 Math_Abs(fp32 x)
{
    return ((x > 0) ? x : -x);
}

