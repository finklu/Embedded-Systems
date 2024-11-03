#include <msp430.h>
#include "hal_timerB0.h"
#include "hal_gpio.h"


extern TimerB0struct CCoperate;



void HAL_TimerB0_Init(void)
{
    TB0CTL |= TBSSEL_2 + MC_1 + ID_3;      // smclk as source + up mode + smclk divided by 8
    TB0EX0 |= TBIDEX_2;                    // smclk additionally divided by 3
    TB0CCTL0 |= CCIE;

    TB0CCR0 |= 0x65BA;                     // (2,5 MHz / 24) * 0,5 = 52083 ## count to 0.5s
    //__bis_SR_register(GIE);
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void TimerB0(void)
{
   // P8OUT ^= LCD_BL;
    TB0CCTL0 &= ~CCIFG;
}
