#include <HAL/hal_timerA.h>
#include <msp430.h>


void HAL_TimerA_Init(void)
{
  // #######    TimerA1    #########
  TA1CTL |= 0x00;                           //Control Register, init state
  TA1CTL |= TASSEL_2;                       //source select SMCLK

  TA1CCTL0 |=  OUTMOD_0 + CCIE;             //no ouput + interrupt enable
  TA1CCTL1 |=  OUTMOD_7;                    //OUTMOD: Reset/set
  TA1CCTL2 |=  OUTMOD_7;                    //OUTMOD: Reset/set


  TA1CCR0 = 41667;                          //60Hz
  TA1CCR1 = 0x00;                           //used for steering control
  TA1CCR2 = 0x00;                           //used for throttle control

  TA1CTL |= MC_1;                           //up-mode selected



  // #######    TimerA0    #########
  TA0CTL |= 0x00;                           //Control Register, init state
  TA0CTL |= TASSEL_2 + ID_2;                //source select SMCLK + input source divider /4

  TA0CCTL0 |= CCIE;
  TA0CCTL1 |= CAP + CM_3;
  TA1CCR0 = 62500;                          //10Hz

  TA0CTL |= MC_1;                           //up-mode selected

}
