#include "hal_timerA.h"
#include <msp430.h>
#include <stdint.h>
#include "AL/al_algo.h"

extern hallMeasurement Measure_Hall_Data = { .distance = 0, .speed = 0 };
extern Status_t Status;

static volatile uint16_t hall_ticks = 0;


void HAL_TimerA_Init(void)
{
  // #######    TimerA1    #########
  TA1CTL |= 0x00;                           //Control Register, init state
  TA1CTL |= TASSEL_2;                       //source select SMCLK

  TA1CCTL0 |=  CCIE;                        //interrupt enable
  TA1CCTL1 |=  OUTMOD_7 + OUT;              //OUTMOD: Reset/set + output high
  TA1CCTL2 |=  OUTMOD_7 + OUT;              //OUTMOD: Reset/set + output high


  TA1CCR0 = 41667;                          //60Hz
  TA1CCR1 = 0x00;                           //used for steering control
  TA1CCR2 = 0x00;                           //used for throttle control

  TA1CTL |= TACLR;
  TA1CTL |= MC_1;                           //up-mode selected



  // #######    TimerA0    #########
  TA0CCTL2 |= 0x00;                         //Capture/Compare register init state
  TA0CCTL2 |= CAP + CM_3 + SCS;             //enable capture mode + both edges + synchronize capture source to timer clock
  TA0CCTL2 |= CCIS_0 + CCIE;                //capture input from pin + interrupt enable

  TA0CCTL0 |= CCIE;
  TA0CCR0 = 62500;                          //10Hz

  TA0CTL |= 0x00;                           //Control Register, init state
  TA0CTL |= TASSEL_2 + ID_2;                //source select SMCLK + input source divider /4
  TA0CTL |= MC_2 + TACLR;                   //up-mode selected + start timer


  // #######   TimerA2     #########
  TA2CTL = 0x00;                            //Control Register, init state
  TA2CTL |= TASSEL_2;                       //source select SMCLK

  TA2CCTL0 |= CCIE;                         //enable interrupts

  TA2CCR0 = 41667;                          //60Hz

  TA2CTL |= MC_1 ;                         //up-mode selected

}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void TimerA0_CCR2(void)
{
    if(TA0CCTL2 & CCIFG)
    {
        hall_ticks++;
        Measure_Hall_Data.distance++;
        TA0CCTL2 &= ~CCIFG;
    }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void TimerA0_CCR0(void)
{
    static uint32_t speed_old = 0;
    if(hall_ticks == 0)
     {
        Measure_Hall_Data.speed = 0;
     }
    else
      {
        Measure_Hall_Data.speed = (hall_ticks *TICK_DISTANCE * PERIOD + speed_old)>>1; // (Ticks*5mm)*10[1/s] average of last 2 measurement
        speed_old = Measure_Hall_Data.speed;
      }

    hall_ticks = 0;
}

#pragma vector = TIMER2_A0_VECTOR
__interrupt void TimerA2_CCR0(void)
{
    Status.algorithmusTimer = 1;
}
