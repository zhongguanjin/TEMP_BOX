#include "com.h"


//#include "led.h"


tComBuf_t comBuf[2];



void com_cycleReset(comCycle_t * pBuf)
{
	pBuf->in = 0;
	pBuf->out = 0;
}

void com_init(COM_DEF COMx,uint32 baud)
{
	// 缓冲区复位
	com_cycleReset(&(comBuf[COMx].tx));
	com_cycleReset(&(comBuf[COMx].rx));
    if(COMx == com1)
    {
        M485_IO_OUT;
        Init_UART1(baud);
    }
    if(COMx == com2)
    {
        Init_UART2(baud);
    }
}
void check_uart(COM_DEF COMx)
{
    if(COMx == com1)
    {
        if ((RC1STAbits.FERR == 1) || (RC1STAbits.OERR == 1))
        {
            static uint8 error_rc = 0;
            error_rc = RCREG;
            NOP();
            error_rc = RCREG;
            RC1STAbits.CREN = 0;
            NOP();
            RC1STAbits.CREN = 1;
            com_init(com1,19200);
        }
    }
    else if(COMx == com2)
    {
        if ((RC2STAbits.FERR == 1) || (RC2STAbits.OERR == 1))
        {
            static uint8 error_rc = 0;
            error_rc = RC2REG;
            NOP();
            error_rc = RC2REG;
            RC2STAbits.CREN = 0;
            NOP();
            RC2STAbits.CREN = 1;
            com_init(com2,115200);
        }
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
/*****************************************************************************
 函 数 名  : com_send_dat
 功能描述  : 串口发送一串数据
 输入参数  : void *p     --指向串口数据结构体的指针
             uint8 len   --数据长度
             uint8 cnt   --重复次数
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年6月30日 星期五
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/

void com_send_dat(COM_DEF comx,void *p,uint8 len)
{
    if(comx == com1)
    {
        uint8 *temp =p;
        M485_EN_H;
        delay_us(500);
        for(uint8 j=0;j<len;j++)
        {
            usart1_send_byte(*temp);
            if(j<len-1) //j<32
            {
                temp++;
            }
        }
        delay_ms(5);
        M485_EN_L;
    }
    else if(comx == com2)
    {
        uint8 *temp =p;
        for(uint8 j=0;j<len;j++)
        {
            usart2_send_byte(*temp);
            if(j<len-1) //j<32
            {
                temp++;
            }
        }
    }
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

void USART1_RXHandler(uint8 dat)
{
    comBuf[com1].rx.buf[comBuf[com1].rx.in++&CONSOLE_RX_BUF_MASK] =dat;
}
void USART2_RXHandler(uint8 dat)
{
    comBuf[com2].rx.buf[comBuf[com2].rx.in++&CONSOLE_RX_BUF_MASK] =dat;
}
