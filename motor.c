#include "motor.h"
#include "dbg.h"


/*****************************************************************************
 函 数 名  : Init_Motor
 功能描述  : 电机初始化函数
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年6月1日 星期四
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
void Init_Motor(void)
{
    A4982_FLOW_IOS_OUTPUT();
    A4982_TEMP_IOS_OUTPUT();
    motor_speed_set(GEAR_50);
    //温度电机配置
    TEMP_MOTOR_EN = 0;                           //电机使能
    motor_step_set(TEMP_MOTOR,MICROSTEP_1_16); //1                                        // 1/16步
    PM[TEMP_MOTOR].cnt = 0;
    motor_stop(TEMP_MOTOR);
    //流量电机配置
    FLOW_MOTOR_EN = 0;                           //电机使能
    motor_step_set(FLOW_MOTOR,MICROSTEP_1_16); //1                                        // 1/16步
    PM[FLOW_MOTOR].cnt = 0;
    motor_stop(FLOW_MOTOR);
}
/*****************************************************************************
 函 数 名  : motor_microstep_set
 功能描述  : 电机步长设置
 输入参数  : MOTOR_DEF mid
             uint8 microstep_set_vaule
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年6月1日 星期四
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
void motor_step_set(MOTOR_DEF mid, uint8 set_vaule)
{
    if(mid == FLOW_MOTOR)
    {
        //满步(M1=0, M2=0)
        if(set_vaule == MICROSTEP_FULL)
        {
            FLOW_MOTOR_MS1 	 = 0;
            FLOW_MOTOR_MS2   = 0;
        }
        //1/2步(M1=1, M2=0)
        if(set_vaule == MICROSTEP_1_2)
        {
            FLOW_MOTOR_MS1 	 = 1;
            FLOW_MOTOR_MS2   = 0;
        }
        //1/4步(M1=0, M2=1)
        if(set_vaule == MICROSTEP_1_4)
        {
            FLOW_MOTOR_MS1 	 = 0;
            FLOW_MOTOR_MS2   = 1;
        }
        //1/16步(M1=1, M2=1)
        if(set_vaule == MICROSTEP_1_16)
        {
            FLOW_MOTOR_MS1   = 1;
            FLOW_MOTOR_MS2   = 1;
        }
   }
   if(mid == TEMP_MOTOR)
    {
        //满步(M1=0, M2=0)
        if(set_vaule == MICROSTEP_FULL)
        {
            TEMP_MOTOR_MS1 	 = 0;
            TEMP_MOTOR_MS2   = 0;
        }
        //1/2步(M1=1, M2=0)
        if(set_vaule == MICROSTEP_1_2)
        {
            TEMP_MOTOR_MS1 	 = 1;
            TEMP_MOTOR_MS2   = 0;
        }
        //1/4步(M1=0, M2=1)
        if(set_vaule == MICROSTEP_1_4)
        {
            TEMP_MOTOR_MS1 	 = 0;
            TEMP_MOTOR_MS2   = 1;
        }
        //1/16步(M1=1, M2=1)
        if(set_vaule == MICROSTEP_1_16)
        {
            TEMP_MOTOR_MS1   = 1;
            TEMP_MOTOR_MS2   = 1;
        }
    }
}
/*****************************************************************************
 函 数 名  : motor_dir_set
 功能描述  : 步进电机方向设置函数
 输入参数  : MOTOR_DEF
             uint8 dir
 输出参数  : 无
 返 回 值  : static
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月26日 星期五
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
 void motor_dir_set( MOTOR_DEF mid, uint8 dir)
{
  	if(mid == FLOW_MOTOR)  //流量电机
	{
		if(dir != CW)
		{
			PM[mid].bDirCur = CCW;  //反向
            FLOW_MOTOR_DIR  = CCW;
		}
		else
		{
			PM[mid].bDirCur = CW;  //正向
            FLOW_MOTOR_DIR  = CW;
		}
	}
    if(mid == TEMP_MOTOR)  //温度电机
    {
        if(dir != CW)
        {
        	PM[mid].bDirCur = CCW;  //反向
            TEMP_MOTOR_DIR  = CCW;
        }
        else
        {
        	PM[mid].bDirCur = CW;  //正向
            TEMP_MOTOR_DIR  = CW;
        }
    }
}

/*****************************************************************************
 函 数 名  : motor_pulse_set
 功能描述  : 电机脉冲高低电平设置
 输入参数  : MOTOR_DEF mid
             uint8 value
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年6月1日 星期四
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
void motor_pulse_set(MOTOR_DEF mid, uint8 value)
{
	if(mid == FLOW_MOTOR)
	{
		FLOW_MOTOR_STEP = value;
	}
	else if(mid == TEMP_MOTOR)
	{
		TEMP_MOTOR_STEP = value;
	}
}

void motor_setPlace(MOTOR_DEF mid,uint32 place)
{
    if(PM[mid].bRunFlg != ON) //不在运行
    {
        if(mid <MOTOR_MAX)
        {
            uint32 pulse;
            if(place > PM[mid].offset)
            {
                pulse = place-PM[mid].offset;
                motor_run_pulse(mid, 0, pulse);
            }
            else
            {
                pulse = PM[mid].offset-place;
                motor_run_pulse(mid, 1, pulse);
            }
        }
    }
}
/*****************************************************************************
 函 数 名  : motor_run_pulse
 功能描述  : 电机运行脉冲步数
 输入参数  : MOTOR_DEF mid
             uint8 pluse
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年6月1日 星期四
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
void motor_run_pulse(MOTOR_DEF mid,uint16 dir,uint32 pulse)
{
    dbg("mp%d,d%d,p%l\r\n",mid,dir,pulse);
    if(PM[mid].bRunFlg != ON) //不在运行
    {
        if(mid == FLOW_MOTOR)
        {
            if(dir == 0) //正向
            {
                motor_dir_set(mid, CW);
                PM[mid].bDirCur =0;
            }
            else
            {
                motor_dir_set(mid, CCW);
                PM[mid].bDirCur =1;
            }
        	PM[FLOW_MOTOR].set = pulse;
        	FLOW_MOTOR_RST = MOTOR_SLEEP_OFF;          //启动电机
        	PM[FLOW_MOTOR].bRunFlg = 1;
        }
        else if(mid == TEMP_MOTOR)
        {
            if(dir == 0) //正向
            {
                motor_dir_set(mid, CW);
                PM[mid].bDirCur =0;
            }
            else
            {
                motor_dir_set(mid, CCW);
                PM[mid].bDirCur =1;
            }
        	PM[TEMP_MOTOR].set = pulse;
        	TEMP_MOTOR_RST = MOTOR_SLEEP_OFF;
        	PM[TEMP_MOTOR].bRunFlg = 1;
        }
        dbg("d:%l,o:%l,s:%l\r\n",PM[mid].dst,PM[mid].offset,PM[mid].set);
    }
    else
    {
       dbg("err,motor run\r\n");
    }
}


uint32 motor_getPulse(MOTOR_DEF mid)
{
	uint32 pls;
	if(PM[mid].set != PM[mid].cnt)
	{
		if(PM[mid].bDirCur != 0) //逆向
		{
			pls = PM[mid].dst + PM[mid].set -PM[mid].cnt;
		}
		else  //正向
		{
			pls = PM[mid].dst + PM[mid].cnt -PM[mid].set;
		}
	}
	else //到位了
	{
		pls = PM[mid].offset;
	}
    dbg("d:%l,o:%l,s:%l\r\n",PM[mid].dst,PM[mid].offset,PM[mid].set);
	return pls;
}

/*****************************************************************************
 函 数 名  : motor_stop
 功能描述  : 步进电机停止
 输入参数  : uint8 num
 输出参数  : 无
 返 回 值  : static
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月26日 星期五
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
 void motor_stop(MOTOR_DEF mid)
{
    if(mid == FLOW_MOTOR)
	{
		FLOW_MOTOR_RST = MOTOR_SLEEP_ON; //关闭电机
		PM[FLOW_MOTOR].bRunFlg = OFF;

	}
	else if(mid == TEMP_MOTOR)
	{
		TEMP_MOTOR_RST = MOTOR_SLEEP_ON;
		PM[TEMP_MOTOR].bRunFlg = OFF;
	}
    if(PM[mid].bDirCur == 0) //正向
    {
         PM[mid].dst = PM[mid].dst+PM[mid].cnt;
    }
    else
    {
         PM[mid].dst = PM[mid].dst-PM[mid].cnt;
    }
    PM[mid].offset =PM[mid].dst;
    PM[mid].cnt=0;
    PM[mid].set=0;
}

void motor_speed_set(uint8 gear)
{
    PR6 = speed[gear];
    dbg("speed:%d\r\n",speed[gear]);
    TMR6IE =1;
}

// 步进电机中断函数
void TaskMotorFun(void)
{
   MOTOR_DEF mid;
   //GIE = 0;
   for(mid = 0; mid < MOTOR_MAX; mid++)
	{
	    if(PM[mid].bRunFlg == ON) //运行
	    {
            if(PM[mid].bPluseFlg != 0) //低脉冲
            {
                motor_pulse_set(mid, OFF);
                PM[mid].bPluseFlg = 0;
            }
            else //高脉冲
    	    {
                if(PM[mid].cnt != PM[mid].set)
                {
                    motor_pulse_set(mid, ON);
                    PM[mid].bPluseFlg = 1;
                    PM[mid].cnt++;
                }
                else
                {
                   motor_stop(mid);
                   PM[mid].bPluseFlg = 0;
                }
            }
        }
    }
    //GIE=1;
}






























