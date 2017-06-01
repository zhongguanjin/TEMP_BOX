#include "motor.h"

typedef struct
{
	uint16  current_pos;  //��ǰλ��
	uint16	target_pos;   //Ŀ��λ��
	uint16  offset;      //ƫ��ֵ
	uint16	dst;          //����
	uint8	div;          //�������
    uint8   bDirCur :1;    // ����λ 1-���з���Ϊ����
    uint8   bInPlace :1;   //��λ��־ 1-��λ
    uint8   bStopFlg :1;   //ֹͣ���
    uint8   bPluseFlg :1;  //�����ǣ�1-�����壬0-������
}PM_MOTOR;

PM_MOTOR PM[MOTOR_MAX];


typedef enum
{
    MICROSTEP_FULL = 0,
    MICROSTEP_1_2,
    MICROSTEP_1_4,
    MICROSTEP_1_16,
    MICROSTEP_MAX

}MOTOR_MICROSTEP;
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
    //IO����Ϊ���
    TRISA = 0B00001000     //RA3Ϊ���룬����Ϊ���
    TRISC = 0B11000100     //RC6��7��2Ϊ���� ����Ϊ���
    //�¶ȵ������
    TEMP_MOTOR_RST   = 1;
    TEMP_MOTOR_DIR   = 1;
    TEMP_MOTOR_STEP  = 1;
    TEMP_MOTOR_EN    = 0;     //0��Ч
    motor_step_set(TEMP_MOTOR,MICROSTEP_1_16); //1// 1/16��

    //�����������
    FLOW_MOTOR_RST 	 = 1;
    FLOW_MOTOR_DIR 	 = 1;
    FLOW_MOTOR_STEP  = 1;
    FLOW_MOTOR_EN 	 = 0;    //0��Ч
    motor_step_set(FLOW_MOTOR,MICROSTEP_1_16);  // 1/16��
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
     if(PM[mid].bStopFlg == 0) //���ж��Ƿ�ֹͣ
     {
      	if(mid == FLOW_MOTOR)  //�������
    	{
    		if(dir != 0)
    		{
    			PM[mid].bDirCur = 1;  //����
                FLOW_MOTOR_DIR  = 1;
    		}
    		else
    		{
    			PM[mid].bDirCur = 0;  //����
                FLOW_MOTOR_DIR  = 0;
    		}
    		FLOW_MOTOR_RST = 1;
    	}
        if(mid == TEMP_MOTOR)  //�¶ȵ��
            {
                if(dir != 0)
                {
                	PM[mid].bDirCur = 1;  //����
                    TEMP_MOTOR_DIR  = 1;
                }
                else
                {
                	PM[mid].bDirCur = 0;  //����
                    TEMP_MOTOR_DIR  = 0;
                }
                TEMP_MOTOR_RST = 1;
            }
	}
}

/*****************************************************************************
 �� �� ��  : ��������趨
 ��������  :
 �������  : ��
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2017��5��26�� ������
    ��    ��   : zgj
    �޸�����   : �����ɺ���

*****************************************************************************/
void motor_pluse_set(MOTOR_DEF mid, uint8 value)
{
	if(mid == FLOW_MOTOR)
	{
		FLOW_MOTOR_STEP = value;
	}
	else if(mid == TEMP_MOTOR)
	{
		FLOW_MOTOR_STEP = value;
	}
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
		FLOW_MOTOR_RST = 0;
	}
	else if(mid == TEMP_MOTOR)
	{
		TEMP_MOTOR_RST = 0;
	}
}


// ��������жϺ���
void TaskMotorFun(void)
{
   uint8 index;
   for(index = 0; index < MOTOR_MAX; index++)
	{
        if(PM[index].bPluseFlg != 0)
        {
            motor_pluse_set(index, OFF);
            PM[index].bPluseFlg = 0;
        } else
	    {   //����
            if(PM[index].current_step < PM[index].target_step)
            {
                motor_dir_set(index, 0)
                PM[index].current_step++;
                motor_pluse_set(index, ON)
                PM[index].bPluseFlg = 1;
            }
            else if(PM[index].current_pos > PM[index].target_pos)
            { // ����
                motor_dir_set(index, 1)
                PM[index].current_step++;
                motor_pluse_set(index, ON)
                PM[index].bPluseFlg = 1;
            }
        }
    }
}






























