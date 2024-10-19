#include <msp430.h>
#include "hal_general.h"

void HAL_Wdt_Init(void)
{
   WDTCTL = WDTPW | WDTHOLD;  // stop watchdog timer
}
