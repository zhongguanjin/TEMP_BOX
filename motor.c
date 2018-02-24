#include "motor.h"
#include "dbg.h"
/*****************************************************************************
 �� �� ��  : Init_Motor
 ��������  : �����ʼ������
 �������  : void
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2017��6��1�� ������
    ��    ��   : zgj
    �޸�����   : �����ɺ���

*****************************************************************************/
void Init_Motor(void)
{

    A4982_FLOW_IOS_OUTPUT();
    A4982_TEMP_IOS_OUTPUT();
    //�¶ȵ������
    TEMP_MOTOR_EN = 0;                           //���ʹ��
    motor_step_set(TEMP_MOTOR,MICROSTEP_1_16); //1                                        // 1/16��
    PM[TEMP_MOTOR].cnt = 0;
    motor_stop(TEMP_MOTOR);
    dbg("init_motor\r\n");
    //�����������
    FLOW_MOTOR_EN = 0;                           //���ʹ��
    motor_step_set(FLOW_MOTOR,MICROSTEP_1_16); //1                                        // 1/16��
    PM[FLOW_MOTOR].cnt = 0;
    motor_stop(FLOW_MOTOR);

}
/*****************************************************************************
 �� �� ��  : motor_microstep_set
 ��������  : �����������
 �������  : MOTOR_DEF mid
             uint8 microstep_set_vaule
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2017��6��1�� ������
    ��    ��   : zgj
    �޸�����   : �����ɺ���

*****************************************************************************/
void motor_step_set(MOTOR_DEF mid, uint8 set_vaule)
{
    if(mid == FLOW_MOTOR)
    {
        //����(M1=0, M2=0)
        if(set_vaule == MICROSTEP_FULL)
        {
            FLOW_MOTOR_MS1 	 = 0;
            FLOW_MOTOR_MS2   = 0;
        }
        //1/2��(M1=1, M2=0)
        if(set_vaule == MICROSTEP_1_2)
        {
            FLOW_MOTOR_MS1 	 = 1;
            FLOW_MOTOR_MS2   = 0;
        }
        //1/4��(M1=0, M2=1)
        if(set_vaule == MICROSTEP_1_4)
        {
            FLOW_MOTOR_MS1 	 = 0;
            FLOW_MOTOR_MS2   = 1;
        }
        //1/16��(M1=1, M2=1)
        if(set_vaule == MICROSTEP_1_16)
        {
            FLOW_MOTOR_MS1   = 1;
            FLOW_MOTOR_MS2   = 1;

        }
   }
   if(mid == TEMP_MOTOR)
    {
        //����(M1=0, M2=0)
        if(set_vaule == MICROSTEP_FULL)
        {
            TEMP_MOTOR_MS1 	 = 0;
            TEMP_MOTOR_MS2   = 0;
        }
        //1/2��(M1=1, M2=0)
        if(set_vaule == MICROSTEP_1_2)
        {
            TEMP_MOTOR_MS1 	 = 1;
            TEMP_MOTOR_MS2   = 0;
        }
        //1/4��(M1=0, M2=1)
        if(set_vaule == MICROSTEP_1_4)
        {
            TEMP_MOTOR_MS1 	 = 0;
            TEMP_MOTOR_MS2   = 1;
        }
        //1/16��(M1=1, M2=1)
        if(set_vaule == MICROSTEP_1_16)
        {
            TEMP_MOTOR_MS1   = 1;
            TEMP_MOTOR_MS2   = 1;

        }
    }

}
/*****************************************************************************
 �� �� ��  : motor_dir_set
 ��������  : ��������������ú���
 �������  : MOTOR_DEF
             uint8 dir
 �������  : ��
 �� �� ֵ  : static
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2017��5��26�� ������
    ��    ��   : zgj
    �޸�����   : �����ɺ���

*****************************************************************************/
 void motor_dir_set( MOTOR_DEF mid, uint8 dir)
{
  	if(mid == FLOW_MOTOR)  //�������
	{
		if(dir != MOTOR_DIR_POSITIVE)
		{
			PM[mid].bDirCur = MOTOR_DIR_NEGATIVE;  //����
            FLOW_MOTOR_DIR  = MOTOR_DIR_NEGATIVE;
		}
		else
		{
			PM[mid].bDirCur = MOTOR_DIR_POSITIVE;  //����
            FLOW_MOTOR_DIR  = MOTOR_DIR_POSITIVE;
		}
	}
    if(mid == TEMP_MOTOR)  //�¶ȵ��
    {
        if(dir != MOTOR_DIR_POSITIVE)
        {
        	PM[mid].bDirCur = MOTOR_DIR_NEGATIVE;  //����
            TEMP_MOTOR_DIR  = MOTOR_DIR_NEGATIVE;
        }
        else
        {
        	PM[mid].bDirCur = MOTOR_DIR_POSITIVE;  //����
            TEMP_MOTOR_DIR  = MOTOR_DIR_POSITIVE;
        }
    }
}

