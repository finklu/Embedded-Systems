#include <msp430.h> 
#include "HAL/hal_general.h"
#include "HAL/hal_gpio.h"

#define LCD_BL_ON P8OUT |= LCD_BL
#define LCD_BL_OFF P8OUT &= ~LCD_BL

extern ButtonCom CCButton;


void multiply(void);

void main(void)
{
    HAL_Init();
    while(1)
    {

      // multiply();
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
    }

}

void multiply(void)
{
    int a=10;
    int b=23;

    float d=2.3435;
    float e=1.239;


    LCD_BL_ON;
    int c = a*b;
    LCD_BL_OFF;
    __delay_cycles(50);
    LCD_BL_ON;
    float f= d*e;
    LCD_BL_OFF;
    __delay_cycles(500);

}
