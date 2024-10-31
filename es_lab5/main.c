
#include <msp430.h> 
#include "HAL/hal_general.h"
#include "HAL/hal_gpio.h"
#include "HAL/hal_timerB0.h"
#include "DL/driver_general.h"

#define LCD_BL_ON P8OUT |= LCD_BL
#define LCD_BL_OFF P8OUT &= ~LCD_BL

extern ButtonCom CCButton;
extern TimerB0struct CCoperate;



void button(void);

void main(void)
{
    HAL_Init();
    Driver_Init();
    while(1)
    {
        if(CCButton.active)
           {
               switch(CCButton.button)
               {
                   case START_BUTTON:
                   Driver_SetThrottle(100);
                   CCButton.active = 0;
                   break;


                   case STOP_BUTTON:
                   Driver_SetThrottle(0);
                   CCButton.active = 0;
                   break;
               }
           }

    }

}


