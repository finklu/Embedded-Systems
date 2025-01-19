#include <msp430.h>
#include "al_algo.h"
#include "HAL/hal_adc12.h"
#include "HAL/hal_timerA.h"
#include "DL/driver_aktorik.h"
#include "DL/driver_lcd.h"

extern ADC12Com ADC12_Data;
extern hallMeasurement Measure_Hall_Data;
PIDParam_t parameters;
Status_t Status;

signed char steeringValue = 0;

void AL_PARAMETER_Init()
{
    parameters.steer.kp = 0.09;
    parameters.steer.ki = 0;
    parameters.steer.kd = 0.0001;
    parameters.steer.esum = 0;
    parameters.steer.ta = 0.01;

    parameters.steer.satLow = -100;
    parameters.steer.satUp = 100;

    Status.Steer.currState = FORWARD;
    Status.Count.leftCurves = 0;
    Status.Count.rightCurves = 0;
    Status.Count.rounds = 0;
    Status.start = 0;
    Status.refreshRate = 0;
    Status.algorithmusTimer = 0;

    parameters.throttle.kp = 1;
    parameters.throttle.ki = 0;
    parameters.throttle.kd = 0.04;
    parameters.throttle.esum = 0;
    parameters.throttle.ta = 0.1;
}

void AL_SteerControl()
{
    parameters.steer.e = ADC12_Data.SensorRight - ADC12_Data.SensorLeft /*+ Status.Steer.align*/;
     if ((parameters.steer.y >parameters.steer.satLow) && (parameters.steer.y <parameters.steer.satUp))
     {
       parameters.steer.esum += parameters.steer.e;
     }

   parameters.steer.y = (parameters.steer.kp *parameters.steer.e);                                               // P-Anteil
   parameters.steer.y += parameters.steer.ki *parameters.steer.ta *parameters.steer.esum;                         // I-Anteil
   parameters.steer.y += parameters.steer.kd * (parameters.steer.e -parameters.steer.eold)/parameters.steer.ta;  // D-Anteil

   parameters.steer.eold =parameters.steer.e;

    // Saturation
    if (parameters.steer.y >parameters.steer.satUp) {
        parameters.steer.y =parameters.steer.satUp;
    }
    if (parameters.steer.y <parameters.steer.satLow) {
        parameters.steer.y =parameters.steer.satLow;
    }
}

void AL_SpeedControl()
{


    parameters.throttle.e = Status.throttle.throttleValue - Measure_Hall_Data.speed / 40;

    parameters.throttle.y = (parameters.throttle.kp *parameters.throttle.e);                                               // P-Anteil
    parameters.throttle.y +=parameters.throttle.ki *parameters.throttle.ta *parameters.throttle.esum;                         // I-Anteil
    parameters.throttle.y +=parameters.throttle.kd * (parameters.throttle.e -parameters.throttle.eold) /parameters.throttle.ta;  // D-Anteil

    parameters.throttle.eold =parameters.throttle.e;

    Driver_SetThrottle(parameters.throttle.y);

}
void AL_SpeedCalculation()
{
    Status.throttle.throttleValue = 0;

    switch(Status.Steer.currState) {
    case FORWARD:
        if(ADC12_Data.SensorFront>=1500)
                {
                    Status.throttle.throttleValue = 65;
                }
                else if(ADC12_Data.SensorFront < 1500 && ADC12_Data.SensorFront > 800)
                {
                    Status.throttle.throttleValue = 50;
                }
                else if(ADC12_Data.SensorFront <= 800)
                {
                    //Status.throttle.throttleValue = 40;
                    Status.throttle.throttleValue = 40 - ADC12_Data.SensorFront / 200;
                }
        break;

    case LEFT:
        Status.throttle.throttleValue = 38;
        break;

    case RIGHT:
        Status.throttle.throttleValue = 38;
        break;

    case DEADLOCK:
        Status.throttle.throttleValue = -40;
        break;
    }


    Driver_SetThrottle(Status.throttle.throttleValue);


}

void AL_DetermineCurve()
{
    if (Status.Count.leftCurves == 1 && Status.Count.rightCurves == 0 && Status.Steer.currState == LEFT)
        Status.Steer.currCurve = C1;
    if (Status.Count.leftCurves == 2 && Status.Count.rightCurves == 0 && Status.Steer.currState == LEFT)
        Status.Steer.currCurve = C2;
    if (Status.Count.leftCurves == 3 && Status.Count.rightCurves == 0 && Status.Steer.currState == LEFT)
        Status.Steer.currCurve = C3;
    if (Status.Count.rightCurves == 1 && Status.Count.leftCurves == 3 && Status.Steer.currState == RIGHT)
        Status.Steer.currCurve = C4;
    if (Status.Count.rightCurves == 2 && Status.Count.leftCurves == 3 && Status.Steer.currState == RIGHT)
        Status.Steer.currCurve = C5;
    if (Status.Count.leftCurves == 4 && Status.Count.rightCurves == 2 && Status.Steer.currState == LEFT)
        Status.Steer.currCurve = C6;
    if (Status.Count.leftCurves == 5 && Status.Count.rightCurves == 2 && Status.Steer.currState == LEFT)
        Status.Steer.currCurve = C7;
    if (Status.Count.leftCurves == 6 && Status.Count.rightCurves == 2 && Status.Steer.currState == LEFT)
        Status.Steer.currCurve = C8;
}

