#ifndef __TASK_MAIN_H__
#define __TASK_MAIN_H__

#include "config.h"

// ����ṹ�壺
typedef struct _TASK_COMPONENTS
{
    uint8 Run;                 // �������б�ǣ�0-�����У�1����
    uint16 Timer;              // ��ʱ��
    uint16 ItvTime;              // �������м��ʱ��
    void (*TaskHook)(void);    // Ҫ���е�������
} TASK_COMPONENTS;              // ������


void TaskShow(void);
extern void TaskProcess(void);
extern void TaskRemarks(void);
float get_temperature(uint8 ad_channel);

float get_voltage(uint8 ad_channel);

void TaskMotorCtrl(void);


#endif

