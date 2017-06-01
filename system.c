

#include "system.h"


static void system_clock_init(void)
{
	OSCSTAT = 0;
	#if(SYSCLK_FREQ_32MHz == 1)
		OSCCON = 0b11110010;		//使用内部8M晶振,使能PLL
	#elif(SYSCLK_FREQ_16MHz== 1)
		OSCCON = 0b01111010;		//使用内部16M晶振
	#elif(SYSCLK_FREQ_8MHz == 1)
		OSCCON = 0b01110010;		//使用内部8M晶振
	#elif(SYSCLK_FREQ_4MHz == 1)
		OSCCON = 0b01101010;		//使用内部4M晶振
    #elif( SYSCLK_FREQ_2MHz  == 1)
		OSCCON = 0b01100010; 		//使用内部2M晶振
    #else
		OSCCON = 0b01011010;		//使用内部1M晶振
	#endif
 	while(!OSCSTATbits.HFIOFR);		//等待晶振稳定
}

void Init_MCU(void)
{
	system_clock_init(); //使用内部8M晶振

}


