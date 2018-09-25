/***************************************************************************************
* timer.c
*
*
*****************************************************************************************/

#include "timer.h"


/*****************************************************************************
 函 数 名  : Init_TMR0
 功能描述  : 定时器0初始化函数
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
void Init_TMR0(void)
{
	TMR0CS	= 0;				//内部指令周期时钟（16m/4=4MHz 0.25us
	TMR0SE	= 0;				//在T0CKI 引脚信号从低至高跳变时，递增计数
	PSA		= 0;				//预分频器分配给Timer0 模块
	PS2		= 1;
	PS1		= 0;
	PS0		= 0;				//预分频比1:32 0.25us*32 = 8us
	TMR0IF	= 0;
	TMR0IE	= 1;				//关闭开TMR0中断
	TMR0	= 0x7D;				//TMR0计数255(0xff)-(1ms/8us)= 0X82 定时1ms产生一个中断
}


void Init_TMR6(void)
{
    //配置 TXCON 控制寄存器
    T6OUTPS0 = 1;
    T6OUTPS1 = 0;  //  1:10 后分频比 0.25*10 = 2.5us  400khz
    T6OUTPS2 = 0;
    T6OUTPS3 = 1;
    TMR6ON = 1;   //使能位
    T6CKPS0 =1;   // 4分频  2.5*4 = 10us  100k
    T6CKPS1 = 0;
    //PR6 = 20;//20;   // 50-500us中断一次  pwm_period=1ms(1khz)   (20-200us一次 pwm_period=400us(2.5khz))
    TMR6IE =0;
    TMR6IF =0;
}


void Init_TMR2(void)
{
    //配置 TXCON 控制寄存器
    T2OUTPS0 = 1;
    T2OUTPS1 = 0;  //  1:10 后分频比 0.25*10 = 2.5us  400khz
    T2OUTPS2 = 0;
    T2OUTPS3 = 1;
    TMR2ON = 1;   //使能位
    T2CKPS0 =1;   // 4分频  2.5*4 = 10us  100k
    T2CKPS1 = 0;
    PR2 = 50;//20;   // 50-500us中断一次  pwm_period=1ms(1khz)   (20-200us一次 pwm_period=400us(2.5khz))
    TMR2IE =1;
    TMR2IF =0;
}

uint32 SystemTicksCount(void)
{
   static uint32 tick_time1=0;
   tick_time1++;

   return tick_time1;
}


