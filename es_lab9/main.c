
#include <msp430.h> 
#include "HAL/hal_general.h"
#include "HAL/hal_timerA.h"
#include "HAL/hal_gpio.h"
#include "HAL/hal_timerB0.h"
#include "HAL/hal_usciB1.h"
#include "DL/driver_general.h"
#include "DL/driver_aktorik.h"
#include "DL/driver_lcd.h"
#include "HAL/hal_adc12.h"

extern ButtonCom CCButton;
extern TimerB0struct CCoperate;
extern USCIB1_SPICom SPI_LCD_Data;
extern hallMeasurement Measure_Hall_Data;
extern ADC12Com ADC12_Data;


void button(void);

void main(void)
{
    HAL_Init();
    Driver_Init();

    while(1)
    {
        if(ADC12_Data.Status.B.ADCrdy == 1)
        {
            ADC12_Data.Status.B.ADCrdy = 0;

            Driver_LCD_WriteText("Distance(R)=",13,0,0);
            Driver_LCD_WriteUInt(ADC12_Data.SensorRight,0,90);

            Driver_LCD_WriteText("Distance(L)=",13,2,0);
            Driver_LCD_WriteUInt(ADC12_Data.SensorLeft,2,90);

            Driver_LCD_WriteText("Distance(F)=",13,4,0);
            Driver_LCD_WriteUInt(ADC12_Data.SensorFront,4,90);

            Driver_LCD_WriteText("VBat in mV=",12,6,0);
            Driver_LCD_WriteUInt(ADC12_Data.VBat,6,90);

        }

    }
}


