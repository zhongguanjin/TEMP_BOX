
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

#define SHOWER_8837_IN2_PIN				LATE3
#define SHOWER_8837_IN1_PIN				LATE4


#define DRAIN_8800_DIR               LATC4
#define DRAIN_8800_MODE              LATC5
#define DRAIN_8800_SLEEP             LATC3
#define DRAIN_8800_EN                LATC2


#define     DRAIN_8800_OUT()              {TRISC2 = 0;TRISC3 = 0;TRISC4 = 0;TRISC5 = 0;}


#define     TAP_8837_OUT()              {TRISD5 = 0;TRISD6 = 0;}

#define     SHOWER_8837_OUT()           { TRISE3 = 0;TRISE4 = 0;}

extern void drv_8837_config(void);
extern void drv8837_ctr(DRV8837_DEF mid,uint8 state);
extern uint8 get_8837_state(DRV8837_DEF mid);

extern void drv8800_open(void);
extern void drv8800_close(void);
extern void drv8800_stop(void);


#endif
