#ifndef __MAIN_H__
#define __MAIN_H__

#include "config.h"

__CONFIG(0x0EA4);       //������
__CONFIG(0x3EFF);


#define TMR0_VALUE				0x82

typedef int (*consoleCallback)(char * buf, int len);
extern consoleCallback console_cb;



/*��������  */
void Init_Sys(void);

#endif

