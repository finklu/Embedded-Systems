#include <msp430.h>
#include "driver_general.h"
#include "driver_aktorik.h"

void Driver_Init(void)
{
  Driver_SteeringInit();
  Driver_ESCInit();
}
