
#include "hal_timerB0.h"
#include <msp430.h>
#include "hal_gpio.h"

extern TimerB0struct CCoperate;



void HAL_TimerB0_Init(void)
{
    TB0CTL |= TBSSEL_2 + MC_1 + ID_3;      // smclk as source + up mode + smclk divided by 8
    TB0EX0 |= TBIDEX_4;                    // smclk additionally divided by 5
    TB0CCTL0 |= CCIE;

    TB0CCR0 = 0x7A12;                      // (2,5 MHz / 40) * 0,5 = 31250 ## count to 0.5s
    __bis_SR_register(GIE);
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void TimerB0(void)
{

    switch(TB0CCTL0 & CCIFG)
    {
        case 1:
               CCoperate.active = 1;
               CCoperate.state = 1;
               TB0CCTL0 &= ~CCIFG;
        break;

        case 0:
            CCoperate.active = 1;
            CCoperate.state = 2;
            TB0CCTL0 &= ~CCIFG;
        break;

        default: TB0CCTL0 &= ~CCIFG;
        break;
    }

}
