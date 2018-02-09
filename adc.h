#ifndef __ADC_H__
#define __ADC_H__

#include "config.h"

typedef enum
{
    ADC_VREF_BAT = 0,        //电源电压  RF7-AN5
    ADC_TEMP_TAP,         //龙头温度     RF2-AN7
    ADC_TEMP_SHOWER,        //花洒温度     RA3-AN3
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

/****************************对外接口相关函数定义***********************************/
extern void Init_ADC(void);                   //ADC初始化
extern  uint16 read_tem_adc(void);            //读取AD转换结果
extern  void Sel_Adc_Channel(uint8 ad_channel);




#endif
