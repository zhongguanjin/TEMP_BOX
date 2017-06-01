

#include "system.h"


static void system_clock_init(void)
{
	OSCSTAT = 0;
	#if(SYSCLK_FREQ_32MHz == 1)
		OSCCON = 0b11110010;		//ʹ���ڲ�8M����,ʹ��PLL
	#elif(SYSCLK_FREQ_16MHz== 1)
		OSCCON = 0b01111010;		//ʹ���ڲ�16M����
	#elif(SYSCLK_FREQ_8MHz == 1)
		OSCCON = 0b01110010;		//ʹ���ڲ�8M����
	#elif(SYSCLK_FREQ_4MHz == 1)
		OSCCON = 0b01101010;		//ʹ���ڲ�4M����
    #elif( SYSCLK_FREQ_2MHz  == 1)
		OSCCON = 0b01100010; 		//ʹ���ڲ�2M����
    #else
		OSCCON = 0b01011010;		//ʹ���ڲ�1M����
	#endif
 	while(!OSCSTATbits.HFIOFR);		//�ȴ������ȶ�
}

void Init_MCU(void)
{
	system_clock_init(); //ʹ���ڲ�8M����

}


