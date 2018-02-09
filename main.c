#include "Task_Main.h"
#include "main.h"
#include "timer.h"
#include "system.h"
#include "adc.h"
#include "uart.h"
#include "motor.h"
#include "dbg.h"
#include "stdio.h"
#include "string.h"
#include "DRV8837.h"

consoleCallback console_cb = NULL;

#define	CONSOLE_RX_BUF_LEN		32	// must 2**n
#define	CONSOLE_RX_BUF_MASK	(CONSOLE_RX_BUF_LEN-1)

typedef struct _UART_BUF_TAG
{
	unsigned char	in;
	unsigned char	out;
	unsigned char	buf[CONSOLE_RX_BUF_LEN];
} uart_buf_t;

uart_buf_t		uart2rx;




void uart_bufInit(uart_buf_t * pBuf);
void cs_drv8837Menu(void);
void cs_motorTestMenu(void);
void console_mainMenu(void);

int console_main(char * buf, int len);
int cs_drc8837Test(char * buf, int len);
int cs_motorTest(char * buf, int len);

int uart2_getch(char * p);
uint8 val_getPara(int16 *cp,char *string);

/*****************************************************************************
 �� �� ��  : Init_Sys
 ��������  : ϵͳ��ʼ������
 �������  : void
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2017��5��18�� ������
    ��    ��   : zgj
    �޸�����   : �����ɺ���

*****************************************************************************/
void Init_Sys(void)
{
	Init_MCU();
	Init_ADC();
	//Init_UART1();
	Init_UART2();
	Init_TMR0();
	Init_TMR6();
    GIE		= 1;
	PEIE	= 1;
}

void uart_bufInit(uart_buf_t * pBuf)
{
	pBuf->in = 0;
	pBuf->out = 0;
}

/*****************************************************************************
 �� �� ��  : uart2_getch
 ��������  : ���ڻ�ȡ�ַ�����
 �������  : char * p
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2018��2��9��
    ��    ��   : zgj
    �޸�����   : �����ɺ���

*****************************************************************************/
int uart2_getch(char * p)
{
	if (uart2rx.in != uart2rx.out)
	{
		*p = uart2rx.buf[uart2rx.out & CONSOLE_RX_BUF_MASK];
		uart2rx.out++;
		return 0;
	}
	else
	{
		return -1;
	}
}

/*****************************************************************************
 �� �� ��  : val_getPara
 ��������  : �ַ���ת�������麯��
 �������  : char *string  Ҫת�����ַ���
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2018��2��6��
    ��    ��   : zgj
    �޸�����   : �����ɺ���

*****************************************************************************/
uint8 val_getPara(int16 *cp,char *string)
{
    uint8 i=0,j=0;
    while(*string)
    {
       if(*string >= '0'&&*string <= '9')
       {
           if(*(string+1)==','||*(string+1)==' ')//���ݺ����Ƕ��ţ��ո�
           {
               cp[i]= *string - '0';
               i++;
               j++;
           }
           else
           {
                cp[i]= cp[i]*10+(*string-'0');
           }
       }
       else if(*string == 0x0D) //�س���
       {
          j++;
          break;
       }
       string++;
    }
    return j;
}
/*****************************************************************************
 �� �� ��  : console_mainMenu
 ��������  : ���˵�
 �������  : void
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2018��2��9��
    ��    ��   : zgj
    �޸�����   : �����ɺ���

*****************************************************************************/
void console_mainMenu(void)
{
	dbg("\r\n\t cs menu:\r\n");
	dbg("a: get bat val\r\n");
	dbg("b: get tempsensor val\r\n");
	dbg("c: motor test\r\n");
	dbg("d: drv8837 test\r\n");
}

/*****************************************************************************
 �� �� ��  : cs_motorTest
 ��������  : �����������
 �������  : char * buf
             int len
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2018��2��9��
    ��    ��   : zgj
    �޸�����   : �����ɺ���

*****************************************************************************/
int cs_motorTest(char * buf, int len)
{
	union {
		int16 u[4];
		struct {
			int16		FUNC;
			int16		MID;
			int16		DIR;
			int16		PULSE;
		};
	} para;
    uint8 i=0;
    i=val_getPara(para.u,buf);
    dbg("para:%d,%d,%d,%d\r\n",para.u[0],para.u[1],para.u[2],para.u[3]);
	if(((para.FUNC == 2)&&(i != 4))
	    ||(para.MID>=MOTOR_MAX)
	    ||((para.FUNC==3)&&(i!=2)))
	{
	    dbg("err\r\n");
        memset(para.u,0,sizeof(para));
		return 1;
	}
	switch(para.FUNC)
	{
		case 1:
		{
		    dbg("1-init\r\n");
			break;
		}
		case 2:
		{
		    dbg("2-run\r\n");
			break;
		}
		case 3:
		{
		    dbg("3-stop\r\n");
			break;
		}
		default:
		{
            memset(para.u,0,sizeof(para));
            return 1;
		}
	}
    memset(para.u,0,sizeof(para));
	cs_motorTestMenu();
	return 0;
}

