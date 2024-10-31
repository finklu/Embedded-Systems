#ifndef _DRIVER_AKTORIK_H_
#define _DRIVER_AKTORIK_H_

#define MAX_LEFT 2750
#define MAX_RIGHT 4750
#define MIDDLE 3750
#define STEP_SIZE 10

#define MAXRPW 2500          //1000us
#define MINRPW  5000          //2000us
#define MINFPW  7500          //3000us
#define MAXFPW  10000         //4000us

void Driver_SteeringInit(void);
void Driver_SetSteering(int steering);
void pulseCreation(int pwm, int pulses);
void Driver_ESCInit(void);

#endif
