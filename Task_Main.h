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


#define     BUF_SIZE   13

//����Э��ṹ��
typedef struct
{
   uint8 check_st;// 1-���źţ�0-���ź�
   uint8 psd;       // ��������
}RadarInfo_t;


RadarInfo_t RadarInfo;



//����Э��ṹ��
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
    };               //����
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

