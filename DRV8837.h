
#ifndef _DRV8837_H_
#define _DRV8837_H_

#include"config.h"

typedef enum
{
  DRV8837_TAP =0,
  DRV8837_SHOWER,
  DRV8837_MAX
}DRV8837_DEF;


uint8 tap_state;
uint8 shower_state;


#define TAP_8837_IN2_PIN				    LATD5
#define TAP_8837_IN1_PIN				    LATD6

#define SHOWER_8837_IN2_PIN				LATE7
#define SHOWER_8837_IN1_PIN				LATD0



#define     TAP_8837_OUT()              {TRISD5 = 0;TRISD6 = 0;}

#define     SHOWER_8837_OUT()           { TRISD0 = 0;TRISE7 = 0;}

extern void drv_8837_config(void);
extern void drv8837_ctr(DRV8837_DEF mid,uint8 state);
extern uint8 get_8837_state(DRV8837_DEF mid);


#endif
