#include <msp430.h>
#include "driver_aktorik.h"

void Driver_SteeringInit(void)
{
   Driver_SetSteering(0);
}




void Driver_SetSteering(int steering)
{
    int regValueSteer = 0;
    if(steering < 0)
        regValueSteer =MIDDLE + (steering * STEP_SIZE);
    else if(steering >= 0)
    {
        regValueSteer =MIDDLE + (steering * STEP_SIZE) ;
    }
    TA1CCR2 = regValueSteer;


}
