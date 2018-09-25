/******************************************************************************
* �ļ����ƣ�SoftTimer.c
* ����ժҪ�������ʱ��ģ��
* ����˵������������TimersInit����������ú����ṩ"1ms��ϵͳʱ��"��"���ϵͳms��"��
*         Ȼ���ڸ��Ե�Ӧ��ģ���е���CreatTimer������ʱ�����ú������صĵ�ַΪ�ö�
*         ʱ���ĵ�ַ��������������ɾ����ʱ����㣬�����Ʊ��ܡ�������ѭ����ִ��
*         ProcessTimer�����Ը��¶�ʱ��ʱ�䡣
* ��ǰ�汾��V1.00
* �� �ߣ�  zgj
* ������ڣ�2018��2��20��
******************************************************************************/

#include "SoftTimer.h"
#include "stdio.h"
#include "string.h"
#include<stdlib.h>

static TIMER_TABLE* sg_ptTimeTableHead = NULL;             /* �����ͷ       */
static TMRSOURCE    sg_pfSysClk        = NULL;             /* ϵͳ1msʱ�Ӻ��� */
static unsigned long    sg_dwTimeMaxValue  = MAX_VALUE_32_BIT; /* ���ms��       */

/*************************************************************************
* �������ƣ�int TimersInit(TMRSOURCE pfTimer)
* ����˵������ʼ�������ʱ��ģ��
* ���������TMRSOURCE pfTimer  ϵͳ1msʱ�Ӻ���
* �����������
* �� �� ֵ��SW_ERROR: ����ʧ��
           SW_OK �����ɹ�
* ����˵������
**************************************************************************/
int TimersInit(TMRSOURCE pfTimer)
{
    if (NULL == pfTimer)
    {
        return SW_ERROR; /* ���ע�ắ���Ƿ�Ϊ��ָ�� */
    }

    sg_ptTimeTableHead = (TIMER_TABLE*)malloc(sizeof(TIMER_TABLE)); /* ����ͷ��� */
    if (NULL == sg_ptTimeTableHead)
    {
        return SW_ERROR; /* ����Ƿ�����ɹ� */
    }

    /* ����ɹ�����г�ʼ�� */
    sg_ptTimeTableHead->next = NULL;               /* �¸�����ַ�ÿ�     */
    sg_pfSysClk              = (TMRSOURCE)pfTimer; /* ע��ϵͳ1msʱ�Ӻ���  */
    return SW_OK;
}

/*************************************************************************
* �������ƣ�TIMER_TABLE* CreatTimer(uint32 dwTimeout, uint8 ucPeriodic, TMRCALLBACK pfTimerCallback, void *pArg)
* ����˵�������������������ʱ��
* ���������uint32       dwTimeout  0~0xFFFFFFFF ��ʱʱ��
           uint8       ucPeriodic  SINGLE      ���δ���
                                   PERIODIC    ���ڴ���
           TMRCALLBACK pfTimerCallback         ��ʱ����ʱ�ص�����
           void       *pArg                    �ص���������

* �����������
* �� �� ֵ������ʧ�� : NULL
           �����ɹ� : ��ʱ��ģ��ָ��
* ����˵���������궨ʱ���󷵻ض�ʱ�����ĵ�ַ���ĵ�ַ����������ɾ���ö�ʱ��
**************************************************************************/
TIMER_TABLE* CreatTimer(uint32 dwTimeout, uint8 ucPeriodic, TMRCALLBACK pfTimerCallback, void *pArg)
{
    TIMER_TABLE* ptTimerNode;
    TIMER_TABLE* ptFind;
    if (NULL == sg_ptTimeTableHead)
    {
        return NULL; /* �������ͷ�ڵ��Ƿ���� */
    }

    /* ����ͷ����Ѿ����� */
    ptTimerNode = (TIMER_TABLE*)malloc(sizeof(TIMER_TABLE)); /* ���붨ʱ����� */
    if (NULL == ptTimerNode)
    {
        return NULL; /* ����Ƿ�����ɹ� */
    }

    /* �������ɹ� */
    ptTimerNode->next                 = NULL;                    /* �¸�����ַ�ÿ� */
    ptTimerNode->data.periodic        = ucPeriodic;              /* ����/���ڴ��� */
    ptTimerNode->data.start           = sg_pfSysClk();           /* ��ȡ��ʱ��ʼʱ�� */
    ptTimerNode->data.now             = ptTimerNode->data.start; /* ��ȡ��ǰʱ�� */
    ptTimerNode->data.elapse          = 0;                       /* �Ѿ�����ʱ�� */
    ptTimerNode->data.timeout         = dwTimeout;               /* ��ʱʱ�� */
    ptTimerNode->data.pfTimerCallback = pfTimerCallback;         /* ע�ᶨʱ�����ص����� */
    ptTimerNode->data.pArg            = pArg;                    /* �ص��������� */

    /* ��������Ķ�ʱ��������ӽ������� */
    ptFind = sg_ptTimeTableHead; /* ��������ͷ��� */
    while(NULL != ptFind->next)  /* �����ǰ��㲻��ĩβ���*/
    {
        ptFind = ptFind->next;   /* ����һ�����ĵ�ַ��Ϊ��ǰ���������� */
    }
    /* �ҵ�ĩβ��� */
    ptFind->next= ptTimerNode;   /* �������������ӵ�ĩβ��� */

    return ptTimerNode;          /* �����ɹ����������������ַ(����ɾ����������ʱ) */
}

