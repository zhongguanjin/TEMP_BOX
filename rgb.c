#include "rgb.h"




void rgb_init(void)
{
    RGB_CTL_OUT();
    rgb_ctr(ALL,OFF);
}

void rgb_ctr(uint8 mid,uint8 st)
{
    switch ( mid )
    {
        case RED :
            {
                if(st == ON)
                {
                    RED_CTL_PIN =ON;
                }
                else
                {
                   RED_CTL_PIN =OFF;
                }
                GREEN_CTL_PIN =OFF;
                BLUE_CTL_PIN =OFF;
                break;
            }
        case GREEN :
            {
                RED_CTL_PIN =OFF;
                if(st == ON)
                {
                   GREEN_CTL_PIN =ON;
                }
                else
                {
                   GREEN_CTL_PIN =OFF;
                }
                BLUE_CTL_PIN =OFF;

                break;
            }

        case BLUE :
            {
                RED_CTL_PIN =OFF;
                GREEN_CTL_PIN =OFF;
                if(st == ON)
                {
                    BLUE_CTL_PIN =ON;
                }
                else
                {
                   BLUE_CTL_PIN =OFF;
                }
                break;
            }
        case ALL:
            {
                if(st == OFF)
                {
                    RED_CTL_PIN =OFF;
                    GREEN_CTL_PIN =OFF;
                    BLUE_CTL_PIN =OFF;
                }
                else
                {
                    RED_CTL_PIN =ON;
                    GREEN_CTL_PIN =ON;
                    BLUE_CTL_PIN =ON;
                }
                break;
            }
        default:
            {
                RED_CTL_PIN =OFF;
                GREEN_CTL_PIN =OFF;
                BLUE_CTL_PIN =OFF;
                break;
            }
    }
}
