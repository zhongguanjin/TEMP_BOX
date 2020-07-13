#include "pulse.h"
#include "math.h"

#include "dbg.h"

speedRampData  srd   = {ST_STOP,0,0,0,0,0};        // 加减速曲线变量

void motor_set_delay(int32 dat);
void motor_st_pro(void);



void motor_set_delay(int32 dat)
{

    PR6 = dat/2;
    TMR6IE = 1;
}



void motor_move(MOTOR_DEF mid,uint8 dir,uint32 pules)
{
    uint16 tim_count;
    uint32 max_s_lim;       // 达到最大速度时的步数
    uint32 accel_lim;       // 必须要开始减速的步数（如果加速没有达到最大速度）
    dbg("mp%d,d%d,p%l\r\n",mid,dir,pules);
    if(PM[mid].bRunFlg != ON) //不在运行
    {
        if(dir == CW)           //正向
        {
           motor_dir_set(TEMP_MOTOR, CW);
           PM[mid].bDirCur = CW;
        }
        else
        {
           motor_dir_set(TEMP_MOTOR, CCW);
           PM[mid].bDirCur = CCW;
        }
        srd.min_delay = (int32)A_T/V_max;                                      //设置最大速度极限, 计算得到min_delay用于定时器的计数器的值。

        srd.step_delay = (int32)((T_FREQ_148* sqrt(A_SQ / accel)));   //通过计算第一个(c0) 的步进延时来设定加速度

        max_s_lim = (uint32)(V_max*V_max/(A_SQ*accel));                //加速到最大速度所需的步数

        if(max_s_lim == 0)
        {
            max_s_lim = 1;
        }
        accel_lim = (uint32)(pules*decel/(accel+decel));                //计算多少步之后我们必须开始减速/

        if(accel_lim == 0)
        {
            accel_lim = 1;
        }
        // 使用限制条件我们可以计算出减速阶段步数
        if(accel_lim <= max_s_lim)
        {
           srd.decel_val = pules - accel_lim;
        }
        else
        {
           srd.decel_val = max_s_lim*accel/decel;
        }
        // 当只剩下一步我们必须减速
        if(srd.decel_val == 0)
        {
           srd.decel_val = -1;
        }
        // 计算开始减速时的步数
        srd.decel_start = pules - srd.decel_val;
        // 如果最大速度很慢，我们就不需要进行加速运动
        if(srd.step_delay <= srd.min_delay)
        {
           srd.step_delay = srd.min_delay;
           srd.pm_st = ST_RUN;
        }
        else
        {
           srd.pm_st = ST_ACCEL;
        }
        dbg("min_delay:%l,step_delay:%l,max_s_lim:%l\r\n",srd.min_delay,srd.step_delay,max_s_lim);
        dbg("decel_start:%l,decel_val:%l,accel_lim:%l\r\n",srd.decel_start, srd.decel_val,accel_lim);

        // 复位加速度计数值
        srd.accel_count = 0;
        motor_set_delay(srd.step_delay);
        PM[TEMP_MOTOR].set = pules;
        TEMP_MOTOR_RST = MOTOR_SLEEP_OFF;
        PM[TEMP_MOTOR].bRunFlg = 1;
        TEMP_MOTOR_RST = MOTOR_SLEEP_OFF;
    }
    else
    {
       dbg("err,motor run\r\n");
    }

}


void motor_st_pro(void)
{
     uint16 tim_count=0;
    // 保存新（下）一个延时周期
    uint16 new_step_delay=0;
    // 加速过程中最后一次延时（脉冲周期）.
     static uint16 last_accel_delay=0;
    // 总移动步数计数器
     static uint32 step_count = 0;
    // 记录new_step_delay中的余数，提高下一步计算的精度
     static int32 rest = 0;
    switch(srd.pm_st) // 加减速曲线阶段
    {
      case ST_STOP:
        step_count = 0;  // 清零步数计数器
        rest = 0;        // 清零余值
        break;
      case ST_ACCEL:
        step_count++;      // 步数加1
        srd.accel_count++; // 加速计数值加1
        new_step_delay = srd.step_delay - (((2 *srd.step_delay) + rest)/(4* srd.accel_count + 1));//计算新(下)一步脉冲周期(时间间隔)
        rest = ((2 * srd.step_delay)+rest)%(4 * srd.accel_count + 1);// 计算余数，下次计算补上余数，减少误差
        if(step_count >= srd.decel_start)// 检查是够应该开始减速
        {
          srd.accel_count = srd.decel_val; // 加速计数值为减速阶段计数值的初始值
          srd.pm_st = ST_DECEL;           // 下个脉冲进入减速阶段
        }
        else if(new_step_delay <= srd.min_delay) // 检查是否到达期望的最大速度
        {
          last_accel_delay = new_step_delay; // 保存加速过程中最后一次延时（脉冲周期）
          new_step_delay = srd.min_delay;    // 使用min_delay（对应最大速度speed）
          rest = 0;                          // 清零余值
          srd.pm_st = ST_RUN;               // 设置为匀速运行状态
        }
        break;
      case ST_RUN:
        step_count++;  // 步数加1
        new_step_delay = srd.min_delay;     // 使用min_delay（对应最大速度speed）
        if(step_count >= srd.decel_start)   // 需要开始减速
        {
          srd.accel_count = srd.decel_val;  // 减速步数做为加速计数值
          new_step_delay = last_accel_delay;// 加阶段最后的延时做为减速阶段的起始延时(脉冲周期)
          srd.pm_st = ST_DECEL;            // 状态改变为减速
        }
        break;
      case ST_DECEL:
        step_count++;  // 步数加1
        srd.accel_count--;
        new_step_delay = srd.step_delay + (((2 * srd.step_delay) + rest)/(4 * srd.accel_count + 1)); //计算新(下)一步脉冲周期(时间间隔)
        rest = ((2 * srd.step_delay)+rest)%(4 * srd.accel_count + 1);// 计算余数，下次计算补上余数，减少误差
        //检查是否为最后一步
        if(srd.accel_count == 0)
        {
            srd.pm_st = ST_STOP;
	        step_count = 0;  // 清零步数计数器
	        rest = 0;        // 清零余值
        }
        break;
    }
    srd.step_delay = new_step_delay; // 为下个(新的)延时(脉冲周期)赋值
}


// 步进电机中断函数
void TaskMotorISR(void)
{
    static uint8 ia=0;  //定时器使用翻转模式，需要进入两次中断才输出一个完整脉冲
    if(PM[TEMP_MOTOR].bRunFlg == ON) //运行
    {
        if(PM[TEMP_MOTOR].bPluseFlg!= 0) //低脉冲
        {
            motor_pulse_set(TEMP_MOTOR, OFF);
            ia++;
            PM[TEMP_MOTOR].bPluseFlg = 0;
        }
        else //高脉冲
	    {
            motor_pulse_set(TEMP_MOTOR, ON);
            PM[TEMP_MOTOR].bPluseFlg = 1;
            ia++;
        }
    }
    if(ia==2)
    {
        ia=0;
        if(PM[TEMP_MOTOR].cnt != PM[TEMP_MOTOR].set)
        {
            PM[TEMP_MOTOR].cnt++;
            motor_st_pro();
            if(srd.pm_st == ST_STOP)
            {
                motor_stop(TEMP_MOTOR);
                TMR6IE = 0;
            }
            else
            {
                //dbg("%d",srd.step_delay);
                motor_set_delay(srd.step_delay);
            }
        }
        else
        {
            motor_stop(TEMP_MOTOR);
            TMR6IE = 0;
        }
    }
}
