#include "driver_measurement.h"

volatile uint16_t hall_ticks = 0;
volatile uint16_t speed = 0;
volatile uint16_t speed_old = 0;

uint16_t Driver_SpeedMeasurement(void)
{
    return speed;
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void TimerA0_CCR2(void)
{
    hall_ticks++;
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void TimerA0_CCR0(void)
{
    uint16_t speed = (hall_ticks * PERIOD + speed_old)>>1;
    speed_old = speed;

    hall_ticks = 0;
}
