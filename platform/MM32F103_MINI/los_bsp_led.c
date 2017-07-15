#include "los_bsp_led.h"

#include "bsp_led.h"  

#ifdef LOS_MM32F103
#include "led.h"
#endif

/******************************************************************************
    here include some special hearder file you need
******************************************************************************/



/*****************************************************************************
 Function    : LOS_EvbLedInit
 Description : Init LED device
 Input       : None
 Output      : None
 Return      : None
 *****************************************************************************/
void LOS_EvbLedInit(void)
{
    //add you code here.
//#ifdef LOS_MM32F103
//    LED_Init();
//#endif
	
	LED_GPIO_Config();
    return;
}

/******************************************************************************
 Function   £ºLOS_EvbLedControl
 Discription: Control led on or off
 Input      : index Led's index
              cmd   Led on or off
 Return     : None
*******************************************************************************/
void LOS_EvbLedControl(unsigned int index, unsigned int cmd)
{
    switch (index)
    {
        case LOS_LED1:
        {
            //add you code here.
//            #ifdef LOS_MM32F103
//                LED1 = cmd;
//            #endif
					LED1(cmd);
            break;
        }
        case LOS_LED2:
        {
            //add you code here.
//            #ifdef LOS_MM32F103
//                LED2 = cmd;
//            #endif
					LED2(cmd);
            break;
        }
        case LOS_LED3:
        {
            //add you code here.
//            #ifdef LOS_MM32F103
//                LED3 = cmd;
//            #endif
					LED3(cmd);
            break;
        }
        case LOS_LED4:
        {
            //add you code here.
            #ifdef LOS_MM32F103
                LED4 = cmd;
            #endif
            break;
        }
        default:
        {
            break;
        }
    }
    return;
}


