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
	// ��������λ
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
 �� �� ��  : com_send_dat
 ��������  : ���ڷ���һ������
 �������  : void *p     --ָ�򴮿����ݽṹ���ָ��
             uint8 len   --���ݳ���
             uint8 cnt   --�ظ�����
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2017��6��30�� ������
    ��    ��   : zgj
    �޸�����   : �����ɺ���

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
 �� �� ��  : CRC8_SUM
 ��������  : CRCУ�麯��
 �������  : void *p
             uint8 len
 �������  : crc8        --check sum
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2017��6��30�� ������
    ��    ��   : zgj
    �޸�����   : �����ɺ���

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
