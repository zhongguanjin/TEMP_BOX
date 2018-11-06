#ifndef __TASK_MAIN_H__
#define __TASK_MAIN_H__

#include "config.h"

// 任务结构体：
typedef struct
{
    uint8 Run;                 // 程序运行标记：0-不运行，1运行
    uint16 Timer;              // 计时器
    uint16 ItvTime;              // 任务运行间隔时间
    void (*TaskHook)(void);    // 要运行的任务函数
} TASK_COMPONENTS;              // 任务定义


#define     BUF_SIZE   13

//串口协议结构体
typedef struct
{
   uint8 check_st;// 1-有信号，0-无信号
   uint8 psd;       // 触发门限
}RadarInfo_t;


RadarInfo_t RadarInfo;



//串口协议结构体
typedef union
{
    struct
    {
        uint8 sta_num;
        uint8 spare1;
        uint8 spare2;
        uint8 dat[BUF_SIZE-5];
        uint8 crc_num;
        uint8 end1_num;
    };               //发送
    uint8 txbuf[BUF_SIZE];
}RFTx_t;


typedef  union
{
      struct
      {
          uint8 sta_num;
          uint8 spare1;
          uint8 spare2;
          uint8 dat[BUF_SIZE-5];
          uint8 crc_num;
          uint8 end1_num;
      };
      uint8 rxbuf[BUF_SIZE];
} RFRx_t;



static RFTx_t  Rftx;
static RFRx_t  Rfrx;



extern void com1_rxDeal(void);

extern int8 get_msgid(void);
extern void set_msgid(uint8 id);


extern void TaskProcess(void);
extern void TaskRemarks(void);
extern float get_temperature(uint8 ad_channel);
extern float get_voltage(uint8 ad_channel);
extern void app_modeSet(uint8 mode);
extern void app_stateSet(uint8 state);


#endif

