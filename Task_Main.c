
/*******************************
********时间片轮询法的架构******
********************************/

#include "task_main.h"
#include "adc.h"
#include "uart.h"
#include "stdio.h"
#include "motor.h"

#define tab_len  29


// 定义结构体变量
static TASK_COMPONENTS TaskComps[] =
{
    {0, 100, 100, TaskMotorCtrl},               //电机控制
    {0, 2000, 2000, TaskShowTem},               // 显示温度
};

// 任务清单
typedef enum _TASK_LIST
{
    TAST_MOTOR_CTRL,
    TAST_SHOW_TEM,             // 显示温度
    TASKS_MAX                // 总的可供分配的定时任务数目
} TASK_LIST;

 //PT100, 2分法， 查表， 根据电阻的AD 值计算温度。

uint16 const temp_tab[tab_len] =    //表格是以5度为一步，即-30, -25, - 20.....
 {
   59  ,79     ,104    ,135    ,171    ,212    ,260    ,312    ,368    ,425    ,       //-30.c  ...15.c
   484     ,541    ,596    ,647    ,695    ,737    ,776    ,809    ,839    ,864    ,   //20.c   ...  65.c
   886     ,905    ,921    ,935    ,947    ,957    ,966    ,973    ,980    ,           //70.c   ...  110.c

 };
/*****************************************************************************
 函 数 名  : get_temperature
 功能描述  : 获取温度函数
 输入参数  : 无
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月25日 星期四
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
float get_temperature(uint8 ad_channel)
{
    uint16 temp_ad = 0;
    uint8 left =0, right = tab_len-1,mid;
    float temp;
    mid = (left+right)/2;
    Sel_Adc_Channel(ad_channel);   //选择ad通道
    temp_ad =read_tem_adc();
    if(temp_ad >temp_tab[right])
    return 110;
    if(temp_ad <temp_tab[left])
    return 0;
    while(((right-left)!=1)&&(temp_tab[mid]!=temp_ad))
    {
        if(temp_tab[mid]< temp_ad)
        {
            left = mid;
        }
    else if(temp_tab[mid] > temp_ad)
        {
            right = mid;
        }
        mid = (left+right)/2;
    }
    temp = (((temp_ad-temp_tab[left])*5.0)/(temp_tab[right]-temp_tab[left]))+(mid*5 -30);// 阀值转化成温度
    return temp;

}

uint16 get_voltage(uint8 ad_channel)
{
    uint16 Ad_Voltage;
    Sel_Adc_Channel(ad_channel);
    Ad_Voltage= read_tem_adc();
    Ad_Voltage = Ad_Voltage*15/1024+5;
    return Ad_Voltage;
}


/*****************************************************************************
 函 数 名  : TaskMotorCtrl
 功能描述  : 电机控制函数
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年6月1日 星期四
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/

void TaskMotorCtrl(void)
{
    if(get_temperature(ADC_TEMP_MID)==31)
    {
        motor_run_pluse(FLOW_MOTOR, 3200);//正传一圈
    }
    else
     {
        motor_run_pluse(FLOW_MOTOR, 0);//反传一圈
     }
}
/*****************************************************************************
 函 数 名  : TaskShowTem
 功能描述  : 温度显示任务
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月25日 星期四
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
void TaskShowTem(void)
{
    sprintf(temp_val,"%.1f",get_temperature(ADC_TEMP_MID));
    uart_send_str(str1);
    uart_send_str(temp_val);
    uart_send_str(str2);
    usart1_send_byte(' ');
    sprintf(voltage,"%d",get_voltage(ADC_VREF));
    uart_send_str(str3);
    uart_send_str(voltage);
    uart_send_str(str4);
    usart1_send_byte('\n');
}

/*****************************************************************************
 函 数 名  : TaskRemarks
 功能描述  : 任务标记处理函数
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月24日 星期三
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
void TaskRemarks(void)
{
    uint8 i;
    for (i=0; i<TASKS_MAX; i++)                                 // 逐个任务时间处理
    {
         if (TaskComps[i].Timer)                                // 时间不为0
        {
            TaskComps[i].Timer--;                                // 减去一个节拍
            if (TaskComps[i].Timer == 0)                            // 时间减完了
            {
                 TaskComps[i].Timer = TaskComps[i].ItvTime;       // 恢复计时器值，从新下一次
                 TaskComps[i].Run = ON;                             // 任务可以运行
            }
        }
   }
}
/*****************************************************************************
 函 数 名  : TaskProcess
 功能描述  : 任务进程函数
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月24日 星期三
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
void TaskProcess(void)
{
    uint8 i;

    for (i=0; i<TASKS_MAX; i++)           // 逐个任务时间处理
    {
         if (TaskComps[i].Run)           // 时间不为0
        {
             TaskComps[i].TaskHook();         // 运行任务
             TaskComps[i].Run = 0;          // 标志清0
        }
    }
}

