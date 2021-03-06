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

#include "pulse.h"


/*****************************************************************************
 函 数 名  : Init_Sys
 功能描述  : 系统初始化函数
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月18日 星期四
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
void Init_Sys(void)
{
	Init_MCU();
	Init_ADC();
	rgb_init();
	//com_init(com2,9600);
	com_init(com1,19200);
	Init_TMR0();
	Init_TMR6();
	Init_Motor();
	drv_8837_config();
    GIE		= 1;
	PEIE	= 1;
}



/*****************************************************************************
 函 数 名  : main
 功能描述  : 主函数
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月18日 星期四
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/

void main(void)
{
	Init_Sys();
	dbg("SYSCLK:%dM\r\n",SYSCLK_Frequency);
dbg("new_fun");
	delay_ms(2000);
    CLRWDT();
	delay_ms(2000);
    CLRWDT();
	//TimersInit(SystemTicksCount);
	app_modeSet(0);
	while(1)
	{
	    //ProcessTimer();
	    com1_rxDeal();
	    //com2_rxDeal();
	    TaskProcess();            // 任务处理
	    CLRWDT();
	}
}

/*****************************************************************************
 函 数 名  : ISR
 功能描述  : 中断服务函数
 输入参数  : void
 输出参数  : 无
 返 回 值  : void
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月18日 星期四
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
void interrupt ISR(void)
{
    if(RCIE &&RCIF)
    {
        RCIF= 0;
        USART1_RXHandler(RCREG);
    }
    if(RC2IE &&RC2IF)
    {
        RC2IF= 0;
        //USART2_RXHandler(RC2REG);
    }
	if (TMR6IF && TMR6IE) // 200us 中断一次
	{
	    TMR6IF = 0;
	    //TaskMotorISR();
	    TaskMotorFun();
	}
    if(TMR0IF && TMR0IE)     // 1ms中断一次
    {
        TMR0IF = 0;
        TMR0 = TMR0+TMR0_VALUE;
        TaskRemarks();       //任务标记轮询处理
        SystemTicksCount();
    }
}


