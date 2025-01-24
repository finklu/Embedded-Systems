#ifndef _AL_ALGO_H_
#define _AL_ALGO_H_

#include <stdint.h>

enum states{FORWARD, LEFT, RIGHT, DEADLOCK};
enum curves{C1, C2, C3, C4, C5, C6, C7, C8};

#define STEER_RIGHT 85
#define STEER_LEFT -85
#define SPEED_FORWARD 60
#define SPEED_CURVE 40
#define GOOD_ZONE 450

typedef struct {
    struct {
        unsigned short currState;
        short lastSteer;
        unsigned short currCurve;
        short align;
        unsigned short deadlockTimer;
        unsigned short deadlockCounter;
    }Steer;
    struct {
        short throttleValue;
    }throttle;
    struct {
        unsigned short leftCurves;
        unsigned short rightCurves;
        unsigned short rounds;
    }Count;
    char algorithmusTimer;
    char start;
    unsigned short refreshRate;
}Status_t;

typedef struct {
    struct {
        float kp, kd, fa;
        int e, eold, y;
        short satLow, satUp;
    } steer;
    struct {
        float kp, ki, kd, esum, ta;
        short e, eold, y, satLow, satUp;
    } throttle;
} PIDParam_t;

void AL_PARAMETER_Init();
void AL_Algorithm();

#endif
