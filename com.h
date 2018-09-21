#ifndef _COM_H_
#define _COM_H_

#include "config.h"

#define	CONSOLE_RX_BUF_LEN		64	// must 2**n
#define	CONSOLE_RX_BUF_MASK	(CONSOLE_RX_BUF_LEN-1)

#define RX_START_ST		0
#define RX_SPARE1_ST	1
#define RX_SPARE2_ST	2
#define RX_DATA_ST		3
#define RX_CHK_ST			4
#define RX_END_ST			5
#define RX_END_ST2			6


typedef struct {
	 uint8	in;
	 uint8	out;
	char	buf[CONSOLE_RX_BUF_LEN];
} comCycle_t;

typedef struct {
	comCycle_t	tx;
	comCycle_t	rx;
} tComBuf_t;

typedef enum {
	com1 = 0,
	com2,
} COM_DEF;

extern tComBuf_t comBuf[2];



void com_cycleReset(comCycle_t * pBuf);

extern uint32 com_rxLeft(COM_DEF COMx);
extern int com_getch(COM_DEF COMx, char * p);
extern void com_init(COM_DEF COMx,uint32 baud);
extern void USART1_RXHandler(uint8 dat);
extern void USART2_RXHandler(uint8 dat);



#endif