/*****************************************************************************
 �� �� ��  : motor_pulse_set
 ��������  : �������ߵ͵�ƽ����
 �������  : MOTOR_DEF mid
             uint8 value
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2017��6��1�� ������
    ��    ��   : zgj
    �޸�����   : �����ɺ���

*****************************************************************************/
void motor_pulse_set(MOTOR_DEF mid, uint8 value)
{
	if(mid == FLOW_MOTOR)
	{
		FLOW_MOTOR_STEP = value;
	}
	else if(mid == TEMP_MOTOR)
	{
		TEMP_MOTOR_STEP = value;
	}
}

/*****************************************************************************
 �� �� ��  : motor_run_pulse
 ��������  : ����������岽��
 �������  : MOTOR_DEF mid
             uint8 pluse
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2017��6��1�� ������
    ��    ��   : zgj
    �޸�����   : �����ɺ���

*****************************************************************************/
void motor_run_pulse(MOTOR_DEF mid,uint16 pulse)
{
    //GIE=0;
    dbg("mp%d,p%d\r\n",mid,pulse);
    if(mid == FLOW_MOTOR)
    {
    	PM[FLOW_MOTOR].set = pulse;
    	FLOW_MOTOR_RST = MOTOR_SLEEP_OFF;          //�������
    	PM[FLOW_MOTOR].bRunFlg = 1;
    }
    else if(mid == TEMP_MOTOR)
    {
    	PM[TEMP_MOTOR].set = pulse;
    	TEMP_MOTOR_RST = MOTOR_SLEEP_OFF;
    	PM[TEMP_MOTOR].bRunFlg = 1;
    }
   // GIE=1;
}


uint16 motor_getPulse(MOTOR_DEF mid)
{

	uint16 pls;
	/*
	if(PM[mid].set != PM[mid].cnt)
	{
		if(PM[mid].bDirCur != 0) //����
		{
			pls = PM[mid].dst + PM[mid].set - PM[mid].cnt;
		}
		else  //����
		{
			pls = PM[mid].dst + PM[mid].cnt - PM[mid].set;
		}
	}
	else
	{
		pls = PM[mid].offset;
	}
	*/
	pls=PM[mid].cnt;
	return pls;
}

/*****************************************************************************
 �� �� ��  : motor_stop
 ��������  : �������ֹͣ
 �������  : uint8 num
 �������  : ��
 �� �� ֵ  : static
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2017��5��26�� ������
    ��    ��   : zgj
    �޸�����   : �����ɺ���

*****************************************************************************/
 void motor_stop(MOTOR_DEF mid)
{
    if(mid == FLOW_MOTOR)
	{
		FLOW_MOTOR_RST = MOTOR_SLEEP_ON; //�رյ��
		PM[FLOW_MOTOR].bRunFlg = OFF;
	}
	else if(mid == TEMP_MOTOR)
	{
		TEMP_MOTOR_RST = MOTOR_SLEEP_ON;
		PM[TEMP_MOTOR].bRunFlg = OFF;
	}
}


// ��������жϺ���
void TaskMotorFun(void)
{
   MOTOR_DEF mid;
   for(mid = 0; mid < MOTOR_MAX; mid++)
	{
	    if(PM[mid].bRunFlg == ON) //����
	    {
            if(PM[mid].bPluseFlg != 0) //������
            {
                motor_pulse_set(mid, OFF);
                PM[mid].bPluseFlg = 0;
            }
            else //������
    	    {
    	        //����
                if(PM[mid].cnt < PM[mid].set)
                {
                    PM[mid].cnt++;
                    motor_dir_set(mid, MOTOR_DIR_POSITIVE);
                    motor_pulse_set(mid, ON);
                    PM[mid].bPluseFlg = 1;
                }
                else if(PM[mid].cnt > PM[mid].set)
                {
                    PM[mid].cnt--;
                    motor_dir_set(mid, MOTOR_DIR_NEGATIVE);
                    motor_pulse_set(mid, ON);
                    PM[mid].bPluseFlg = 1;
                }
                else
                {
                   motor_stop(mid);
                   PM[mid].bPluseFlg = 0;
                }
            }
        }
    }
}






























