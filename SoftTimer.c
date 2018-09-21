#include "SoftTimer.h"



trIf_typedef TimerList[SRC_MAX];



void trIf_Init(uint8 id,app_func fun)
{
    if(id >= SRC_MAX)
    {
        return ;
    }
    else
    {
        TimerList[id].ticks= 0;
        TimerList[id].overtime =0;
        TimerList[id].fun= fun;
        TimerList[id].run= 0;
    }
}


void trIf_stop(uint8 id)
{
    TimerList[id].overtime =0;
    TimerList[id].ticks= 0;
    TimerList[id].run= 0;
}

void trIf_start(uint8 id, uint32 ticks,uint8 flag)
{
    if(id >= SRC_MAX)
    {
        return ;
    }
    else
    {
        TimerList[id].ticks= ticks;
        TimerList[id].overtime = sys_ticks() + ticks;
        TimerList[id].flag = flag;
        TimerList[id].run= 1;

	}
}

void trIf_Execute(void)
{
	uint8 id = 0;
	for(id = 0; id < SRC_MAX; id++)
	{
	    if(TimerList[id].run ==1) //开定时器
	    {
            if(tick_timeout(TimerList[id].overtime))  //时间到
            {
                if(TimerList[id].flag == TIMER_PERIOD)//周期
                {
                     TimerList[id].overtime = sys_ticks() + TimerList[id].ticks;
                }
                else
                {
                     trIf_stop(id);
                }
                TimerList[id].fun();
            }
        }
	}
}



uint32 SystemTicksCount(void)
{
   static uint32 tick_time1=0;
   tick_time1++;

   return tick_time1;
}











































