#if 1

/*************************************************************************
* �������ƣ�int KillTimer(TIMER_TABLE* ptNode)
* ����˵����ɾ����ʱ�����
* ���������TIMER_TABLE* ptNode ��ʱ������ַ
* �����������
* �� �� ֵ��SW_ERROR: ����ʧ��
           SW_OK �����ɹ�
* ����˵������
**************************************************************************/
int KillTimer(TIMER_TABLE* ptNode)
{
    TIMER_TABLE* ptFind;

    if (NULL == ptNode)
    {
        return SW_ERROR; /* ��鶨ʱ������Ƿ�Ϊ�� */
    }

    /* ��ʱ����㲻Ϊ�� */
    ptFind = sg_ptTimeTableHead; /* ���ҵ�ͷ��� */
    while (ptFind) /* �������ĩβ��� */
    {
        if (ptFind->next == ptNode)      /* �����һ����Ƿ�Ϊ��ɾ����� */
        {
            ptFind->next = ptNode->next; /* �����������½�� */
            free(ptNode);                /* ɾ����� */
            return SW_OK;                /* �����ɹ����˳����� */
        }
        ptFind = ptFind->next;           /* ����������һ��� */
    }
    return SW_ERROR;                     /* δ�ҵ�������ʧ�� */
}


/*************************************************************************
* �������ƣ�int ResetTimer(TIMER_TABLE* ptNode)
* ����˵����������ʱ�����
* ���������TIMER_TABLE* ptNode ��ʱ������ַ
* �����������
* �� �� ֵ��SW_ERROR: ����ʧ��
           SW_OK �����ɹ�
* ����˵������
**************************************************************************/
int ResetTimer(TIMER_TABLE* ptNode)
{
    if (NULL == ptNode)
    {
        return SW_ERROR;                /* ��鶨ʱ������Ƿ�Ϊ�� */
    }

    ptNode->data.start = sg_pfSysClk(); /* ���¶�ʱ����ʼʱ�� */
    return SW_OK;                       /* �����ɹ� */
}


/*************************************************************************
* �������ƣ�int ProcessTimer(void)
* ����˵�������¶�ʱ�����
* �����������
* �����������
* �� �� ֵ��SW_ERROR: ����ʧ��
            SW_OK �����ɹ�
* ����˵������
**************************************************************************/
int ProcessTimer(void)
{
    TIMER_TABLE* ptFind;
    TIMER_TABLE* ptNodeFree;
    if (NULL == sg_ptTimeTableHead)
    {
        return SW_ERROR; /* ����Ƿ�����ɹ� */
    }
    ptFind = sg_ptTimeTableHead->next;    /* �ҵ���һ����Ч��� */
    while(ptFind)                         /* �������ĩβ��� */
    {
        ptFind->data.now = sg_pfSysClk(); /* ����ʱ�� */

        /* ����˿�ʱ������ʼʱ���ʱ��� */
        if(ptFind->data.now >= ptFind->data.start)
        {
            ptFind->data.elapse = ptFind->data.now - ptFind->data.start;
        }
        else
        {
            ptFind->data.elapse = sg_dwTimeMaxValue - ptFind->data.start + ptFind->data.now;
        }

        if(ptFind->data.elapse >= ptFind->data.timeout)          /* ���ʱ����ڵ����趨�ļ�ʱʱ�� */
        {
            if(ptFind->data.pfTimerCallback)                     /* ���Ѿ�ע���˺Ϸ��Ļص����� */
            {
                ptFind->data.pfTimerCallback(ptFind->data.pArg); /* ִ�лص����� */
            }
            if(ptFind->data.periodic)
            {
                ResetTimer(ptFind);                              /* ����������Դ�����������ʱ�� */
            }
            else
            {                                                    /* ����ǵ��δ�����ɾ����ʱ�� */
                ptNodeFree = ptFind;
                ptFind = ptFind->next;
                KillTimer(ptNodeFree);
                continue;
            }
        }
        ptFind = ptFind->next;                                   /* ����������һ����ʱ����� */
    }
    return SW_OK;                                                /* �����ɹ� */
}
#endif
/* end of file */

