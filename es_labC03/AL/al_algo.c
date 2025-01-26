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
#define SPEED_SUPERFAST         80
#define SPEED_FAST              70
#define SPEED_MEDIUM            60
#define SPEED_SLOW              55
#define SPEED_CURVE             55
#define SPEED_BACKWARDS         -45
#define SPEED_STARK_BREMSEN     -30
#define SPEED_MEDIUM_BREMSEN    -20
#define SPEED_KAUM_BREMSEN      -5
#define SPEED_ANSTEHEN          10

//----- DISTANZ PARAMETER ------
#define DISTANCE_CLOSE          500
#define DISTANCE_MEDIUM         1100
#define DISTANCE_FAR            1480
#define DISTANCE_IN_KURVE_MAX   500

//----- SPEED MEASURE PARAMETER ------
#define CURRSPEED_SLOW         1100
#define CURRSPEED_FAST         1700






extern ADC12Com ADC12_Data;
extern hallMeasurement Measure_Hall_Data;
PIDParam_t parameters;
Status_t Status;

short steeringValue = 0, throttleValue = 0;
unsigned long long int mm_in_curve = 0;


void AL_PARAMETER_Init()
{
    parameters.steer.kp = 0.08;     //0.08
    parameters.steer.kd = 0.001;    //0.003
    parameters.steer.fa = 60;

    parameters.steer.satLow = -30;
    parameters.steer.satUp = 30;

    Status.Steer.currState = FORWARD;
    Status.Count.leftCurves = 0;
    Status.Count.rightCurves = 0;
    Status.Count.rounds = 0;
    Status.start = 0;
    Status.refreshRate = 0;
    Status.algorithmusTimer = 0;

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
            if (ADC12_Data.SensorFront <= DISTANCE_CLOSE) {
                if (Measure_Hall_Data.speed > CURRSPEED_FAST) {
                    throttleValue = SPEED_STARK_BREMSEN;
                }
                 else if (Measure_Hall_Data.speed > CURRSPEED_SLOW) {
                     throttleValue = SPEED_MEDIUM_BREMSEN;
                 }
                  else {
                      throttleValue = SPEED_SLOW;
                  }
            }
            else if (ADC12_Data.SensorFront <= DISTANCE_MEDIUM) {
                if (Measure_Hall_Data.speed > CURRSPEED_FAST) {
                    throttleValue = SPEED_KAUM_BREMSEN;
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
    }


    Driver_SetThrottle(throttleValue);
}

void AL_ShowDisplay()
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

}


void AL_Algorithm()
{
    AL_SteerControl();

    short sens_diff = ADC12_Data.SensorRight - ADC12_Data.SensorLeft;
    short sens_sum = ADC12_Data.SensorRight + ADC12_Data.SensorLeft;

    switch(Status.Steer.currState)
    {
        case FORWARD:

           if(ADC12_Data.SensorLeft >= 780) {
               Measure_Hall_Data.distance = 0;
                Status.Steer.currState = LEFT;
            }
            else if(ADC12_Data.SensorRight >= 780) {
                Measure_Hall_Data.distance = 0;
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
            if(sens_sum <= ADC12_Data.SensorFront +200 && sens_diff < GOOD_ZONE){ // || Measure_Hall_Data.distance >= DISTANCE_IN_KURVE_MAX) {
                Status.Steer.currState = FORWARD;
            }
            else {
                steeringValue = STEER_RIGHT;

            }
            break;

        case BACKWARDS:

            if (Status.Steer.deadlockTimer <= 60)
             {
               steeringValue = STEER_FORWARD;
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

// ##################################

    Driver_SetSteering(steeringValue);
    AL_SpeedCalculation();


  //  AL_DetermineCurve();
  //  AL_CalculateAlignment();
  //  AL_ShowDisplay();
}
