#include "com.h"

#include "uart.h"
#include "system.h"


tComBuf_t comBuf[2];



void com_cycleReset(comCycle_t * pBuf)
{
	pBuf->in = 0;
	pBuf->out = 0;
}

/*****************************************************************************
 函 数 名  : CRC8_SUM
 功能描述  : CRC校验函数
 输入参数  : void *p
             uint8 len
 输出参数  : crc8        --check sum
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年6月30日 星期五
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
uint8 CRC8_SUM(void *p,uint8 len)
{
    uint8 crc8 = 0;
    uint8 *temp =p;
    for(uint8 i=0;i<len;i++)
    {
        crc8 ^=*temp;
        temp++;
    }
    return crc8;
}

void com_init(COM_DEF COMx,uint32 baud)
{
	// 缓冲区复位
	com_cycleReset(&(comBuf[COMx].tx));
	com_cycleReset(&(comBuf[COMx].rx));
    if(COMx == com1)
    {
        M485_SCIO_OUT;
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

void com_send_485(COM_DEF COMx,uint8 *buf, uint32 len)
{
    uint8 i=0;
    CTR_485(ON);
    delay_ms(2);
    for(i=0;i<len;i++)
    {
        usart1_send_byte(buf[i]);
    }
    delay_ms(1);
    CTR_485(OFF);
}

void USART1_RXHandler(uint8 dat)
{
    comBuf[com1].rx.buf[comBuf[com1].rx.in++&CONSOLE_RX_BUF_MASK] =dat;
}
void USART2_RXHandler(uint8 dat)
{
    comBuf[com2].rx.buf[comBuf[com2].rx.in++&CONSOLE_RX_BUF_MASK] =dat;
}
