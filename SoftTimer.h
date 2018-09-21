#ifndef __TIMER_H_
#define __TIMER_H_

#include "config.h"

#define TIMER_ONESHOT		0
#define TIMER_PERIOD		1


#define sys_ticks()			SystemTicksCount()
#define tick_timeout(x)		((int)(sys_ticks() - x) > 0)


typedef void (*app_func)(void);

typedef struct
{
	uint32	ticks;          //周期
	uint32  overtime;       //时间到
	//void (*func)(void);
	app_func fun;
	uint8    run;        //0-stop,1-run
	uint8    flag;     //循环还是单次
} trIf_typedef;


typedef enum
{
    SRC_MAIN=0,
    SRC_MAX
}SRC_DEF;


extern unsigned long SystemTicksCount(void);

extern void trIf_Init(uint8 id,app_func fun);

extern void trIf_start( uint8 id, uint32 ticks, uint8 flag);

extern void trIf_stop( uint8 id);

extern void trIf_Execute(void);
extern uint32 SystemTicksCount(void);


#endif
