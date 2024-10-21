#ifndef _DRIVER_AKTORIK_H_
#define _DRIVER_AKTORIK_H_

#define MAX_LEFT 2750
#define MAX_RIGHT 4750
#define MIDDLE 3750
#define STEP_SIZE 10

void Driver_SteeringInit(void);
void Driver_SetSteering(int steering);

#endif
