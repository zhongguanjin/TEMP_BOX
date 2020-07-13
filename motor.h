#ifndef  __MOTOR_H__
#define  __MOTOR_H__

#include "config.h"

typedef enum
{
    FLOW_MOTOR = 0,    //流量电机
    TEMP_MOTOR,        //温度电机
    MOTOR_MAX
}MOTOR_DEF;


typedef struct
{
	uint32  cnt;        //当前步数
	uint32	set;        //目标位置
	uint32  offset;     //偏移值
	uint32   dst;        //距离零点的位置
	union {
	    struct{
            uint8   bDirCur :1;    // 方向位 0-正向,1-逆向
            uint8   bRunFlg :1;     //运行标记
            uint8   bInitok :1;   //初始化完成标志
            uint8   bPluseFlg :1;  //脉冲标记，1-高脉冲，0-低脉冲
        };
        uint8 bitval;
    };
}PM_MOTOR;

PM_MOTOR PM[MOTOR_MAX];



//const uint8 speed[20] ={250,240,230,220,210,200,190,180,170,160,150,140,130,120,110,100,90,80,70,60};

typedef enum
{
    MICROSTEP_FULL = 0,
    MICROSTEP_1_2,
    MICROSTEP_1_4,
    MICROSTEP_1_16,
    MICROSTEP_MAX
}MOTOR_MICROSTEP;

#define MOTOR_SLEEP_ON     0       //打开睡眠
#define MOTOR_SLEEP_OFF    1       //关闭睡眠

#define CW    0    //正向
#define CCW    1    //反向


#define TEMP_MOTOR_STEP  LATG4
#define TEMP_MOTOR_DIR   LATG3
#define TEMP_MOTOR_EN 	 LATF3
#define TEMP_MOTOR_MS1 	 LATF4
#define TEMP_MOTOR_MS2   LATF5
#define TEMP_MOTOR_RST 	 LATF6


#define FLOW_MOTOR_STEP  LATA1
#define FLOW_MOTOR_DIR 	 LATA2
#define FLOW_MOTOR_EN 	 LATC1
#define FLOW_MOTOR_MS1 	 LATA4
#define FLOW_MOTOR_MS2   LATA5
#define FLOW_MOTOR_RST 	 LATA0

#define A4982_RST_TEMP_OUTPUT 	{TRISF6 = 0;ANSF6 = 0; LATF6 = 0;} //RF6
#define A4982_DIR_TEMP_OUTPUT 	{TRISG3 = 0;ANSG3 = 0; LATG3 = 0;} //RG3
#define A4982_STEP_TEMP_OUTPUT 	{TRISG4 = 0;ANSG4 = 0; LATG4 = 0;} //RG4
#define A4982_EN_TEMP_OUTPUT 	{TRISF3 = 0;ANSF3 = 0; LATF3 = 0;} //RF3
#define A4982_MS1_TEMP_OUTPUT 	{TRISF4 = 0;ANSF4 = 0; LATF4 = 0;} //RF4
#define A4982_MS2_TEMP_OUTPUT 	{TRISF5 = 0;ANSF5 = 0; LATF5 = 0;} //RF5
/*========== 流量步进电机控制口配置=============*/
#define A4982_RST_FLOW_OUTPUT 	{ANSA0 = 0; TRISA0 = 0; LATA0 = 0;} //RA0/AN0
#define A4982_DIR_FLOW_OUTPUT 	{ANSA2 = 0; TRISA2 = 0; LATA2 = 0;} //RA2/AN2
#define A4982_STEP_FLOW_OUTPUT 	{ANSA1 = 0; TRISA1 = 0; LATA1 = 0;} //RA1/AN1
#define A4982_EN_FLOW_OUTPUT 	{TRISC1 = 0; LATC1 = 0;}            //RC1
#define A4982_MS1_FLOW_OUTPUT 	{TRISA4 = 0; LATA4 = 0;}            //RA4
#define A4982_MS2_FLOW_OUTPUT 	{ANSA5 = 0; TRISA5 = 0; LATA5 = 0;} //RA5/AN4

#define A4982_FLOW_IOS_OUTPUT() {A4982_RST_FLOW_OUTPUT; A4982_DIR_FLOW_OUTPUT; A4982_STEP_FLOW_OUTPUT;A4982_EN_FLOW_OUTPUT; A4982_MS1_FLOW_OUTPUT; A4982_MS2_FLOW_OUTPUT;}

#define A4982_TEMP_IOS_OUTPUT() {A4982_RST_TEMP_OUTPUT; A4982_DIR_TEMP_OUTPUT; A4982_STEP_TEMP_OUTPUT;A4982_EN_TEMP_OUTPUT; A4982_MS1_TEMP_OUTPUT; A4982_MS2_TEMP_OUTPUT;}

extern void motor_step_set(MOTOR_DEF mid, uint8 set_vaule);
extern void motor_dir_set( MOTOR_DEF mid, uint8 dir);
extern void motor_pulse_set(MOTOR_DEF mid, uint8 pulse);
extern void motor_stop(MOTOR_DEF mid);

extern void TaskMotorFun(void);
extern uint32 motor_getPulse(MOTOR_DEF mid);
extern void Init_Motor(void);
extern void motor_run_pulse(MOTOR_DEF mid,uint16 dir,uint32 pulse);
extern void motor_speed_set(uint8 gear);
extern void motor_setPlace(MOTOR_DEF mid,uint32 place);
#endif