/*****************************************************************************
 �� �� ��  : cs_motorTestMenu
 ��������  : ������Բ˵�
 �������  : void
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2018��2��9��
    ��    ��   : zgj
    �޸�����   : �����ɺ���

*****************************************************************************/
void cs_motorTestMenu(void)
{
	console_cb = cs_motorTest;
	dbg("\r\n\tmotor test\r\n");
	dbg("init:1\r\n");
	dbg("run: 2,mid,dir,pulse\r\n");
	dbg("stop: 3,mid\r\n");
	dbg("mid: 0-temp motor,1-flow motor\r\n");
	dbg("dir: 0-POS,1-REV\r\n");
	dbg("pulse: step num\r\n");
	dbg("motor>\r\n");
}
/*****************************************************************************
 �� �� ��  : cs_drc8837Test
 ��������  : ���巧��������
 �������  : char * buf
             int len
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2018��2��9��
    ��    ��   : zgj
    �޸�����   : �����ɺ���

*****************************************************************************/
int cs_drc8837Test(char * buf, int len)
{
	union {
		int16 u[2];
		struct {
			int16		FUNC;
			int16		MID;
		};
	} para;
    uint8 i=0;
    i=val_getPara(para.u,buf);
    dbg("para:%d,%d\r\n",para.u[0],para.u[1]);
    if((i!=2)||(para.MID>=DRV8837_MAX))
    {
        dbg("err\r\n");
        memset(para.u,0,sizeof(para));
        return 1;
    }
    switch(para.FUNC)
    {
        case 1:
        {
            if(para.MID ==0)
            {
                dbg("tap open\r\n");
                drv8837_ctr(DRV8837_TAP, ON);
            }
            else
            {
               dbg("shower open\r\n");
               drv8837_ctr(DRV8837_SHOWER, ON);
            }
            break;
        }
        case 2:
        {
            if(para.MID ==0)
            {
                dbg("tap close\r\n");
                drv8837_ctr(DRV8837_TAP, OFF);

            }
            else
            {
               dbg("shower close\r\n");
               drv8837_ctr(DRV8837_SHOWER, OFF);
            }
            break;
        }
        case 3:
        {
            if(para.MID ==0)
            {
                dbg("tap state:%d\r\n",get_8837_state(DRV8837_TAP));
            }
            else
            {
                dbg("shower state:%d\r\n",get_8837_state(DRV8837_SHOWER));
            }
            break;
        }
		default:
		{
            memset(para.u,0,sizeof(para));
            return 1;
		}
    }
    memset(para.u,0,sizeof(para));
    cs_drv8837Menu();
    return 0;

}

/*****************************************************************************
 �� �� ��  : cs_drv8837Menu
 ��������  : ���巧���Բ˵�
 �������  : void
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2018��2��9��
    ��    ��   : zgj
    �޸�����   : �����ɺ���

*****************************************************************************/
void cs_drv8837Menu(void)
{
 	console_cb = cs_drc8837Test;
	dbg("\r\n\t drv8837 test\r\n");
	dbg("open: 1,mid\r\n");
    dbg("close: 2,mid\r\n");
    dbg("state: 3,mid\r\n");
    dbg("mid: 0-tap,1-shower\r\n");
	dbg("drv8837>\r\n");
}
/*****************************************************************************
 �� �� ��  : console_main
 ��������  : ���˵�����
 �������  : char * buf
             int len
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2018��2��9��
    ��    ��   : zgj
    �޸�����   : �����ɺ���

*****************************************************************************/
int console_main(char * buf, int len)
{
	if(memcmp(buf,"reboot",6) == 0)
	{
        dbg("reboot\r\n");
        delay_ms(600);
        RESET();
	}
   	switch(buf[0])
	{
		case 'a':
		{
		    dbg("bat:%fv\r\n",get_voltage(ADC_VREF_BAT));

			break;
		}
		case 'b':
		{
		    dbg("tap:%fc,shower:%fc\r\n",get_temperature(ADC_TEMP_TAP),get_temperature(ADC_TEMP_SHOWER));
			break;
		}
		case 'c':
		{
			cs_motorTestMenu();
			break;
		}
		case 'd':
		{
            cs_drv8837Menu();
			break;
		}
		default:
		{
            return 1;
		}
    }
	return 0;
}


/*****************************************************************************
 �� �� ��  : main
 ��������  : ������
 �������  : void
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2017��5��18�� ������
    ��    ��   : zgj
    �޸�����   : �����ɺ���

*****************************************************************************/
void main(void)
{
	Init_Sys();
	dbg("SYSCLK:%dM\r\n",SYSCLK_Frequency);
	drv_8837_config();
	while(1)
	{
		static char buf[256];
		static int len=0;
		char ch;
		if(0 == uart2_getch(&ch))
		{
			buf[len++] = ch;
			if(ch < 0x20)
			{
				if(len != 0)
				{	// ����0D
					buf[len] = 0;
					if(console_cb == NULL)
					{
						console_cb = console_main;
					}
					if(console_cb(buf,len) != 0)
					{
						console_cb = NULL;
						console_mainMenu();
					}
				}
				len = 0;
			}
			else
			{
				dbg("%c\r\n",ch);
			}
		}
	    TaskProcess();            // ������
	    CLRWDT();
	}
}

/*****************************************************************************
 �� �� ��  : ISR
 ��������  : �жϷ�����
 �������  : void
 �������  : ��
 �� �� ֵ  : void
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2017��5��18�� ������
    ��    ��   : zgj
    �޸�����   : �����ɺ���

*****************************************************************************/
void interrupt ISR(void)
{
    if(RC2IE &&RC2IF)
    {
        RCIF= 0;
        uart2rx.buf[uart2rx.in++&CONSOLE_RX_BUF_MASK] = (uint8)(RC2REG);
        //usart2_send_byte(RC2REG);
    }
	if (TMR6IF && TMR6IE) // 1ms �ж�һ��
	{
	    TMR6IF = 0;
	}
    if(TMR0IF && TMR0IE)     // 1ms�ж�һ��
    {
        TMR0IF = 0;
        TMR0 = TMR0+TMR0_VALUE;
        TaskRemarks();       //��������ѯ����
    }
}
