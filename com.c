#include "com.h"

#include "uart.h"


tComBuf_t comBuf[2];



void com_cycleReset(comCycle_t * pBuf)
{
	pBuf->in = 0;
	pBuf->out = 0;
}

void com_init(COM_DEF COMx,uint32 baud)
{
	// »º³åÇø¸´Î»
	com_cycleReset(&(comBuf[COMx].tx));
	com_cycleReset(&(comBuf[COMx].rx));
    if(COMx == com1)
    {
        Init_UART1(baud);
    }
    if(COMx == com2)
    {
        Init_UART2(baud);
    }
}


uint32 com_rxLeft(COM_DEF COMx)
{
	return (uint32)(comBuf[COMx].rx.in - comBuf[COMx].rx.out);
}

int com_getch(COM_DEF COMx, char * p)
{
	comCycle_t * pcbuf = &(comBuf[COMx].rx);

	if (pcbuf->in != pcbuf->out)
	{
		*p = pcbuf->buf[pcbuf->out& CONSOLE_RX_BUF_MASK];
		pcbuf->out++;
		return OK;
	}
	else
	{
		return ERR;
	}
}


void USART1_RXHandler(uint8 dat)
{
    comBuf[com1].rx.buf[comBuf[com1].rx.in++&CONSOLE_RX_BUF_MASK] =dat;
}
void USART2_RXHandler(uint8 dat)
{
    comBuf[com2].rx.buf[comBuf[com2].rx.in++&CONSOLE_RX_BUF_MASK] =dat;
}
