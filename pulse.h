#ifndef _PULSE_H_
#define _PULSE_H_

#include "config.h"
#include "motor.h"

#define   PI  3.14159
// �ٶ����ֵ���������͵����������Щ�����1800����Щ���Դﵽ4000
 uint32 V_max  = 10;   //(25/8*PI);         // �ٶ� ��λΪrad/sec
// ���ٶȺͼ��ٶ�ѡȡһ�����ʵ����Ҫ��ֵԽ���ٶȱ仯Խ�죬�Ӽ��ٽ׶αȽ϶���
// ���Լ��ٶȺͼ��ٶ�ֵһ������ʵ��Ӧ���жೢ�Գ����Ľ��
 uint32 accel = 157;//(50*PI);         // ���ٶ� ��λΪrad/sec^2
 uint32 decel = 157;//(50*PI);         // ���ٶ� ��λΪrad/sec^2

#define  SYS_FREQ           ((double)16000000)

#define  TIMER_FREQ   		(SYS_FREQ/4/40) //10��Ƶ,4��Ƶ 16M/4*(1/40)
#define	 MICRO_STEP    		16 			// ���ϸ����
#define  SPR          		(200*MICRO_STEP) //תһȦ���貽��

#define  ALPHA    		    ((float)(2*PI/SPR))       			// ��= 2*pi/spr

#define  A_T                ((float)(ALPHA*TIMER_FREQ))		// ��*ft


#define T_FREQ_148          ((float)(TIMER_FREQ*0.676)) 		// 0.676Ϊ�������ֵ

#define A_SQ                ((float)(2*ALPHA))   //


/* ���Ͷ��� ------------------------------------------------------------------*/
typedef struct {
   uint8  pm_st ;  // �����ת״̬
   int32  step_delay;  // �¸��������ڣ�ʱ������������ʱΪ���ٶ�
   uint32 decel_start; // ��������λ��
   int32  decel_val;   // ���ٽ׶β���
   int32  min_delay;   // ��С��������(����ٶȣ������ٶ��ٶ�)
   int32  accel_count; // �Ӽ��ٽ׶μ���ֵ
}speedRampData;








enum
{
    ST_STOP =0, //ֹͣ
    ST_ACCEL,   //����
    ST_DECEL,   //����
    ST_RUN,     //����
    ST_MAX
};





extern void TaskMotorISR(void);
extern void motor_move(MOTOR_DEF mid,uint8 dir,uint32 pules);



#endif
