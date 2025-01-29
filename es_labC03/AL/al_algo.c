#include <msp430.h>
#include "al_algo.h"
#include "HAL/hal_adc12.h"
#include "HAL/hal_timerA.h"
#include "DL/driver_aktorik.h"
#include "DL/driver_lcd.h"

//----- AUSLENK PARAMETER ------
#define STEER_FORWARD            0
#define STEER_LEFT              -85
#define STEER_RIGHT             85

//----- SPEED PARAMETER ------
#define SPEED_SUPERFAST         85
#define SPEED_FAST              70
#define SPEED_MEDIUM            60
#define SPEED_SLOW              55
#define SPEED_CURVE             55
#define SPEED_BACKWARDS         -60
#define SPEED_STARK_BREMSEN     -30
#define SPEED_MEDIUM_BREMSEN    -15
#define SPEED_KAUM_BREMSEN      -10
#define SPEED_ANSTEHEN          10

//----- DISTANZ PARAMETER ------
#define DISTANCE_CLOSE            500
#define DISTANCE_MEDIUM           1400
#define DISTANCE_FAR              1480
#define DISTANCE_NACH_KURVE_SPEED 2000
#define DISTANCE_NACH_KURVE       20
#define DISTANCE_IN_KURVE_MAX     1000

//----- SPEED MEASURE PARAMETER ------
#define CURRSPEED_SLOW         1100
#define CURRSPEED_FAST         1700

//----- RICHTUNG ------
#define CW 0

extern ADC12Com ADC12_Data;
extern hallMeasurement Measure_Hall_Data;
PIDParam_t parameters;
Status_t Status;

short steeringValue = 0, throttleValue = 0;
unsigned long long int mm_in_curve = 0;


void AL_PARAMETER_Init()
{
    parameters.steer.kp = 0.07;     //0.08
    parameters.steer.kd = 0.001;    //0.003
    parameters.steer.fa = 60;

    parameters.steer.satLow = -40;
    parameters.steer.satUp = 40;

    Status.Steer.currState = FORWARD;
    Status.Count.leftCurves = 0;
    Status.Count.rightCurves = 0;
    Status.Count.rounds = 0;
    Status.start = 0;
    Status.refreshRate = 0;
    Status.algorithmusTimer = 0;

    Measure_Hall_Data.distance = 0;

}

void AL_SteerControl()
{
   parameters.steer.e = (ADC12_Data.SensorRight - ADC12_Data.SensorLeft);

   parameters.steer.y = (parameters.steer.kp *parameters.steer.e);                                               // P-Anteil
   parameters.steer.y += parameters.steer.kd * (parameters.steer.e -parameters.steer.eold)*parameters.steer.fa;  // D-Anteil

   parameters.steer.eold =parameters.steer.e;

    // Saturation
    if (parameters.steer.y >parameters.steer.satUp) {
        parameters.steer.y =parameters.steer.satUp;
    }
    if (parameters.steer.y <parameters.steer.satLow) {
        parameters.steer.y =parameters.steer.satLow;
    }
}


void AL_SpeedCalculation()
{
    switch(Status.Steer.currState) {
        case FORWARD:
            if (ADC12_Data.SensorFront <= DISTANCE_MEDIUM) {
                if (Measure_Hall_Data.speed > CURRSPEED_FAST) {
                    throttleValue = SPEED_MEDIUM_BREMSEN;
                }
                else {
                    throttleValue = SPEED_MEDIUM;
                }
            }

            else if (ADC12_Data.SensorFront <= DISTANCE_FAR) {
                throttleValue = SPEED_FAST;
            }
            else {
                throttleValue = SPEED_SUPERFAST;
            }
            break;

        case LEFT:
            if (Measure_Hall_Data.speed > CURRSPEED_SLOW) {
              throttleValue = SPEED_KAUM_BREMSEN;
            }
            else {
              throttleValue = SPEED_CURVE;
            }

            break;


        case RIGHT:
            if (Measure_Hall_Data.speed > CURRSPEED_SLOW) {
              throttleValue = SPEED_KAUM_BREMSEN;
            }
            else {
              throttleValue = SPEED_CURVE;
            }
            break;

        case BACKWARDS:
            throttleValue = SPEED_BACKWARDS;
            break;

        case TODESKREISEL:
            throttleValue = SPEED_CURVE;
            break;
    }


    Driver_SetThrottle(throttleValue);
}

void AL_Algorithm()
{
    AL_SteerControl();

    short sens_diff = ADC12_Data.SensorRight - ADC12_Data.SensorLeft;
    short sens_sum = ADC12_Data.SensorRight + ADC12_Data.SensorLeft;

    switch(Status.Steer.currState)
    {
        case FORWARD:
           if(ADC12_Data.SensorLeft >= 795) {
                Status.Steer.currState = LEFT;
            }
            else if(ADC12_Data.SensorRight >= 795) {
                Status.Steer.currState = RIGHT;
            }
            else {
                steeringValue = parameters.steer.y;
            }
            break;

        case LEFT:

            Status.Steer.lastSteer = STEER_LEFT;
            if(sens_sum <= ADC12_Data.SensorFront + 200 && sens_diff > -GOOD_ZONE) {
                Measure_Hall_Data.distance = 0;
                Status.Steer.currState = FORWARD;
            }
            else {
                steeringValue = STEER_LEFT;

            }
            break;

        case RIGHT:

            Status.Steer.lastSteer = STEER_RIGHT;
            if(sens_sum <= ADC12_Data.SensorFront + 200 && sens_diff < GOOD_ZONE){//} || Measure_Hall_Data.distance >= DISTANCE_IN_KURVE_MAX) {
                Measure_Hall_Data.distance = 0;
                Status.Steer.currState = FORWARD;
            }
            else if(CW == 1 && ADC12_Data.SensorRight <= 100){
                Status.Steer.currState = FORWARD;
            }
            else
            {
                steeringValue = STEER_RIGHT;
            }
            break;

        case BACKWARDS:

            steeringValue = Status.Steer.lastSteer;
            if(ADC12_Data.SensorFront >= 600)
            {
                Status.Steer.currState = FORWARD;
            }
            else if(ADC12_Data.SensorRight >= 780)
            {
                Status.Steer.currState = RIGHT;
            }
            else if(ADC12_Data.SensorLeft >= 780)
            {
                Status.Steer.currState = LEFT;
            }
            break;

        case TODESKREISEL:
            if(ADC12_Data.SensorFront >= DISTANCE_FAR)
            {
                Status.Steer.currState = FORWARD;
            }
            break;

    }

// ####### DEADLOCK ABFRAGE  ######

    if (ADC12_Data.SensorFront <= 10)
        {
          Status.Steer.deadlockCounter++;
          if (Status.Steer.deadlockCounter >= 30)
           {
             Status.Steer.currState = BACKWARDS;
             Status.Steer.deadlockTimer = 0;
           }
        }
         else
            {
             Status.Steer.deadlockCounter = 0;
            }

// ######### TODESKREISEL ABFRAGE  #############
/*
if(Measure_Hall_Data.distance >= DISTANCE_IN_KURVE_MAX)
{
    Status.Steer.currState = TODESKREISEL;
}
*/

    Driver_SetSteering(steeringValue);
    AL_SpeedCalculation();


  //  AL_DetermineCurve();
  //  AL_CalculateAlignment();
}
