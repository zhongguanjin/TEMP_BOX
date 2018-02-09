
#include "uart.h"


#define 	PIC_CLK 		    16000000                     // ʱ��Ƶ��
#define     BAUD                9600                         //������
#define     SPBRGx_VAL          ((PIC_CLK/(16UL * BAUD) -1))
#define     BUSY                1
#define     FREE                0

#define RX2_PIN  TRISG2  //��������ͨѶ�˿�
#define TX2_PIN  TRISG1
#define RX1_PIN  TRISC7  //��������ͨѶ�˿�
#define TX1_PIN  TRISC6



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
    RX1_PIN = 1;
    TX1_PIN = 1;
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
    RCIE = 1;                //USART1 �����ж�����λ
	TXIE = 0;
	RCIF = 0;
}

void Init_UART2(void)
{
    RX2_PIN = 1;
    TX2_PIN = 1;
	//���÷��ͼĴ���
    TX2STAbits.TX9D=0;      //����żУ��λ
    TX2STAbits.TRMT=0;      //������λ�Ĵ���״̬λ  0-TSR ����
    TX2STAbits.BRGH = 1;
    TX2STAbits.SYNC=0;
    TX2STAbits.TXEN=1;
    TX2STAbits.TX9=0;       //����8λ
	//���ý��ռĴ���
    RC2STAbits.RX9D=0;      //����żУ��λ
    RC2STAbits.OERR=0;
    RC2STAbits.FERR=0;
    RC2STAbits.ADDEN=0;
    RC2STAbits.CREN=1;      //��������ʹ��λ
    // RCSTA2bits.SREN=0;
    RC2STAbits.RX9=0;       //����8λ
    RC2STAbits.SPEN=1;      //����ʹ��λ

    SPBRG2 = SPBRGx_VAL;      //�����ʶ�Ӧ��ֵ
    RC2IE = 1;                //USART1 �����ж�����λ
	TX2IE = 0;
	RC2IF = 0;
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

void usart1_send_byte(char dat)
{
    while(!TX1STAbits.TRMT)		//TRMT=0:���ڷ��ͣ�TRMT=1:���������
	{
		continue;
	}
	TXREG=dat;
}
void usart2_send_byte(char dat)
{
    while(!TX2STAbits.TRMT)		//TRMT=0:���ڷ��ͣ�TRMT=1:���������
	{
		continue;
	}
	TX2REG=dat;
}




