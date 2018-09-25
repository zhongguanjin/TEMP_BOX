#ifndef __TIMER_H__
#define __TIMER_H__


#include "config.h"



extern void Init_TMR0(void);

extern void Init_TMR6(void);

extern uint32 SystemTicksCount(void);

#define sys_ticks()			SystemTicksCount()
#define tick_timeout(x)		((int)(sys_ticks() - x) > 0)


#endif
