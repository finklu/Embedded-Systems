#include <msp430.h>
#include "driver_aktorik.h"

int pulse_counter = 0;

void Driver_SteeringInit(void)
{
   Driver_SetSteering(0);
}


void Driver_SetSteering(int steering)
{
    int regValueSteer = 0;
    if(steering < 0)
    {
       (steering < -100) ? (steering = -100) : steering;
        regValueSteer =MIDDLE + (steering * STEP_SIZE);
    }

    else if(steering >= 0)
    {
        (steering > 100) ? (steering = 100) : steering;
        regValueSteer =MIDDLE + (steering * STEP_SIZE) ;
    }
    TA1CCR2 = regValueSteer;


}

void Driver_SetThrottle(int throttle)
{
    int regValueThrottle = 0;
    if(throttle < 0)
        {
           (throttle < -100) ? (throttle = -100) : throttle;
            regValueThrottle =5000 + (throttle * 25);
        }

        else if(throttle >= 0)
        {
            (throttle > 100) ? (throttle = 100) : throttle;
            regValueThrottle =7500 + (throttle * 25) ;
        }
        TA1CCR1 = regValueThrottle;

}


void Driver_ESCInit(void)
{
    pulseCreation(MAXRPW, 131);
    pulseCreation(MINRPW, 128);
    pulseCreation(MINFPW, 128);
    pulseCreation(MAXFPW, 128);

    pulseCreation(6000, 30);

}

void pulseCreation(int pwm, int pulses)
{
    pulse_counter = 0;
    TA1CCR1 = pwm;
    while(pulse_counter < pulses){}
}


#pragma vector = TIMER1_A0_VECTOR
__interrupt void TimerA1(void)
{
    pulse_counter++;
}

