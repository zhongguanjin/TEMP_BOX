/******************************************************************************
* �ļ����ƣ�SoftTimer.h
* ����ժҪ�������ʱ��ģ��ͷ�ļ�
* ����˵������������TimersInit����������ú����ṩ"1ms��ϵͳʱ��"��"���ϵͳms��"��
*          Ȼ���ڸ��Ե�Ӧ��ģ���е���CreatTimer������ʱ�����ú������صĵ�ַΪ�ö�
*          ʱ���ĵ�ַ��������������ɾ����ʱ����㣬�����Ʊ��ܡ�������ѭ����ִ��
*          ProcessTimer�����Ը��¶�ʱ��ʱ�䡣
* ��ǰ�汾��V1.00
* �� �ߣ�  zgj
* ������ڣ�2018��2��20��

******************************************************************************/
#ifndef __SOFTTIMER_H_
#define __SOFTTIMER_H_



#include "config.h"

#if 0

#ifdef __cplusplus
extern "C" {
#endif




#define MAX_VALUE_32_BIT 0xFFFFFFFF /* 32bit���ms�� */

#define SW_ERROR               (-1) /* ����ʧ�ܷ���ֵ */
#define SW_OK                   (0) /* �����ɹ�����ֵ */

#define SINGLE		0
#define PERIOIC		1



typedef int (*TMRCALLBACK)(void *pArg);  //��ʱ�ص�����ָ��
typedef unsigned long (*TMRSOURCE)(void);       //ϵͳʱ�Ӻ���ָ��

/****************************************************
* �ṹ����TIMER
* ������ �����ʱ�����ݽṹ
* ������
     uint8        periodic;        SINGLE       ���δ���
                                   PERIOIC      ���ڴ���
     uint32       start;           0~0xFFFFFFFF ��ʱ����ʼʱ��
     uint32       now;             0~0xFFFFFFFF ��ʱ����ǰʱ��
     uint32       elapse;          0~0xFFFFFFFF ��ʱ���ѹ�ʱ��
     uint32       timeout;         0~0xFFFFFFFF ��ʱ����ʱʱ��
     TMRCALLBACK  pfTimerCallback;              ��ʱ������ִ�еĻص�����
     void        *pArg;                         �ص������Ĳ���
* �� �ߣ�  zgj
* ������ڣ�2018��2��20��

****************************************************/
typedef struct _TIMER
{
    uint8      periodic;         /* ���δ���/���ڴ��� */
    uint32      start;           /* ��ʱ����ʼʱ�� */
    uint32      now;             /* ��ʱ����ǰʱ�� */
    uint32      elapse;          /* ��ʱ���ѹ�ʱ�� */
    uint32      timeout;         /* ��ʱ����ʱʱ�� */
    TMRCALLBACK pfTimerCallback; /* ��ʱ������ִ�еĻص����� */
    void       *pArg;            /* �ص������Ĳ��� */
} TIMER;


/****************************************************
* �ṹ����TIMER_TABLE
* ������ ��ʱ���б�(����)
* ������
          TIMER                data; ����ʱ���������
          struct _TIMER_TABLE* next; ��һ����ʱ������ַ
* �� �ߣ�  zgj
* ������ڣ�2018��2��20��
****************************************************/
typedef struct _TIMER_TABLE
{
    TIMER                data; /* ����ʱ��������� */       //������
    struct _TIMER_TABLE* next; /* ��һ����ʱ������ַ */   //ָ����
}TIMER_TABLE;

/*************************************************************************
* �������ƣ�int TimersInit(TMRSOURCE pfTimer)
* ����˵������ʼ�������ʱ��ģ��
* ���������TMRSOURCE pfTimer    ϵͳ1msʱ�Ӻ���
* �����������
* �� �� ֵ��SW_ERROR: ����ʧ��
           SW_OK �����ɹ�
* ����˵������
**************************************************************************/
int TimersInit(TMRSOURCE pfTimer);

/*************************************************************************
* �������ƣ�TIMER_TABLE* CreatTimer(uint32 dwTimeout, uint8 ucPeriodic, TMRCALLBACK pfTimerCallback, void *pArg)
* ����˵�������������������ʱ��
* ���������uint32         dwTimeout  0~0xFFFFFFFF ��ʱʱ��
           uint8          ucPeriodic SINGLE      ���δ���
                                     PERIODIC    ���ڴ���
           TMRCALLBACK    pfTimerCallback        ��ʱ����ʱ�ص�����
            void         *pArg                   �ص���������

* �����������
* �� �� ֵ������ʧ�� : NULL
           �����ɹ� : ��ʱ��ģ��ָ��
* ����˵���������궨ʱ���󷵻ض�ʱ�����ĵ�ַ���õ�ַ����������ɾ���ö�ʱ��
**************************************************************************/
TIMER_TABLE* CreatTimer(uint32 dwTimeout, uint8 ucPeriodic, TMRCALLBACK pfTimerCallback, void *pArg);


/*************************************************************************
* �������ƣ�int KillTimer(TIMER_TABLE* ptNode)
* ����˵����ɾ����ʱ�����
* ���������TIMER_TABLE* ptNode ��ʱ������ַ
* �����������
* �� �� ֵ��SW_ERROR: ����ʧ��
           SW_OK �����ɹ�
* ����˵������
**************************************************************************/
extern int KillTimer(TIMER_TABLE* ptNode);


/*************************************************************************
* �������ƣ�int ResetTimer(TIMER_TABLE* ptNode)
* ����˵����������ʱ�����
* ���������TIMER_TABLE* ptNode ��ʱ������ַ
* �����������
* �� �� ֵ��SW_ERROR: ����ʧ��
           SW_OK �����ɹ�
* ����˵������
**************************************************************************/
extern int ResetTimer(TIMER_TABLE* ptNode);


/*************************************************************************
* �������ƣ�int ProcessTimer(void)
* ����˵�������¶�ʱ�����
* �����������
* �����������
* �� �� ֵ��SW_ERROR: ����ʧ��
           SW_OK �����ɹ�
* ����˵������
**************************************************************************/
extern int ProcessTimer(void);


#ifdef __cplusplus
}
#endif

#else

#define TIMER_ONESHOT		0
#define TIMER_PERIOD		1





typedef void (*app_func)(void);

typedef struct
{
	uint32	ticks;          //����
	uint32  overtime;       //ʱ�䵽
	//void (*func)(void);
	app_func fun;
	uint8    run;        //0-stop,1-run
	uint8    flag;     //ѭ�����ǵ���
} trIf_typedef;


typedef enum
{
    SRC_MAIN=0,
    SRC_MAX
}SRC_DEF;




extern void trIf_Init(uint8 id,app_func fun);

extern void trIf_start( uint8 id, uint32 ticks, uint8 flag);

extern void trIf_stop( uint8 id);

extern void trIf_Execute(void);
extern uint32 SystemTicksCount(void);


#define sys_ticks()			SystemTicksCount()
#define tick_timeout(x)		((int)(sys_ticks() - x) > 0)

#endif



#endif



/* end of file */

