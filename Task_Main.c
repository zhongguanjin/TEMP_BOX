
/*******************************
********时间片轮询法的架构******
********************************/
#include "Task_Main.h"
#include "adc.h"
#include "uart.h"
#include "stdio.h"
#include "motor.h"
#include "dbg.h"
#include "stdlib.h"
//#include "pulse.h"
#include "timer.h"

#include "SoftTimer.h"


#define tab_len  29

/*0-空闲 1-初始化 2,工作*/
enum {
	MODE_IDLE = 0,
	MODE_INIT,
	MODE_DBG,
	MODE_WORK,
	MODE_MAX
};

enum
{
    ST_INIT_INIT = 0,
    ST_INIT_TEMP_RUN,   //流量电机
    ST_INIT_FLOW_RUN,
    ST_INIT_END,
    ST_INIT_MAX
};

enum
{
    ST_DBG_INIT = 0,
    ST_DBG_RUN,
    ST_DBG_MAX
};


volatile uint8 dev_mode = 0;

volatile uint8 dev_state = 0;

uint32 overticks=0;
uint8 msg_id = SRC_MAIN;


void Taskpro(void);
void TasktrIf(void);




void if_init_state(void);
int if_init_timer(void *pArg);

int if_dbg_timer(void *pArg);

void if_dbg_state(void);

void SetOverTicks(uint32 ticks);





// 定义结构体变量
static TASK_COMPONENTS TaskComps[] =
{
    {0, 100, 100, Taskpro},
    {0, 10, 10, TasktrIf},

};
// 任务清单
typedef enum _TASK_LIST
{

    TAST_PRO,             //
    TASK_TRIF,
    TASKS_MAX                // 总的可供分配的定时任务数目
} TASK_LIST;



uint16 const temp_tab[tab_len] =    //表格是以5度为一步，即-30, -25, - 20.....
 {
   32  ,52     ,77    ,108    ,144    ,185    ,234    ,285    ,339    ,396    ,       //-30.c  ...15.c
   454     ,512    ,567    ,620    ,669    ,713    ,753    ,782    ,812    ,837    ,   //20.c   ...  65.c
   859     ,878    ,894    ,908    ,920    ,930    ,939    ,946    ,953    ,           //70.c   ...  110.c

 };

