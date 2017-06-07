#include "motor.h"

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
    //IO配置为输出
    TRISA = 0B00001000;     //RA3为输入，其他为输出
    TRISC = 0B11000100;     //RC6，7，2为输入 其他为输出
    //流量电机配置
    FLOW_MOTOR_EN = 0;                           //电机使能
    motor_step_set(FLOW_MOTOR,MICROSTEP_1_16);  // 1/16步
    PM[FLOW_MOTOR].current_step = 0;
    motor_stop(FLOW_MOTOR);
    //温度电机配置
    TEMP_MOTOR_EN = 0;                           //电机使能
    motor_step_set(TEMP_MOTOR,MICROSTEP_1_16); //1                                        // 1/16步
    PM[TEMP_MOTOR].current_step = 0;
    motor_stop(TEMP_MOTOR);
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
    // if(PM[mid].bRunFlg == OFF) //先判断是否停止
    // {
      	if(mid == FLOW_MOTOR)  //流量电机
    	{
    		if(dir != MOTOR_DIR_POSITIVE)
    		{
    			PM[mid].bDirCur = MOTOR_DIR_NEGATIVE;  //反向
                FLOW_MOTOR_DIR  = MOTOR_DIR_NEGATIVE;
    		}
    		else
    		{
    			PM[mid].bDirCur = MOTOR_DIR_POSITIVE;  //正向
                FLOW_MOTOR_DIR  = MOTOR_DIR_POSITIVE;
    		}
    	}
        if(mid == TEMP_MOTOR)  //温度电机
            {
                if(dir != MOTOR_DIR_POSITIVE)
                {
                	PM[mid].bDirCur = MOTOR_DIR_NEGATIVE;  //反向
                    TEMP_MOTOR_DIR  = MOTOR_DIR_NEGATIVE;
                }
                else
                {
                	PM[mid].bDirCur = MOTOR_DIR_POSITIVE;  //正向
                    TEMP_MOTOR_DIR  = MOTOR_DIR_POSITIVE;
                }
            }
	//}
}

/*****************************************************************************
 函 数 名  : motor_pluse_set
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
void motor_pluse_set(MOTOR_DEF mid, uint8 value)
{
	if(mid == FLOW_MOTOR)
	{
		FLOW_MOTOR_STEP = value;
	}
	else if(mid == TEMP_MOTOR)
	{
		FLOW_MOTOR_STEP = value;
	}
}

/*****************************************************************************
 函 数 名  : motor_run_pluse
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
void motor_run_pluse(MOTOR_DEF mid, uint16 pluse)
{

  if(PM[mid].bRunFlg == 0)
  {
    if(mid == FLOW_MOTOR)
    {
    	PM[FLOW_MOTOR].target_step = pluse;
    	FLOW_MOTOR_RST = MOTOR_SLEEP_OFF;          //启动电机
    	PM[FLOW_MOTOR].bRunFlg = 1;
    }
    else if(mid == TEMP_MOTOR)
    {
    	PM[TEMP_MOTOR].target_step = pluse;
    	TEMP_MOTOR_RST = MOTOR_SLEEP_OFF;
    	PM[FLOW_MOTOR].bRunFlg = 1;
    }
  }

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
}


// 步进电机中断函数
void TaskMotorFun(void)
{
   MOTOR_DEF mid;
   for(mid = 0; mid < MOTOR_MAX; mid++)
	{
        if(PM[mid].bPluseFlg != 0)
        {
            motor_pluse_set(mid, OFF);
            PM[mid].bPluseFlg = 0;
        }
        else
	    {
	        //正向
            if(PM[mid].current_step < PM[mid].target_step)
            {
                motor_dir_set(mid, MOTOR_DIR_POSITIVE);
                motor_pluse_set(mid, ON);
                PM[mid].bPluseFlg = 1;
                PM[mid].current_step++;
            }
            else if(PM[mid].current_step > PM[mid].target_step)
                { // 反向
                    motor_dir_set(mid, MOTOR_DIR_NEGATIVE);
                    motor_pluse_set(mid, ON);
                    PM[mid].bPluseFlg = 1;
                    PM[mid].current_step--;
                }
                else
                {
                   //PM[mid].current_step = PM[mid].target_step;
                   PM[mid].bPluseFlg = 0;
                   motor_stop(mid);
                }
        }
    }
}






























