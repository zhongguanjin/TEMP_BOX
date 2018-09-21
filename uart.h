#ifndef __UART_H__
#define __UART_H__

#include "config.h"




extern void Init_UART1(uint32 baud);
extern void usart1_send_byte(char ch);

extern void Init_UART2(uint32 baud);
extern void usart2_send_byte(char ch);



#endif


