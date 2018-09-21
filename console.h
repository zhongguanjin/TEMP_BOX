#ifndef _CONSOLE_H_
#define _CONSOLE_H_

typedef int (*consoleCallback)(char * buf, int len);
extern consoleCallback console_cb;

extern void com2_rxDeal(void);

#endif
