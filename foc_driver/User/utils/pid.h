#ifndef __PID_H__
#define __PID_H__


typedef struct
{
	volatile float kp;						// ����
	volatile float ki;						// ����
	volatile float kd;						// ΢��
	
	volatile float p_term;				// ������
	volatile float i_term;				// ������
	volatile float d_term;				// ΢����
	
	volatile float i_term_max;		// �����ۼ��޷�
	volatile float i_term_min;		// �����ۼ��޷�
	
	volatile float ref_value;			// Ŀ��ֵ
	volatile float fback_value;		// ʵ��ֵ
	
	volatile float error;					// ���
	volatile float pre_err;				// ��һ�����
	
	volatile float out_min;				// ����޷�
	volatile float out_max;				// ����޷�
	
	volatile float out_value;

}pid_para_t;


#endif /* __PID_H__ */















