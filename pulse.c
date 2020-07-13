#include "pulse.h"
#include "math.h"

#include "dbg.h"

speedRampData  srd   = {ST_STOP,0,0,0,0,0};        // �Ӽ������߱���

void motor_set_delay(int32 dat);
void motor_st_pro(void);



void motor_set_delay(int32 dat)
{

    PR6 = dat/2;
    TMR6IE = 1;
}



void motor_move(MOTOR_DEF mid,uint8 dir,uint32 pules)
{
    uint16 tim_count;
    uint32 max_s_lim;       // �ﵽ����ٶ�ʱ�Ĳ���
    uint32 accel_lim;       // ����Ҫ��ʼ���ٵĲ������������û�дﵽ����ٶȣ�
    dbg("mp%d,d%d,p%l\r\n",mid,dir,pules);
    if(PM[mid].bRunFlg != ON) //��������
    {
        if(dir == CW)           //����
        {
           motor_dir_set(TEMP_MOTOR, CW);
           PM[mid].bDirCur = CW;
        }
        else
        {
           motor_dir_set(TEMP_MOTOR, CCW);
           PM[mid].bDirCur = CCW;
        }
        srd.min_delay = (int32)A_T/V_max;                                      //��������ٶȼ���, ����õ�min_delay���ڶ�ʱ���ļ�������ֵ��

        srd.step_delay = (int32)((T_FREQ_148* sqrt(A_SQ / accel)));   //ͨ�������һ��(c0) �Ĳ�����ʱ���趨���ٶ�

        max_s_lim = (uint32)(V_max*V_max/(A_SQ*accel));                //���ٵ�����ٶ�����Ĳ���

        if(max_s_lim == 0)
        {
            max_s_lim = 1;
        }
        accel_lim = (uint32)(pules*decel/(accel+decel));                //������ٲ�֮�����Ǳ��뿪ʼ����/

        if(accel_lim == 0)
        {
            accel_lim = 1;
        }
        // ʹ�������������ǿ��Լ�������ٽ׶β���
        if(accel_lim <= max_s_lim)
        {
           srd.decel_val = pules - accel_lim;
        }
        else
        {
           srd.decel_val = max_s_lim*accel/decel;
        }
        // ��ֻʣ��һ�����Ǳ������
        if(srd.decel_val == 0)
        {
           srd.decel_val = -1;
        }
        // ���㿪ʼ����ʱ�Ĳ���
        srd.decel_start = pules - srd.decel_val;
        // �������ٶȺ��������ǾͲ���Ҫ���м����˶�
        if(srd.step_delay <= srd.min_delay)
        {
           srd.step_delay = srd.min_delay;
           srd.pm_st = ST_RUN;
        }
        else
        {
           srd.pm_st = ST_ACCEL;
        }
        dbg("min_delay:%l,step_delay:%l,max_s_lim:%l\r\n",srd.min_delay,srd.step_delay,max_s_lim);
        dbg("decel_start:%l,decel_val:%l,accel_lim:%l\r\n",srd.decel_start, srd.decel_val,accel_lim);

        // ��λ���ٶȼ���ֵ
        srd.accel_count = 0;
        motor_set_delay(srd.step_delay);
        PM[TEMP_MOTOR].set = pules;
        TEMP_MOTOR_RST = MOTOR_SLEEP_OFF;
        PM[TEMP_MOTOR].bRunFlg = 1;
        TEMP_MOTOR_RST = MOTOR_SLEEP_OFF;
    }
    else
    {
       dbg("err,motor run\r\n");
    }

}


