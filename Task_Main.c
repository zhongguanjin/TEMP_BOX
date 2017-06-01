
/*******************************
********ʱ��Ƭ��ѯ���ļܹ�******
********************************/

#include "task_main.h"
#include "adc.h"
#include "uart.h"

#define tab_len  29

uint16 temp_value = 0;

// ����ṹ�����
static TASK_COMPONENTS TaskComps[] =
{
    {0, 1000, 1000, TaskShowTem},               // ��ʾ�¶�
};

// �����嵥
typedef enum _TASK_LIST
{

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
uint16 get_temperature(uint8 ad_channel)
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

    return (uint16)(temp*10);

}

uint16 get_voltage(uint8 ad_channel)
{
    uint16 Ad_Voltage;

    Sel_Adc_Channel(ad_channel);
    Ad_Voltage= read_tem_adc();
    return Ad_Voltage;
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
    uint16 temp;
    temp = get_temperature(ADC_TEMP_MID);
    usart1_send_byte(temp%10000/1000+'0');
    usart1_send_byte(temp%1000/100+'0');
    usart1_send_byte(temp%100/10+'0');
    usart1_send_byte('.');
    usart1_send_byte(temp%10+'0');
    usart1_send_byte('C');
    usart1_send_byte(' ');

    temp = get_voltage(ADC_TEMP_MID);
    usart1_send_byte(temp/100+'0');
    usart1_send_byte(temp%100/10+'0');
    usart1_send_byte(temp%100%10+'0');
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
                 TaskComps[i].Run = 1;                             // �����������
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

