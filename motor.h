#ifndef  __MOTOR_H__
#define  __MOTOR_H__

#include "config.h"

typedef enum
{
    FLOW_MOTOR = 0,    //流量电机
    TEMP_MOTOR,        //温度电机
    MOTOR_MAX
}MOTOR_DEF;



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
#endif