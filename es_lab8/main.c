
#include <msp430.h> 
#include "HAL/hal_general.h"
#include "HAL/hal_timerA.h"
#include "HAL/hal_gpio.h"
#include "HAL/hal_timerB0.h"
#include "HAL/hal_usciB1.h"
#include "DL/driver_general.h"
#include "DL/driver_aktorik.h"
#include "DL/driver_lcd.h"

extern ButtonCom CCButton;
extern TimerB0struct CCoperate;
extern USCIB1_SPICom SPI_LCD_Data;
extern hallMeasurement Measure_Hall_Data;


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
                               Driver_SetThrottle(50);
                               CCButton.active = 0;
                           break;


                           case STOP_BUTTON:
                               Driver_SetThrottle(-50);
                               CCButton.active = 0;
                           break;
                       }
                   }
            Driver_LCD_WriteText("v in mm/s =",12,0,0);
            Driver_LCD_WriteUInt(Measure_Hall_Data.speed,0,70);

    }
}


