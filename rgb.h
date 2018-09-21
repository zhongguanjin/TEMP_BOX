#ifndef _RGB_H_
#define _RGB_H_

#include "config.h"

#define WHITE_CTL_PIN       LATG0

#define GREEN_CTL_PIN       LATF1

#define RED_CTL_PIN         LATC0

#define BLUE_CTL_PIN        LATF0

#define     RGB_CTL_OUT()              {TRISF0 = 0;TRISF1 = 0;TRISC0 = 0;TRISG0= 0;}

enum
{
    RED=1,
    GREEN,
    BLUE,
    ALL
};


extern void rgb_init(void);
extern void rgb_ctr(uint8 mid,uint8 st);

#endif

