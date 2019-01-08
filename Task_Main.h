#ifndef __TASK_MAIN_H__
#define __TASK_MAIN_H__

#include "config.h"

// ����ṹ�壺
typedef struct
{
    uint8 Run;                 // �������б�ǣ�0-�����У�1����
    uint16 Timer;              // ��ʱ��
    uint16 ItvTime;              // �������м��ʱ��
    void (*TaskHook)(void);    // Ҫ���е�������
} TASK_COMPONENTS;              // ������




char soft_version[] = {"V1.2"};




extern int8 get_msgid(void);
extern void set_msgid(uint8 id);


extern void TaskProcess(void);
extern void TaskRemarks(void);
extern uint8 get_temperature(uint8 ad_channel);
extern float get_voltage(uint8 ad_channel);
extern void app_modeSet(uint8 mode);
extern void app_stateSet(uint8 state);

extern void com1_rxDeal(void);
#endif

