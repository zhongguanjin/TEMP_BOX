#include "Task_Main.h"
#include "main.h"
#include "timer.h"
#include "system.h"
#include "adc.h"
//#include "uart.h"
#include "motor.h"
#include "dbg.h"
#include "DRV8837.h"
#include "com.h"
#include "console.h"
#include "SoftTimer.h"

#include "rgb.h"

//#include "pulse.h"


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
	rgb_init();
	com_init(com2,9600);
	Init_TMR0();
	Init_TMR6();
	Init_Motor();
	drv_8837_config();
    GIE		= 1;
	PEIE	= 1;
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
	//TimersInit(SystemTicksCount);
	app_modeSet(1);
	while(1)
	{
	    //ProcessTimer();
	    com2_rxDeal();
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
        RC2IF= 0;
        USART2_RXHandler(RC2REG);
    }
	if (TMR6IF && TMR6IE) // 200us �ж�һ��
	{
	    TMR6IF = 0;
	    //TaskMotorISR();
	    TaskMotorFun();
	}
    if(TMR0IF && TMR0IE)     // 1ms�ж�һ��
    {
        TMR0IF = 0;
        TMR0 = TMR0+TMR0_VALUE;
        TaskRemarks();       //��������ѯ����
        SystemTicksCount();
    }
}