/*****************************************************************************
 函 数 名  : get_temperature
 功能描述  : 获取温度函数
 输入参数  : 无
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月25日 星期四
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
float get_temperature(uint8 ad_channel)
{
    uint16 temp_ad = 0;
    uint8 left =0, right = tab_len-1,mid;
    float temp;
    mid = (left+right)/2;
    Sel_Adc_Channel(ad_channel);   //选择ad通道
    temp_ad =read_tem_adc();
    if(temp_ad >temp_tab[right])
    return 110;
    if(temp_ad <temp_tab[left])
    return 0;
    while(((right-left)!=1)&&(temp_tab[mid]!=temp_ad))
    {
        if(temp_tab[mid]< temp_ad)
        {
            left = mid;
        }
        else if(temp_tab[mid] > temp_ad)
        {
            right = mid;
        }
        mid = (left+right)/2;
    }
    temp = ((float)((temp_ad-temp_tab[left])*5)/(temp_tab[right]-temp_tab[left]))
            +(mid*5 -30);// 阀值转化成温度  放大10倍
    return temp;
}

float get_voltage(uint8 ad_channel)
{
    float Ad_Voltage;
    Sel_Adc_Channel(ad_channel);
    Ad_Voltage = (float)(read_tem_adc()*5)/1023*251/51;
    return Ad_Voltage;
}



void SetOverTicks(uint32 ticks)
{
    overticks= sys_ticks() + ticks;
}


void if_init_state(void)
{
    //TIMER_TABLE* ptnode;
     switch ( dev_state)
     {
         case ST_INIT_INIT:
             {
              //ptnode=CreatTimer(1000, PERIOIC, if_init_timer, NULL);
                  trIf_Init(SRC_MAIN, if_init_timer);
                  trIf_start(SRC_MAIN, 1000, TIMER_PERIOD);// 1000ms
             }
            break;
        case ST_INIT_TEMP_RUN:
            {
                motor_run_pulse(TEMP_MOTOR,0,3200);
                SetOverTicks(10000);//5s
            }
            break;
        case ST_INIT_FLOW_RUN:
            {
                motor_run_pulse(FLOW_MOTOR,0,3200);
                 SetOverTicks(10000);//5s
            }
            break;
        case ST_INIT_END:
            {
                app_stateSet(ST_DBG_INIT);
                app_modeSet(MODE_DBG);
                set_msgid(SRC_MAIN);
                //KillTimer(ptnode);
                trIf_stop(SRC_MAIN);
            }
            break;
        default:
            {

            }
            break;
    }
}


int if_init_timer(void *pArg)
{
    switch ( dev_state)
    {
        case ST_INIT_INIT:
            {
                set_msgid(SRC_MAIN);
                app_stateSet(ST_INIT_TEMP_RUN);
            }
            break;
        case ST_INIT_TEMP_RUN:
            {
                if(PM[TEMP_MOTOR].bRunFlg == 0) //stop
                {
                     motor_run_pulse(TEMP_MOTOR,1,3200);
                     set_msgid(SRC_MAIN);
                     app_stateSet(ST_INIT_FLOW_RUN);
                }
                if(tick_timeout(overticks))
                {
                    dbg("over time\r\n");
                    set_msgid(SRC_MAIN);
                    app_stateSet(ST_INIT_FLOW_RUN);
                }
            }
            break;
        case ST_INIT_FLOW_RUN:
            {
                if(PM[FLOW_MOTOR].bRunFlg == 0) //stop
                {
                     set_msgid(SRC_MAIN);
                     motor_run_pulse(FLOW_MOTOR,1,3200);
                     app_stateSet(ST_INIT_END);
                }
                if(tick_timeout(overticks))
                {
                    dbg("over time\r\n");
                    set_msgid(SRC_MAIN);
                    app_stateSet(ST_INIT_END);
                }
            }
            break;
        case ST_INIT_END:
            {

            }
            break;
        default:
            break;
    }
    return 0;
}


uint32 place[4]={800,1600,2400,3200};
uint8  PlaceNo;


int if_dbg_timer(void *pArg)
{
	if(PM[TEMP_MOTOR].bRunFlg == 0) //stop
	{	// stop
		if(dev_state == ST_DBG_INIT)
		{
			app_stateSet(ST_DBG_RUN);
			set_msgid(SRC_MAIN);
		}
		else
		{
			app_stateSet(ST_DBG_INIT);
			set_msgid(SRC_MAIN);
		}
	}
	    return 0;
}



void if_dbg_state(void)
{
    //TIMER_TABLE* ptnode;
     switch (dev_state)
     {
         case ST_DBG_INIT:
             {
                //ptnode=CreatTimer(1000, PERIOIC, if_dbg_timer, NULL);
                trIf_Init(SRC_MAIN, if_dbg_timer);
                trIf_start(SRC_MAIN, 2000, TIMER_PERIOD);// 1000ms
             }
            break;
        case ST_DBG_RUN:
            {
                uint32 dstPls;
                uint8 randPlace=PlaceNo;
            	while(randPlace == PlaceNo)
				{
					randPlace = (uint8)(rand() & 3) + 1;
				}
                dbg("Place:%d\r\n",randPlace);
			    PlaceNo = randPlace;
			    dstPls = place[randPlace-1];
				motor_setPlace(TEMP_MOTOR,dstPls);
				motor_setPlace(FLOW_MOTOR,dstPls);
            }
            break;
        default:
            {
                //KillTimer(ptnode);
                trIf_stop(SRC_MAIN);
            }
            break;
    }

}



void TasktrIf(void)
{
    trIf_Execute();
}

/*****************************************************************************
 函 数 名  : Taskpro
 功能描述  : 进程任务
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月25日 星期四
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
void Taskpro(void)
{
    if(get_msgid() == SRC_MAIN)
    {
       switch (dev_mode)
       {
           case MODE_IDLE:
               {
                    set_msgid(SRC_MAX);
                    dbg("idle\r\n");
               }
               break;
           case MODE_INIT:
               {
                   set_msgid(SRC_MAX);
                   if_init_state();
               }
               break;
           case MODE_DBG:
               {
                    set_msgid(SRC_MAX);
                    if_dbg_state();
               }
               break;
           case MODE_WORK:
               {
                    set_msgid(SRC_MAX);
               }
               break;
           default:
               set_msgid(SRC_MAX);
               break;
       }
    }
}





uint8 get_msgid(void)
{

    return msg_id;
}

void set_msgid(uint8 id)
{
    msg_id = id;
}

void app_modeSet(uint8 mode)
{
     dev_mode = mode;
     dbg("mode :%d\r\n",dev_mode);
}
void app_stateSet(uint8 state)
{
     dev_state= state;
     dbg("state :%d\r\n",dev_state);
}




/*****************************************************************************
 函 数 名  : TaskRemarks
 功能描述  : 任务标记处理函数
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月24日 星期三
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/

void TaskRemarks(void)
{
    uint8 i;
    for (i=0; i<TASKS_MAX; i++)                                 // 逐个任务时间处理
    {
         if (TaskComps[i].Timer)                                // 时间不为0
        {
            TaskComps[i].Timer--;                                // 减去一个节拍
            if (TaskComps[i].Timer == 0)                            // 时间减完了
            {
                 TaskComps[i].Timer = TaskComps[i].ItvTime;       // 恢复计时器值，从新下一次
                 TaskComps[i].Run = ON;                             // 任务可以运行
            }
        }
   }
}

/*****************************************************************************
 函 数 名  : TaskProcess
 功能描述  : 任务进程函数
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月24日 星期三
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
void TaskProcess(void)
{
    uint8 i;
    for (i=0; i<TASKS_MAX; i++)                // 逐个任务时间处理
    {
        if (TaskComps[i].Run)                 // 时间不为0
        {
             TaskComps[i].TaskHook();        // 运行任务
             TaskComps[i].Run = 0;          // 标志清0
        }
    }
}

