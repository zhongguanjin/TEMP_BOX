#ifndef __ADC_H__
#define __ADC_H__

#include "config.h"

typedef enum
{
    ADC_VREF = 0,            //��Դ��ѹ  RA3-AN3
    ADC_TEMP_IN,         //�¶�1     RF2-AN7
    ADC_TEMP_MID,        //�¶�2     RF3-AN8
    ADC_WATER_FLOW,      //������    RG4-AN12
    ADC_MAX
};

/* ADC */
#define AN_0  0
#define AN_1  1
#define AN_2  2
#define AN_3  3
#define AN_4  4
#define AN_5  5
#define AN_6  6
#define AN_7  7
#define AN_8  8
#define AN_9  9
#define AN_10 10
#define AN_11 11
#define AN_12 12
#define AN_13 13
#define AN_14 14
#define AN_15 15
#define AN_16 16
#define AN_17 17
#define AN_18 18

/****************************����ӿ���غ�������***********************************/
extern void Init_ADC(void);                   //ADC��ʼ��
extern  uint16 read_tem_adc(void);            //��ȡADת�����
extern  void Sel_Adc_Channel(uint8 ad_channel);




#endif
