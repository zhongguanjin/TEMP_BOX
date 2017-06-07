#ifndef  __MOTOR_H__
#define  __MOTOR_H__

#include "config.h"

typedef enum
{
    FLOW_MOTOR = 0,    //�������
    TEMP_MOTOR,        //�¶ȵ��
    MOTOR_MAX
}MOTOR_DEF;


typedef struct
{
	uint16  current_step;  //��ǰλ��
	uint16	target_step;   //Ŀ��λ��
	uint16  offset;      //ƫ��ֵ
	//uint16	dst;          //����
	//uint8	div;          //�������
    uint8   bDirCur :1;    // ����λ 1-���з���Ϊ����
    uint8   bRunFlg :1;     //���б��
    uint8   bInitok :1;   //��ʼ����ɱ�־
    uint8   bPluseFlg :1;  //�����ǣ�1-�����壬0-������
}PM_MOTOR;

PM_MOTOR PM[MOTOR_MAX];

typedef enum
{
    MICROSTEP_FULL = 0,
    MICROSTEP_1_2,
    MICROSTEP_1_4,
    MICROSTEP_1_16,
    MICROSTEP_MAX

}MOTOR_MICROSTEP;
#define MOTOR_SLEEP_ON     0       //��˯��
#define MOTOR_SLEEP_OFF    1       //�ر�˯��

#define MOTOR_DIR_POSITIVE    0    //����
#define MOTOR_DIR_NEGATIVE    1    //����

#define TEMP_MOTOR_RST 	 LATC4
#define TEMP_MOTOR_DIR 	 LATC0
#define TEMP_MOTOR_STEP  LATC3
#define TEMP_MOTOR_EN 	 LATA6
#define TEMP_MOTOR_MS1 	 LATA7
#define TEMP_MOTOR_MS2   LATC5


#define FLOW_MOTOR_RST 	 LATA0
#define FLOW_MOTOR_DIR 	 LATA2
#define FLOW_MOTOR_STEP  LATA1
#define FLOW_MOTOR_EN 	 LATC1
#define FLOW_MOTOR_MS1 	 LATA4
#define FLOW_MOTOR_MS2   LATA5

extern void motor_step_set(MOTOR_DEF mid, uint8 set_vaule);
extern void motor_dir_set( MOTOR_DEF mid, uint8 dir);
extern void motor_pluse_set(MOTOR_DEF mid, uint8 value);
extern void motor_stop(MOTOR_DEF mid);
extern void TaskMotorFun(void);
extern void Init_Motor(void);
extern void motor_run_pluse(MOTOR_DEF mid, uint16 pluse);
#endif