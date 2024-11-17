
#include <msp430.h> 
#include "HAL/hal_general.h"
#include "HAL/hal_gpio.h"
#include "HAL/hal_timerB0.h"
#include "DL/driver_aktorik.h"

#define LCD_BL_ON P8OUT |= LCD_BL
#define LCD_BL_OFF P8OUT &= ~LCD_BL

extern ButtonCom CCButton;
extern TimerB0struct CCoperate;



void button(void);

void main(void)
{
    HAL_Init();
    Driver_SteeringInit();
    while(1)
    {
        if(CCButton.active)
           {
               switch(CCButton.button)
               {
                   case START_BUTTON:
                   Driver_SetSteering(100);
                   CCButton.active = 0;
                   break;


                   case STOP_BUTTON:
                   Driver_SetSteering(-100);
                   CCButton.active = 0;
                   break;
               }
           }

    }

}


