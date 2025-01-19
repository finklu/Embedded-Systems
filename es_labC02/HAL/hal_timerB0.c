#include <msp430.h>
#include "hal_timerB0.h"
#include "hal_gpio.h"


extern TimerB0struct CCoperate;



void HAL_TimerB0_Init(void)
{
    TB0CTL |= TBSSEL_2 + MC_1;      // smclk as source + up mode          // smclk additionally divided by 3
    //TB0CCTL0 |= CCIE;
    TB0CCTL1 |=  OUTMOD_7;

    TB0CCR0 |= 20833;              // (2,5 MHz) * 0,00833 = 120Hz
    TB0CCR1 |= 0x1;                //10416


}
