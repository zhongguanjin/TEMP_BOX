
#include "adc.h"

void Init_ADC(void)
{
   // PORTF 模拟选择寄存器
    ANSELF = 0B10001100;              //使能AD引脚RF2/AN7、RA3/AN3
    ANSELA = 0B10001000;              //使能AD引脚RA3/AN3
    //PORTF 三态寄存器
	TRISF  |= 0B10001100;              //将相应的AD引脚设置为输入

    ANSA3 = 1;

    TRISA3 = 1;
    ANSG4 = 1;
    TRISG4 = 1;
	//A/D 控制寄存器1
	ADFM   = 1;                         //右对齐
	ADCS2  = 1;
	ADCS1 = 1;
	ADCS0 = 0;                      //转换时钟  Fsoc/64  110
	ADPREF1 = 0;
	ADPREF0 = 0;
	ADNREF  = 0;                      //选择负参考电压VSS
	// A/D 控制寄存器0
	ADCON0 = 0B00100001;		      //选择相应通道AN8
	ADIF = 0;
	ADIE = 0;						 //开启AD模块

}

/*****************************************************************************
 函 数 名  : Sel_Adc_Channel
 功能描述  : 选择通道函数
 输入参数  : uint8 ad_channel
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2017年5月25日 星期四
    作    者   : zgj
    修改内容   : 新生成函数

*****************************************************************************/
void Sel_Adc_Channel(uint8 ad_channel)
{
     switch(ad_channel)
	{
	    case ADC_VREF_BAT:                      //电源电压AD
		{
		    ADCON0 = (AN_5<<2);
			break;
    	}
		case ADC_TEMP_TAP:                   //温度传感器1
		{
		    ADCON0 = (AN_7<<2);
			break;
    	}
		case ADC_TEMP_SHOWER:                   //温度传感器2
		{
		    ADCON0 = (AN_3<<2);
			break;
    	}
	}
}
/*****************************************************************************
 函 数 名  :  read_tem_adc
 功能描述  : 读取AD转换数值，存在数组里面
 输入参数  : 无
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2016年5月13日
 作    者  : SJY
 修改内容  : 新生成函数

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



