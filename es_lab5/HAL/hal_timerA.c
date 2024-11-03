#include <HAL/hal_timerA.h>
#include <msp430.h>
#include <stdint.h>

volatile uint32_t speed = 0;
static volatile uint16_t hall_ticks = 0;

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
  TA0CCTL2 |= 0x00;                         //Capture/Compare register init state
  TA0CCTL2 |= CAP + CM_3 + SCS;             //enable capture mode + both edges + synchronize capture source to timer clock
  TA0CCTL2 |= CCIS_0 + CCIE;                //capture input from pin + interrupt enable

  TA1CCR0 = 62500;                          //10Hz

  TA0CTL |= 0x00;                           //Control Register, init state
  TA0CTL |= TASSEL_2 + ID_2;                //source select SMCLK + input source divider /4
  TA0CTL |= MC_2 + TACLR;                   //up-mode selected + start timer

}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void TimerA0_CCR2(void)
{
    if(TA0CCTL2 & CCIFG)
    {
        hall_ticks++;
    }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void TimerA0_CCR0(void)
{
    static uint32_t speed_old = 0;

    uint32_t speed = (hall_ticks * PERIOD + speed_old)>>1;
    speed_old = speed;

    hall_ticks = 0;
}

