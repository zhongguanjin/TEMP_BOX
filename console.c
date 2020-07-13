#include "console.h"
#include "dbg.h"
#include "com.h"
#include "system.h"
#include "adc.h"
#include "motor.h"
#include "stdio.h"
#include "string.h"
#include "Task_Main.h"

#include "DRV8837.h"
#include "rgb.h"
#include "pulse.h"
consoleCallback console_cb = NULL;


void cs_drv8837Menu(void);
void cs_motorMenu(void);
void console_mainMenu(void);
void cs_rgbMenu(void);
int console_main(char * buf, int len);
int cs_drc8837Test(char * buf, int len);
int cs_motorTest(char * buf, int len);
int cs_rgbTest(char * buf, int len);

uint8 val_getPara(int16 *cp,char *string);


/*****************************************************************************
 函 数 名  : val_getPara
 功能描述  : 字符串转整形数组函数
 输入参数  : char *string  要转化的字符串
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2018年2月6日
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
uint8 val_getPara(int16 *cp,char *string)
{
#if 1
    uint8 i=0,j=0,k=0;
    while(*string)
    {
       if(*string >= '0'&&*string <= '9')
       {
           k++;
           if(k==1)
           {
             cp[i]= *string-'0';
           }
           else if(k>=2)
           {
              cp[i]= cp[i]*10+(*string-'0');
           }
       }
       else if(*string==','||*string==' ')
       {
           k=0;
           i++;
           j++;
       }
       else if(*string == 0x0D) //回车符
       {
          j++;
          break;
       }
       string++;
    }
    return j;
#else
    uint8 i=0,j=0;
    while(*string)
    {
       if(*string >= '0'&&*string <= '9')
       {
           if(*(string+1)==','||*(string+1)==' ')//数据后面是逗号，空格
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
       else if(*string == 0x0D) //回车符
       {
          j++;
          break;
       }
       string++;
    }
    return j;
#endif
}

/*****************************************************************************
 函 数 名  : com2_rxDeal
 功能描述  : com2接收数据处理
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2018年7月9日 星期一
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
void com2_rxDeal(void)
{
    static char buf[256];
    static int len=0;
    char ch;
	if(com_rxLeft(com2) != 0)
	{
		while(1)
		{
			if(OK == com_getch(com2,&ch))
			{
                buf[len++] = ch;
                if(ch < 0x20)
                {
                    if(len != 0)
                    {   // 包含0D
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
                    //dbg("%c\r\n",ch);
                }
			}
			else
			{
				break;
			}
		}
	}
}

/*****************************************************************************
 函 数 名  : console_mainMenu
 功能描述  : 主菜单
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2018年2月9日
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/

void console_mainMenu(void)
{
	my_printf("\r\n\t cs menu:\r\n");
	dbg("a: get bat val\r\n");
	dbg("b: get tempsensor val\r\n");
	dbg("c: motor test\r\n");
	dbg("d: drv8837 test\r\n");
	dbg("e: rgb test\r\n");
}

/*****************************************************************************
 函 数 名  : cs_motorTest
 功能描述  : 电机测试驱动
 输入参数  : char * buf
             int len
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2018年2月9日
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
int cs_motorTest(char * buf, int len)
{
	union {
		int16 u[5];
		struct {
			int16		FUNC;
			int16		MID;
			int16		DIR;
			int32		PULSE;
		};
	} para;
    memset(para.u,0,sizeof(para));
    uint8 i=0;
    i=val_getPara(para.u,buf);
    dbg("para:%d,%d,%d,%d\r\n",para.u[0],para.u[1],para.u[2],para.u[3],para.u[4]);
	if(((para.FUNC == 2)&&(i != 4))
	    //||(para.MID>=MOTOR_MAX)
	    ||((para.FUNC==3)&&(i!=2)))
	{
	    dbg("err\r\n");
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
		    motor_move(para.MID,para.DIR,para.PULSE);
		    //motor_run_pulse(para.MID,para.DIR,para.PULSE);
			break;
		}
		case 3:
		{
		    dbg("3-stop\r\n");
		    motor_stop(para.MID);
			break;
		}
		case 4:
		{
		    uint32 pls=0;
		    pls=motor_getPulse(para.MID);
		    dbg("pls %l\r\n",pls);
			break;
		}
		case 5:
		{
		    dbg("5-all run\r\n");
		    motor_run_pulse(0,para.DIR,para.PULSE);
            motor_run_pulse(1,para.DIR,para.PULSE);
			break;
		}
		case 6:
		{
		    dbg("6-set speed\r\n");
            motor_speed_set(para.MID);
			break;
		}
        case 7:
        {
            if(para.MID == 1)
            {
                set_msgid(0);
                app_stateSet(0);
            }
            else
            {
                set_msgid(0);
                app_stateSet(2);
            }
            break;
        }
		default:
		{
            return 1;
		}
	}
	cs_motorMenu();
	return 0;
}

/*****************************************************************************
 函 数 名  : cs_motorTestMenu
 功能描述  : 电机测试菜单
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2018年2月9日
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
void cs_motorMenu(void)
{
	console_cb = cs_motorTest;
	my_printf("\r\n\tmotor test\r\n");
	dbg("init:1\r\n");
	dbg("run: 2,mid,dir,pulse\r\n");
	dbg("stop: 3,mid\r\n");
	dbg("get pls: 4,mid\r\n");
    dbg("all run:5,mid,dir,pulse\r\n");
    dbg("set speed:6,gear\r\n");
    dbg("test:7\r\n");
	dbg("mid: 0-flow motor,1-temp motor\r\n");
	dbg("dir: 0-POS,1-REV\r\n");
	dbg("pulse: step \r\n");
	dbg("gear: 0-19 \r\n");

	dbg("motor>\r\n");
}
/*****************************************************************************
 函 数 名  : cs_drc8837Test
 功能描述  : 脉冲阀测试驱动
 输入参数  : char * buf
             int len
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2018年2月9日
    作    者   : zgj
    修改内容   : 新生成函数

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
    memset(para.u,0,sizeof(para));
    uint8 i=0;
    i=val_getPara(para.u,buf);
    dbg("para:%d,%d\r\n",para.u[0],para.u[1]);
    if((i!=2)||(para.MID>=DRV8837_MAX))
    {
        dbg("err\r\n");
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
            return 1;
		}
    }
    cs_drv8837Menu();
    return 0;
}

/*****************************************************************************
 函 数 名  : cs_drv8837Menu
 功能描述  : 脉冲阀测试菜单
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2018年2月9日
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
void cs_drv8837Menu(void)
{
 	console_cb = cs_drc8837Test;
	my_printf("\r\n\t drv8837 test\r\n");
	dbg("open: 1,mid\r\n");
    dbg("close: 2,mid\r\n");
    dbg("state: 3,mid\r\n");
    dbg("mid: 0-tap,1-shower\r\n");
	dbg("drv8837>\r\n");
}


/*****************************************************************************
 函 数 名  : cs_drc8837Test
 功能描述  : 脉冲阀测试驱动
 输入参数  : char * buf
             int len
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2018年2月9日
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
int cs_rgbTest(char * buf, int len)
{
	union {
		int16 u[2];
		struct {
			int16		FUNC;
			int16		MID;
		};
	} para;
    memset(para.u,0,sizeof(para));
    uint8 i=0;
    i=val_getPara(para.u,buf);
    dbg("para:%d,%d\r\n",para.u[0],para.u[1]);
    if((i!=2)||(para.MID>=ALL+1))
    {
        dbg("err\r\n");
        return 1;
    }
    rgb_ctr(para.FUNC, para.MID);
    cs_rgbMenu();
    return 0;
}


void cs_rgbMenu(void)
{
 	console_cb = cs_rgbTest;
	my_printf("\r\n\t rgb test\r\n");
	dbg("red: 1,mid\r\n");
    dbg("green: 2,mid\r\n");
    dbg("blue: 3,mid\r\n");
    dbg("all: 4,mid\r\n");
    dbg("mid: 0-off,1-on\r\n");
	dbg("rgb>\r\n");
}

/*****************************************************************************
 函 数 名  : console_main
 功能描述  : 主菜单驱动
 输入参数  : char * buf
             int len
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2018年2月9日
    作    者   : zgj
    修改内容   : 新生成函数

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
			cs_motorMenu();
			break;
		}
		case 'd':
		{
            cs_drv8837Menu();
			break;
		}
		case 'e':
		{
            cs_rgbMenu();
			break;
		}
		default:
		{
            return 1;
		}
    }
	return 0;
}

