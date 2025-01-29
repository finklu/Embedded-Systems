#include <msp430.h> 
#include "HAL/hal_general.h"
#include "HAL/hal_adc12.h"
#include "HAL/hal_timerA.h"
#include "DL/driver_general.h"
#include "DL/driver_aktorik.h"
#include "DL/driver_lcd.h"
#include "AL/al_algo.h"
#include "AL/al_data.h"

#define DEBUG 0

extern ADC12Com ADC12_Data;
extern PIDParam_t parameters;
extern Status_t Status;
extern hallMeasurement Measure_Hall_Data;


void main(void)
{
  HAL_Init();
  Driver_Init();
  AL_PARAMETER_Init();

  while(1)
  {

   if(ADC12_Data.Status.B.ADCrdy == 1 && DEBUG)
   {
     if(Status.refreshRate == 512)
     {
      Status.refreshRate = 0;
      ADC12_Data.Status.B.ADCrdy = 0;

      Driver_LCD_WriteText("v in mm/s =",12,0,0);
      Driver_LCD_WriteUInt(Measure_Hall_Data.speed,0,70);

      Driver_LCD_WriteText("Distance(R)=",13,2,0);
      Driver_LCD_WriteUInt(ADC12_Data.SensorRight,2,70);

      Driver_LCD_WriteText("Distance(L)=",13,3,0);
      Driver_LCD_WriteUInt(ADC12_Data.SensorLeft,3,70);

      Driver_LCD_WriteText("Distance(F)=",13,4,0);
      Driver_LCD_WriteUInt(ADC12_Data.SensorFront,4,70);

      Driver_LCD_WriteText("VBat in mV=",12,5,0);
      Driver_LCD_WriteUInt(ADC12_Data.ADCBuffer[3],5,70);
     }
     else
     {
         Status.refreshRate++;
     }
   }

   if(Status.algorithmusTimer == 1)
   {
    Status.algorithmusTimer = 0;
    if(Status.start == 1)
    {
       AL_SensorDistances();
       AL_Algorithm();
    }
    else
      {
       Driver_SetThrottle(0);
      }
   }
  }
}


