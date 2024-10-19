
#include <msp430.h> 
#include "HAL/hal_general.h"
#include "HAL/hal_gpio.h"
#include "HAL/hal_timerB0.h"

#define LCD_BL_ON P8OUT |= LCD_BL
#define LCD_BL_OFF P8OUT &= ~LCD_BL

extern ButtonCom CCButton;
extern TimerB0struct CCoperate;

void timerB0(void);

void main(void)
{
    HAL_Init();
    while(1)
    {
     //timerB0();
/*
     if(CCButton.active)
     {
         switch(CCButton.button)
         {
         case START_BUTTON: LCD_BL_ON;
         break;
         case STOP_BUTTON: LCD_BL_OFF;
         break;
         }
     }
*/
    }

}

void timerB0(void)
{
    if(CCoperate.active)
    {
        switch(CCoperate.state)
        {
            case 1: LCD_BL_ON;
            break;
            case 2: LCD_BL_OFF;
            break;
        }
    }
}

