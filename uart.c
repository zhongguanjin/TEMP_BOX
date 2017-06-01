
#include "uart.h"


#define 	PIC_CLK 		    16000000//4000000                    // ʱ��Ƶ��
#define     BAUD                9600                                                 //������
#define     SPBRGx_VAL          ((PIC_CLK/(16UL * BAUD) -1))
#define     BUSY                1
#define     FREE                0

#define RX_PIN  TRISC7  //��������ͨѶ�˿�
#define TX_PIN  TRISC6

static	uint8 usart1_tx_data[64];			//�������������
static	uint8	usart1_tx_len;		//�������ݵĳ���
volatile static  bit  tx1_sign;

/*****************************************************************************
 �� �� ��  : usart1_init
 ��������  : USART1��ʼ����������9600
 �������  : ��
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2016��5��13��
 ��    ��  : SJY
 �޸�����  : �����ɺ���

*****************************************************************************/

void Init_UART1(void)
{
    RX_PIN = 1;
    TX_PIN = 1;
	//���÷��ͼĴ���
    TX1STAbits.TX9D=0;      //����żУ��λ
    TX1STAbits.TRMT=0;      //������λ�Ĵ���״̬λ  0-TSR ����
    TX1STAbits.BRGH = 1;
    TX1STAbits.SYNC=0;
    TX1STAbits.TXEN=1;
    TX1STAbits.TX9=0;       //����8λ
	//���ý��ռĴ���
    RC1STAbits.RX9D=0;      //����żУ��λ
    RC1STAbits.OERR=0;
    RC1STAbits.FERR=0;
    RC1STAbits.ADDEN=0;
    RC1STAbits.CREN=1;      //��������ʹ��λ
    // RCSTA2bits.SREN=0;
    RC1STAbits.RX9=0;       //����8λ
    RC1STAbits.SPEN=1;      //����ʹ��λ

    SPBRG = SPBRGx_VAL;      //�����ʶ�Ӧ��ֵ
    //RCIE = 1;                //USART1 �����ж�����λ
	//TXIE = 1;
}
/*****************************************************************************
 �� �� ��  : usart1_send_byte
 ��������  : USART���͵����ֽ�
 �������  : char chҪ���͵��ַ�
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2016��5��13��
 ��    ��  : SJY
 �޸�����  : �����ɺ���

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


