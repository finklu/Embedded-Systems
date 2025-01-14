#include <msp430.h> 
#include "HAL/hal_general.h"
#include "HAL/hal_gpio.h"
#include "HAL/hal_adc12.h"
#include "HAL/hal_timerA.h"
#include "DL/driver_general.h"
#include "DL/driver_aktorik.h"
#include "DL/driver_lcd.h"
#include "AL/al_algo.h"
#include "AL/al_data.h"


extern ButtonCom CCButton;
extern ADC12Com ADC12_Data;
extern PIDParam_t parameters;
extern Status_t Status;


void main(void)
{
  HAL_Init();
  Driver_Init();
  AL_PARAMETER_Init();

  while(1)
  {
   if(Status.algorithmusTimer)
    {
      Status.algorithmusTimer = 0;
      if(Status.start == 1 && ADC12_Data.Status.B.ADCrdy == 1)
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


