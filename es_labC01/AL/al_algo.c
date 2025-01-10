#include <msp430.h>
#include "al_algo.h"
#include "HAL/hal_adc12.h"
#include "HAL/hal_timerA.h"

extern ADC12Com ADC12_Data;
extern hallMeasurement Measure_Hall_Data;
PIDParam_t parameters;
Status_t Status;

short steeringValue = 0;

void AL_PARAMETER_Init()
{
    parameters.steer.kp = 0.11;
    parameters.steer.ki = 0;
    parameters.steer.kd = 0.05;
    parameters.steer.esum = 0;
    parameters.steer.ta = 0.1;
    parameters.steer.satLow = -100;
    parameters.steer.satUp = 100;

    Status.Steer.currState = FORWARD;
    Status.Count.leftCurves = 0;
    Status.Count.rightCurves = 0;
    Status.Count.rounds = 0;
    Status.start = 0;
    Status.refreshRate = 0;
}

void AL_SteerControl()
{
    parameters.steer.e = ADC12_Data.SensorRight - ADC12_Data.SensorLeft + Status.Steer.align;
     if ((parameters.steer.y >parameters.steer.satLow) && (parameters.steer.y <parameters.steer.satUp))
     {
       parameters.steer.esum += parameters.steer.e;
     }

   parameters.steer.y = (parameters.steer.kp *parameters.steer.e);                                               // P-Anteil
   parameters.steer.y +=parameters.steer.ki *parameters.steer.ta *parameters.steer.esum;                         // I-Anteil
   parameters.steer.y +=parameters.steer.kd * (parameters.steer.e -parameters.steer.eold) /parameters.steer.ta;  // D-Anteil

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
    short throttleValue = 0;
    switch(Status.Steer.currState) {
    case FORWARD:
        throttleValue = 60;//SPEED_FORWARD;
        break;
    case LEFT:
        throttleValue = 40;
        break;
    case RIGHT:
        throttleValue = 40;
        break;
    }
    Driver_SetThrottle(throttleValue);
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

    if(ADC12_Data.Status.B.ADCrdy == 1 && Status.refreshRate == 120)
       {
            ADC12_Data.Status.B.ADCrdy = 0;
            Driver_LCD_WriteText("v in mm/s =",12,0,0);
            Driver_LCD_WriteUInt(Measure_Hall_Data.speed,0,70);

            Driver_LCD_WriteText("Distance(R)=",13,2,0);
            Driver_LCD_WriteUInt(ADC12_Data.SensorRight,2,70);

            Driver_LCD_WriteText("Distance(L)=",13,3,0);
            Driver_LCD_WriteUInt(ADC12_Data.SensorLeft,3,70);

            Driver_LCD_WriteText("Distance(F)=",13,4,0);
            Driver_LCD_WriteUInt(ADC12_Data.SensorFront,4,70);

            Driver_LCD_WriteText("VBat in mV=",12,5,0);
            Driver_LCD_WriteUInt(ADC12_Data.ADCBuffer[3],5,70);
            Status.refreshRate = 0;
       }
       else {
           Status.refreshRate++;
       }

}



void AL_Algorithm()
{
    AL_SteerControl();

    short sens_diff = ADC12_Data.SensorRight - ADC12_Data.SensorLeft;
    short sens_sum = ADC12_Data.SensorRight + ADC12_Data.SensorLeft;

    switch(Status.Steer.currState) {
        case FORWARD:
            if(sens_diff < -GOOD_ZONE && ADC12_Data.SensorLeft > 750) {
                Status.Count.leftCurves++;
                Status.Steer.currState = LEFT;
            }
            else if(sens_diff > GOOD_ZONE && ADC12_Data.SensorRight > 750) {
                Status.Count.rightCurves++;
                Status.Steer.currState = RIGHT;
            }
            else {
                steeringValue = parameters.steer.y;
            }
            break;

        case LEFT:
            if(sens_sum <= ADC12_Data.SensorFront + 200 && sens_diff > -GOOD_ZONE) {
                Status.Steer.currState = FORWARD;
            }
            else {
                steeringValue = STEER_LEFT;
            }
            break;

        case RIGHT:
            if(sens_sum <= ADC12_Data.SensorFront + 200 && sens_diff < GOOD_ZONE) {
                Status.Steer.currState = FORWARD;
            }
            else {
                steeringValue = STEER_RIGHT;
            }
            break;
    }

    AL_DetermineCurve();
    AL_CalculateAlignment();


    Driver_SetSteering(steeringValue);

    if(Status.start) {
           AL_SpeedControl();
       }
       else {
           Driver_SetThrottle(0);
       }

    AL_ShowDisplay();
}

