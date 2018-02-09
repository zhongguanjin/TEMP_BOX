#ifndef __TASK_MAIN_H__
#define __TASK_MAIN_H__

#include "config.h"

// 任务结构体：
typedef struct _TASK_COMPONENTS
{
    uint8 Run;                 // 程序运行标记：0-不运行，1运行
    uint16 Timer;              // 计时器
    uint16 ItvTime;              // 任务运行间隔时间
    void (*TaskHook)(void);    // 要运行的任务函数
} TASK_COMPONENTS;              // 任务定义


void TaskShow(void);
extern void TaskProcess(void);
extern void TaskRemarks(void);
float get_temperature(uint8 ad_channel);

float get_voltage(uint8 ad_channel);

void TaskMotorCtrl(void);


#endif

