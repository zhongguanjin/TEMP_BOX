
/*******************************
********ʱ��Ƭ��ѯ���ļܹ�******
********************************/

#include "task_main.h"
#include "adc.h"
#include "uart.h"
#include "stdio.h"
#include "motor.h"

#define tab_len  29


// ����ṹ�����
static TASK_COMPONENTS TaskComps[] =
{
    {0, 100, 100, TaskMotorCtrl},               //�������
    {0, 2000, 2000, TaskShowTem},               // ��ʾ�¶�
};

// �����嵥
typedef enum _TASK_LIST
{
    TAST_MOTOR_CTRL,
    TAST_SHOW_TEM,             // ��ʾ�¶�
    TASKS_MAX                // �ܵĿɹ�����Ķ�ʱ������Ŀ
} TASK_LIST;

 //PT100, 2�ַ��� ��� ���ݵ����AD ֵ�����¶ȡ�

uint16 const temp_tab[tab_len] =    //�������5��Ϊһ������-30, -25, - 20.....
 {
   59  ,79     ,104    ,135    ,171    ,212    ,260    ,312    ,368    ,425    ,       //-30.c  ...15.c
   484     ,541    ,596    ,647    ,695    ,737    ,776    ,809    ,839    ,864    ,   //20.c   ...  65.c
   886     ,905    ,921    ,935    ,947    ,957    ,966    ,973    ,980    ,           //70.c   ...  110.c

 };
/*****************************************************************************
 �� �� ��  : get_temperature
 ��������  : ��ȡ�¶Ⱥ���
 �������  : ��
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2017��5��25�� ������
    ��    ��   : zgj
    �޸�����   : �����ɺ���

*****************************************************************************/
float get_temperature(uint8 ad_channel)
{
    uint16 temp_ad = 0;
    uint8 left =0, right = tab_len-1,mid;
    float temp;
    mid = (left+right)/2;
    Sel_Adc_Channel(ad_channel);   //ѡ��adͨ��
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
    temp = (((temp_ad-temp_tab[left])*5.0)/(temp_tab[right]-temp_tab[left]))+(mid*5 -30);// ��ֵת�����¶�
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
 �� �� ��  : TaskMotorCtrl
 ��������  : ������ƺ���
 �������  : void
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2017��6��1�� ������
    ��    ��   : zgj
    �޸�����   : �����ɺ���

*****************************************************************************/

void TaskMotorCtrl(void)
{
    if(get_temperature(ADC_TEMP_MID)==31)
    {
        motor_run_pluse(FLOW_MOTOR, 3200);//����һȦ
    }
    else
     {
        motor_run_pluse(FLOW_MOTOR, 0);//����һȦ
     }
}
/*****************************************************************************
 �� �� ��  : TaskShowTem
 ��������  : �¶���ʾ����
 �������  : void
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2017��5��25�� ������
    ��    ��   : zgj
    �޸�����   : �����ɺ���

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
 �� �� ��  : TaskRemarks
 ��������  : �����Ǵ�����
 �������  : void
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2017��5��24�� ������
    ��    ��   : zgj
    �޸�����   : �����ɺ���

*****************************************************************************/
void TaskRemarks(void)
{
    uint8 i;
    for (i=0; i<TASKS_MAX; i++)                                 // �������ʱ�䴦��
    {
         if (TaskComps[i].Timer)                                // ʱ�䲻Ϊ0
        {
            TaskComps[i].Timer--;                                // ��ȥһ������
            if (TaskComps[i].Timer == 0)                            // ʱ�������
            {
                 TaskComps[i].Timer = TaskComps[i].ItvTime;       // �ָ���ʱ��ֵ��������һ��
                 TaskComps[i].Run = ON;                             // �����������
            }
        }
   }
}
/*****************************************************************************
 �� �� ��  : TaskProcess
 ��������  : ������̺���
 �������  : void
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2017��5��24�� ������
    ��    ��   : zgj
    �޸�����   : �����ɺ���

*****************************************************************************/
void TaskProcess(void)
{
    uint8 i;

    for (i=0; i<TASKS_MAX; i++)           // �������ʱ�䴦��
    {
         if (TaskComps[i].Run)           // ʱ�䲻Ϊ0
        {
             TaskComps[i].TaskHook();         // ��������
             TaskComps[i].Run = 0;          // ��־��0
        }
    }
}

