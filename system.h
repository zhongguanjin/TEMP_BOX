#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "config.h"


#define  SYSCLK_FREQ_16MHz 1

#define  _XTAL_FREQ   ((double)16000000)

#define delay_us(x) __delay_us(x)

#define delay_ms(x) __delay_ms(x)

extern void system_init(void);
extern void Init_MCU(void);

uint8 SYSCLK_Frequency;

#endif
