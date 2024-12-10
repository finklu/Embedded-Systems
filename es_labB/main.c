
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
#include "DL/driver_distancetable.h"

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
    int i =0;

    while(1)
    {
        if(CCButton.active)
          {
           switch(CCButton.button)
              {
                  case START_BUTTON:
                      if(ADC12_Data.Status.B.ADCrdy == 1)
                              {
                                  ADC12_Data.Status.B.ADCrdy = 0;
                                  Driver_LCD_WriteText("v in mm/s =",12,0,0);
                                  Driver_LCD_WriteUInt(Measure_Hall_Data.speed,0,90);

                                  Driver_LCD_WriteText("Distance(R)=",13,2,0);
                                  Driver_LCD_WriteUInt(ir_right[ADC12_Data.ADCBuffer[0]>>3],2,90);

                                  Driver_LCD_WriteText("Distance(L)=",13,3,0);
                                  Driver_LCD_WriteUInt(ir_left[ADC12_Data.ADCBuffer[1]>>3],3,90);

                                  Driver_LCD_WriteText("Distance(F)=",13,4,0);
                                  Driver_LCD_WriteUInt(ir_front[ADC12_Data.ADCBuffer[2]>>3],4,90);

                                  Driver_LCD_WriteText("VBat in mV=",12,5,0);
                                  Driver_LCD_WriteUInt(ADC12_Data.ADCBuffer[3],5,90);

                              }
                      //Driver_SetThrottle(100);
                      CCButton.active = 0;
                  break;


                  case STOP_BUTTON:
                     // Driver_SetThrottle(0);
                      CCButton.active = 0;
                  break;
              }
          }

    }
}


