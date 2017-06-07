
#include "adc.h"



void Init_ADC(void)
{
   // PORTF ģ��ѡ��Ĵ���
    ANSELF = 0B10001100;              //ʹ��AD����RF2/AN7��RF3/AN8
    //PORTF ��̬�Ĵ���
	TRISF  |= 0B10001100;              //����Ӧ��AD��������Ϊ����
    ANSA3 = 1;
    TRISA3 = 1;
    ANSG4 = 1;
    TRISG4 = 1;
	//A/D ���ƼĴ���1
	ADFM   = 1;                         //�Ҷ���
	ADCS2  = 1;
	ADCS1 = 1;
	ADCS0 = 0;                      //ת��ʱ��  Fsoc/64  110
	ADPREF1 = 0;
	ADPREF0 = 0;                    //ѡ�����ο���ѹ�ڲ��ο���ѹ
	ADNREF  = 0;                      //ѡ�񸺲ο���ѹVSS
	// A/D ���ƼĴ���0
	ADCON0 = 0B00100001;		      //ѡ����Ӧͨ��AN8
	ADIF = 0;
	ADIE = 0;						 //����ADģ��

}

/*****************************************************************************
 �� �� ��  : Sel_Adc_Channel
 ��������  : ѡ��ͨ������
 �������  : uint8 ad_channel
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2017��5��25�� ������
    ��    ��   : zgj
    �޸�����   : �����ɺ���

*****************************************************************************/
void Sel_Adc_Channel(uint8 ad_channel)
{

     switch(ad_channel)
	{
	    case ADC_VREF:                      //��Դ��ѹAD
		{
		    ADCON0 = (AN_3<<2);
			//ADC_VAL[ADC_VREF] = read_tem_adc();
			break;
    	}
		case ADC_TEMP_IN:                   //�¶ȴ�����1
		{
		    ADCON0 = (AN_7<<2);
			//ADC_VAL[ADC_TEMP_IN] = read_tem_adc();
			break;
    	}
		case ADC_TEMP_MID:                   //�¶ȴ�����2
		{
		    ADCON0 = (AN_8<<2);
			//ADC_VAL[ADC_TEMP_MID] = read_tem_adc();
			break;
    	}
		case ADC_WATER_FLOW:                 //������
		{
			ADCON0 = (AN_12<<2);
           // ADC_VAL[ADC_WATER_FLOW] = read_tem_adc();
			break;
    	}
	}

}
/*****************************************************************************
 �� �� ��  :  read_tem_adc
 ��������  : ��ȡADת����ֵ��������������
 �������  : ��
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2016��5��13��
 ��    ��  : SJY
 �޸�����  : �����ɺ���

*****************************************************************************/
uint16 read_tem_adc(void)
{
    static uint8 ADDelay;
    uint16  AD_VAL;
    ADON=1;
    GO = 1;
	ADDelay = 0;
	while(GO)
	{
		ADDelay ++;
		if(ADDelay>80)
		{
			break;
		}
	}
	if(GO != 1)
	{
		AD_VAL = (uint16)(ADRESH<<8) + ADRESL;
	}

   return AD_VAL;
}



