#include <msp430.h>
#include "hal_timerA1.h"


void HAL_TimerA1_Init(void)
{
  TA1CTL |= TASSEL_2 + MC_1;                //source select SMCLK + Up mode
  TA1CCTL0 |=  OUTMOD_7 + CCIE;                    //OUTMOD = RESET/SET + interrupt enable
  TA1CCTL1 |=  OUTMOD_7;
  TA1CCTL2 |=  OUTMOD_7;
  __bis_SR_register(GIE);


  TA1CCR0 = 41667;                        //60Hz

  TA1CCR1 = 0;
  TA1CCR2 = 0;

}