void motor_st_pro(void)
{
     uint16 tim_count=0;
    // �����£��£�һ����ʱ����
    uint16 new_step_delay=0;
    // ���ٹ��������һ����ʱ���������ڣ�.
     static uint16 last_accel_delay=0;
    // ���ƶ�����������
     static uint32 step_count = 0;
    // ��¼new_step_delay�е������������һ������ľ���
     static int32 rest = 0;
    switch(srd.pm_st) // �Ӽ������߽׶�
    {
      case ST_STOP:
        step_count = 0;  // ���㲽��������
        rest = 0;        // ������ֵ
        break;
      case ST_ACCEL:
        step_count++;      // ������1
        srd.accel_count++; // ���ټ���ֵ��1
        new_step_delay = srd.step_delay - (((2 *srd.step_delay) + rest)/(4* srd.accel_count + 1));//������(��)һ����������(ʱ����)
        rest = ((2 * srd.step_delay)+rest)%(4 * srd.accel_count + 1);// �����������´μ��㲹���������������
        if(step_count >= srd.decel_start)// ����ǹ�Ӧ�ÿ�ʼ����
        {
          srd.accel_count = srd.decel_val; // ���ټ���ֵΪ���ٽ׶μ���ֵ�ĳ�ʼֵ
          srd.pm_st = ST_DECEL;           // �¸����������ٽ׶�
        }
        else if(new_step_delay <= srd.min_delay) // ����Ƿ񵽴�����������ٶ�
        {
          last_accel_delay = new_step_delay; // ������ٹ��������һ����ʱ���������ڣ�
          new_step_delay = srd.min_delay;    // ʹ��min_delay����Ӧ����ٶ�speed��
          rest = 0;                          // ������ֵ
          srd.pm_st = ST_RUN;               // ����Ϊ��������״̬
        }
        break;
      case ST_RUN:
        step_count++;  // ������1
        new_step_delay = srd.min_delay;     // ʹ��min_delay����Ӧ����ٶ�speed��
        if(step_count >= srd.decel_start)   // ��Ҫ��ʼ����
        {
          srd.accel_count = srd.decel_val;  // ���ٲ�����Ϊ���ټ���ֵ
          new_step_delay = last_accel_delay;// �ӽ׶�������ʱ��Ϊ���ٽ׶ε���ʼ��ʱ(��������)
          srd.pm_st = ST_DECEL;            // ״̬�ı�Ϊ����
        }
        break;
      case ST_DECEL:
        step_count++;  // ������1
        srd.accel_count--;
        new_step_delay = srd.step_delay + (((2 * srd.step_delay) + rest)/(4 * srd.accel_count + 1)); //������(��)һ����������(ʱ����)
        rest = ((2 * srd.step_delay)+rest)%(4 * srd.accel_count + 1);// �����������´μ��㲹���������������
        //����Ƿ�Ϊ���һ��
        if(srd.accel_count == 0)
        {
            srd.pm_st = ST_STOP;
	        step_count = 0;  // ���㲽��������
	        rest = 0;        // ������ֵ
        }
        break;
    }
    srd.step_delay = new_step_delay; // Ϊ�¸�(�µ�)��ʱ(��������)��ֵ
}


// ��������жϺ���
void TaskMotorISR(void)
{
    static uint8 ia=0;  //��ʱ��ʹ�÷�תģʽ����Ҫ���������жϲ����һ����������
    if(PM[TEMP_MOTOR].bRunFlg == ON) //����
    {
        if(PM[TEMP_MOTOR].bPluseFlg!= 0) //������
        {
            motor_pulse_set(TEMP_MOTOR, OFF);
            ia++;
            PM[TEMP_MOTOR].bPluseFlg = 0;
        }
        else //������
	    {
            motor_pulse_set(TEMP_MOTOR, ON);
            PM[TEMP_MOTOR].bPluseFlg = 1;
            ia++;
        }
    }
    if(ia==2)
    {
        ia=0;
        if(PM[TEMP_MOTOR].cnt != PM[TEMP_MOTOR].set)
        {
            PM[TEMP_MOTOR].cnt++;
            motor_st_pro();
            if(srd.pm_st == ST_STOP)
            {
                motor_stop(TEMP_MOTOR);
                TMR6IE = 0;
            }
            else
            {
                //dbg("%d",srd.step_delay);
                motor_set_delay(srd.step_delay);
            }
        }
        else
        {
            motor_stop(TEMP_MOTOR);
            TMR6IE = 0;
        }
    }
}
