#include "DRV8837.h"
#include "system.h"

void drv_8837_config(void)
{
    TAP_8837_OUT();
    SHOWER_8837_OUT();
}

uint8 get_8837_state(DRV8837_DEF mid)
{
    if(mid== DRV8837_TAP)
    {
        return  tap_state;
    }
    else
    {
        return shower_state;
    }
}
void drv8837_ctr(DRV8837_DEF mid, uint8 state)
{
    switch ( mid )
    {
        case DRV8837_TAP:
            {
                if(state == ON)
                {
                    TAP_8837_IN1_PIN=1;
                    TAP_8837_IN2_PIN =0;
                    delay_ms(20);
                    TAP_8837_IN1_PIN =0;
                    TAP_8837_IN2_PIN =0;
                    tap_state = ON;
                }
                else
                {
                    TAP_8837_IN1_PIN=0;
                    TAP_8837_IN2_PIN =1;
                    delay_ms(20);
                    TAP_8837_IN1_PIN =0;
                    TAP_8837_IN2_PIN =0;
                    tap_state = OFF;
                }
            }
            break;
        case DRV8837_SHOWER:
            {
                if(state == ON)
                {
                    SHOWER_8837_IN1_PIN=1;
                    SHOWER_8837_IN2_PIN =0;
                    delay_ms(20);
                    SHOWER_8837_IN1_PIN =0;
                    SHOWER_8837_IN2_PIN =0;
                    shower_state = ON;
                }
                else
                {
                    SHOWER_8837_IN1_PIN=0;
                    SHOWER_8837_IN2_PIN =1;
                    delay_ms(20);
                    SHOWER_8837_IN1_PIN =0;
                    SHOWER_8837_IN2_PIN =0;
                    shower_state = OFF;
                }
            }
            break;
    }
}



