#ifndef _DRIVER_AKTORIK_H_
#define _DRIVER_AKTORIK_H_

#define MAX_LEFT -100          //steering max left  reg:2750
#define MAX_RIGHT 100          //steering max right reg:4750
#define STRAIGHT 3750         //reg value driving straight
#define STEP_SIZE 10           //(MAX_RIGHT-STRAIGHT)/100

#define MAX_RPW 2500            //1000us  max backward
#define MIN_RPW 5000            //2000us  min backward
#define MIN_FPW 7500            //3000us  min forward
#define MAX_FPW 10000           //4000us  max forward
#define STEP_SIZE_THROTTLE 25   //(MAX_FPW-MIN_FPW)/100
#define MAXBACK -100
#define MAXFORW 100

void Driver_SteeringInit(void);

void Driver_SetSteering(signed char steering);
void Driver_SetThrottle(signed char throttle);

void pulseCreation(int pwm, int pulses);
void Driver_ESCInit(void);

#endif
