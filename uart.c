
#include "uart.h"


#define 	PIC_CLK 		    16000000//4000000                    // 时钟频率
#define     BAUD                9600                                                 //波特率
#define     SPBRGx_VAL          ((PIC_CLK/(16UL * BAUD) -1))
#define     BUSY                1
#define     FREE                0

#define RX_PIN  TRISC7  //定义数据通讯端口
#define TX_PIN  TRISC6

static	uint8 usart1_tx_data[64];			//发射的数据数组
static	uint8	usart1_tx_len;		//发射数据的长度
volatile static  bit  tx1_sign;

/*****************************************************************************
 函 数 名  : usart1_init
 功能描述  : USART1初始化，波特率9600
 输入参数  : 无
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2016年5月13日
 作    者  : SJY
 修改内容  : 新生成函数

*****************************************************************************/

void Init_UART1(void)
{
    RX_PIN = 1;
    TX_PIN = 1;
	//配置发送寄存器
    TX1STAbits.TX9D=0;      //无奇偶校验位
    TX1STAbits.TRMT=0;      //发送移位寄存器状态位  0-TSR 已满
    TX1STAbits.BRGH = 1;
    TX1STAbits.SYNC=0;
    TX1STAbits.TXEN=1;
    TX1STAbits.TX9=0;       //发送8位
	//配置接收寄存器
    RC1STAbits.RX9D=0;      //无奇偶校验位
    RC1STAbits.OERR=0;
    RC1STAbits.FERR=0;
    RC1STAbits.ADDEN=0;
    RC1STAbits.CREN=1;      //连续接收使能位
    // RCSTA2bits.SREN=0;
    RC1STAbits.RX9=0;       //接收8位
    RC1STAbits.SPEN=1;      //串口使能位

    SPBRG = SPBRGx_VAL;      //波特率对应初值
    //RCIE = 1;                //USART1 接收中断允许位
	//TXIE = 1;
}
/*****************************************************************************
 函 数 名  : usart1_send_byte
 功能描述  : USART发送单个字节
 输入参数  : char ch要发送的字符
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2016年5月13日
 作    者  : SJY
 修改内容  : 新生成函数

*****************************************************************************/

void usart1_send_byte(char ch)
{
    uint8 uart_cnt  = 0;
	TXREG=ch;
	while(!TX1STAbits.TRMT)
	{
		uart_cnt  ++;
		if( uart_cnt  > 1000)
		{
			uart_cnt  = 0;
			break;
		}
	}
}


