#include "motor.h"

typedef struct
{
	uint16  current_pos;  //当前位置
	uint16	target_pos;   //目标位置
	uint16  offset;      //偏移值
	uint16	dst;          //距离
	uint8	div;          //电机速率
    uint8   bDirCur :1;    // 方向位 1-运行方向为逆向
    uint8   bInPlace :1;   //到位标志 1-到位
    uint8   bStopFlg :1;   //停止标记
    uint8   bPluseFlg :1;  //脉冲标记，1-高脉冲，0-低脉冲
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
    TRISA = 0B00001000     //RA3为输入，其他为输出
    TRISC = 0B11000100     //RC6，7，2为输入 其他为输出
    //温度电机配置
    TEMP_MOTOR_RST   = 1;
    TEMP_MOTOR_DIR   = 1;
    TEMP_MOTOR_STEP  = 1;
    TEMP_MOTOR_EN    = 0;     //0有效
    motor_step_set(TEMP_MOTOR,MICROSTEP_1_16); //1// 1/16步

    //流量电机配置
    FLOW_MOTOR_RST 	 = 1;
    FLOW_MOTOR_DIR 	 = 1;
    FLOW_MOTOR_STEP  = 1;
    FLOW_MOTOR_EN 	 = 0;    //0有效
    motor_step_set(FLOW_MOTOR,MICROSTEP_1_16);  // 1/16步
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
     if(PM[mid].bStopFlg == 0) //先判断是否停止
     {
      	if(mid == FLOW_MOTOR)  //流量电机
    	{
    		if(dir != 0)
    		{
    			PM[mid].bDirCur = 1;  //反向
                FLOW_MOTOR_DIR  = 1;
    		}
    		else
    		{
    			PM[mid].bDirCur = 0;  //正向
                FLOW_MOTOR_DIR  = 0;
    		}
    		FLOW_MOTOR_RST = 1;
    	}
        if(mid == TEMP_MOTOR)  //温度电机
            {
                if(dir != 0)
                {
                	PM[mid].bDirCur = 1;  //反向
                    TEMP_MOTOR_DIR  = 1;
                }
                else
                {
                	PM[mid].bDirCur = 0;  //正向
                    TEMP_MOTOR_DIR  = 0;
                }
                TEMP_MOTOR_RST = 1;
            }
	}
}

/*****************************************************************************
 函 数 名  : 电机脉冲设定
 功能描述  :
 输入参数  : 无
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月26日 星期五
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
		FLOW_MOTOR_RST = 0;
	}
	else if(mid == TEMP_MOTOR)
	{
		TEMP_MOTOR_RST = 0;
	}
}


// 步进电机中断函数
void TaskMotorFun(void)
{
   uint8 index;
   for(index = 0; index < MOTOR_MAX; index++)
	{
        if(PM[index].bPluseFlg != 0)
        {
            motor_pluse_set(index, OFF);
            PM[index].bPluseFlg = 0;
        } else
	    {   //正向
            if(PM[index].current_step < PM[index].target_step)
            {
                motor_dir_set(index, 0)
                PM[index].current_step++;
                motor_pluse_set(index, ON)
                PM[index].bPluseFlg = 1;
            }
            else if(PM[index].current_pos > PM[index].target_pos)
            { // 反向
                motor_dir_set(index, 1)
                PM[index].current_step++;
                motor_pluse_set(index, ON)
                PM[index].bPluseFlg = 1;
            }
        }
    }
}






























