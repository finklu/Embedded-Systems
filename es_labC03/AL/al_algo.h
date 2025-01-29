#ifndef _AL_ALGO_H_
#define _AL_ALGO_H_

#include <stdint.h>

enum states{FORWARD, LEFT, RIGHT, BACKWARDS, TODESKREISEL};
enum curves{C1, C2, C3, C4, C5, C6, C7, C8};

#define GOOD_ZONE 400

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
        short e, eold;
        signed short y;
        signed short satLow, satUp;
    } steer;
} PIDParam_t;

void AL_PARAMETER_Init();
void AL_Algorithm();

#endif
