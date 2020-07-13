#ifndef _PULSE_H_
#define _PULSE_H_

#include "config.h"
#include "motor.h"

#define   PI  3.14
// 速度最大值由驱动器和电机决定，有些最大是1800，有些可以达到4000
uint32 V_max  = 10;   //(25/8*PI);         // 速度 单位为1 rad/sec   v_max = α*ft/c_min = (2PI/3200)*1Mhz / 120 ≈16(0.1 rad/s)
// 加速度和减速度选取一般根据实际需要，值越大速度变化越快，加减速阶段比较抖动
// 所以加速度和减速度值一般是在实际应用中多尝试出来的结果

uint32 accel = 157;//(50*PI);         // 加速度 单位为1rad/sec^2
uint32 decel = 157;//(50*PI);         // 减速度 单位为1rad/sec^2

#define  SYS_FREQ           ((double)16000000)

#define  TIMER_FREQ   		(SYS_FREQ/4/4/10) //10分频,4分频 16M/4*(1/4)/10 =1M/10=100k


#define	 MICRO_STEP    		16 			// 电机细分数
#define  SPR          		(200*MICRO_STEP) //转一圈所需步数

#define  ALPHA    		    ((float)(2*PI/SPR))       			// α= 2*pi/spr

#define  A_T                ((float)(ALPHA*TIMER_FREQ))		// α*ft


#define T_FREQ_148          ((float)(TIMER_FREQ*0.65)) 		// 0.677为误差修正值

#define A_SQ                ((float)(2*ALPHA))   //


/* 类型定义 ------------------------------------------------------------------*/
typedef struct {
   uint8  pm_st ;  // 电机旋转状态
   int32  step_delay;  // 下个脉冲周期（时间间隔），启动时为加速度
   uint32 decel_start; // 启动减速位置
   int32  decel_val;   // 减速阶段步数
   int32  min_delay;   // 最小脉冲周期(最大速度，即匀速段速度对应的计数值)
   int32  accel_count; // 加减速阶段计数值
}speedRampData;








enum
{
    ST_STOP =0, //停止
    ST_ACCEL,   //加速
    ST_DECEL,   //减速
    ST_RUN,     //匀速
    ST_MAX
};





extern void TaskMotorISR(void);
extern void motor_move(MOTOR_DEF mid,uint8 dir,uint32 pules);



#endif

