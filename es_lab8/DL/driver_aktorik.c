#include <msp430.h>
#include <stdint.h>
#include "driver_aktorik.h"

uint8_t pulse_counter = 0;

void Driver_SteeringInit(void)
{
   Driver_SetSteering(0);   //initialize steering to straight
}


void Driver_SetSteering(signed char steering)
{
    uint16_t regValueSteer = 0;                                     //Register steering value
    if(steering < 0)
    {
       (steering < MAX_LEFT) ? (steering = MAX_LEFT) : steering;    //when input<MAXLEFT set to MAXLEFT
        regValueSteer =STRAIGHT + (steering * STEP_SIZE);
    }

    else if(steering >= 0)
    {
        (steering > MAX_RIGHT) ? (steering = MAX_RIGHT) : steering; //when input>MAXRIGHT set to MAXRIGHT
        regValueSteer =STRAIGHT + (steering * STEP_SIZE) ;
    }
    TA1CCR2 = regValueSteer;                                        //update PWM signal for steering


}

void Driver_SetThrottle(signed char throttle)
{
    uint16_t regValueThrottle = 0;                                       //Register throttle value
    if(throttle < 0)
        {
           (throttle < MAXBACK) ? (throttle = MAXBACK) : throttle;       //when input<MAXRPW set to MAXRPW
            regValueThrottle =MIN_RPW + (throttle * STEP_SIZE_THROTTLE);
        }

        else if(throttle >= 0)
        {
            (throttle > MAXFORW) ? (throttle = MAXFORW) : throttle;      //when input>MAXFPW set to MAXFPW
            regValueThrottle =MIN_FPW + (throttle * STEP_SIZE_THROTTLE) ;
        }
        TA1CCR1 = regValueThrottle;                                      //update PWM signal for throttle

}



void Driver_ESCInit(void)
{
    pulseCreation(MAX_RPW, 131);         //Reset/Startup + Max. Reverse
    pulseCreation(MIN_RPW, 128);         //Min. Reverse
    pulseCreation(MIN_FPW, 128);         //Min. Forward
    pulseCreation(MAX_FPW, 128);

    pulseCreation(MAXBREAK, 30);            //>MinRPW <MINFWP Kablibrierung abgeschlossen

}

void pulseCreation(int pwm, int pulses)
{
    pulse_counter = 0;
    TA1CCR1 = pwm;
    while(pulse_counter < pulses){}     //wait until number of pulses occured
}


#pragma vector = TIMER1_A0_VECTOR
__interrupt void TimerA1(void)
{
    pulse_counter++;
}