void AL_CalculateAlignment()
{
    switch (Status.Steer.currCurve) {
        case C1:
            Status.Steer.align = 0;
            break;
        case C2:
            Status.Steer.align = 0;
            break;
        case C3:
            Status.Steer.align = 0;
            break;
        case C4:
            Status.Steer.align = 0;
            break;
        case C5:
            Status.Steer.align = 0;
            break;
        case C6:
            Status.Steer.align = 0;
            break;
        case C7:
            Status.Steer.align = 0;
            break;
        case C8:
            Status.Steer.align = 0;
            break;
        default:
            Status.Steer.align = 0;
            break;
    }
}

void AL_ShowDisplay()
{
            ADC12_Data.Status.B.ADCrdy = 0;
            Driver_LCD_WriteText("v in mm/s =",12,0,0);
            Driver_LCD_WriteUInt(Measure_Hall_Data.speed,0,90);

            Driver_LCD_WriteText("Distance(R)=",13,2,0);
            Driver_LCD_WriteUInt(ADC12_Data.SensorRight,2,70);

            Driver_LCD_WriteText("Distance(L)=",13,3,0);
            Driver_LCD_WriteUInt(ADC12_Data.SensorLeft,3,70);

            Driver_LCD_WriteText("Distance(F)=",13,4,0);
            Driver_LCD_WriteUInt(ADC12_Data.SensorFront,4,70);

            Driver_LCD_WriteText("VBat in mV=",12,5,0);
            Driver_LCD_WriteUInt(ADC12_Data.ADCBuffer[3],5,70);

            Driver_LCD_WriteText("Kurve=",12,7,0);
            Driver_LCD_WriteUInt(Status.Steer.currCurve,7,70);
}



void AL_Algorithm()
{
    AL_SteerControl();

    short sens_diff = ADC12_Data.SensorRight - ADC12_Data.SensorLeft;
    short sens_sum = ADC12_Data.SensorRight + ADC12_Data.SensorLeft;

    switch(Status.Steer.currState) {
        case FORWARD:
           if(ADC12_Data.SensorLeft >= 750) {
                Status.Count.leftCurves++;
                Status.Steer.currState = LEFT;
            }
            else if(ADC12_Data.SensorRight >= 750) {
                Status.Count.rightCurves++;
                Status.Steer.currState = RIGHT;
            }
            else {
                steeringValue = parameters.steer.y;
            }
            break;

        case LEFT:
            Status.Steer.lastSteer = STEER_LEFT;
            if(sens_sum <= ADC12_Data.SensorFront + 200 && sens_diff > -GOOD_ZONE) {
                Status.Steer.currState = FORWARD;
            }
            else {
                steeringValue = STEER_LEFT;

            }
            break;

        case RIGHT:
            Status.Steer.lastSteer = STEER_RIGHT;
            if(sens_sum <= ADC12_Data.SensorFront + 200 && sens_diff < GOOD_ZONE) {
                Status.Steer.currState = FORWARD;
            }
            else {
                steeringValue = STEER_RIGHT;

            }
            break;

        case DEADLOCK:

            if (Status.Steer.deadlockTimer < 90) // 1.5sec backwards (90 cycles / 60Hz algo)
             {
               steeringValue = Status.Steer.lastSteer;
               Status.Steer.deadlockTimer++;
             }
             else
              {
               Status.Steer.currState = FORWARD;
               Status.Steer.deadlockCounter = 0;
              }
            break;


    }

// ####### DEADLOCK ABFRAGE  ######
    if (ADC12_Data.SensorFront <= 10)
    {
      Status.Steer.deadlockCounter++;
      if (Status.Steer.deadlockCounter >= 90)      // 1.5sec in deadlock (90 cycles / 60Hz algo)
       {
         Status.Steer.currState = DEADLOCK;
         Status.Steer.deadlockTimer = 0;
       }
    }
     else
        {
         Status.Steer.deadlockCounter = 0;
        }
// ##################################

    Driver_SetSteering(steeringValue);
    AL_SpeedCalculation();


    AL_DetermineCurve();
    AL_CalculateAlignment();
    AL_ShowDisplay();
}

