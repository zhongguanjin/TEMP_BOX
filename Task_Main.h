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



/*
typedef enum
{
    SRC_MAIN=0,
    SRC_MAX
}SRC_DEF;
*/
#define ComRx_START 0x02
#define ComRx_SPARE 0xA3
#define ComRx_END1  0x0F
#define ComRx_END2  0x04
#define ComRx_ADDR  0x01


#define ComTx_START 0x02
#define ComTx_SPARE 0x3A
#define ComTx_END1  0x0B
#define ComTx_END2  0x04
#define ComTx_ADDR  0x01


#define     BUF_SIZE   32
#define     crc_len    (BUF_SIZE-5)

typedef  union
{
      struct
      {
          uint8 sta_num;
          uint8 spare1;
          uint8 dev_addr;
          uint8 dat[BUF_SIZE-6];
          uint8 crc_num;
          uint8 end1_num;
          uint8 end2_num;
      };
      uint8 buf[BUF_SIZE];
} ComBuf_t;




extern int8 get_msgid(void);
extern void set_msgid(uint8 id);


extern void TaskProcess(void);
extern void TaskRemarks(void);
extern float get_temperature(uint8 ad_channel);
extern float get_voltage(uint8 ad_channel);
extern void app_modeSet(uint8 mode);
extern void app_stateSet(uint8 state);
extern void com1_rxDeal(void);

#endif

