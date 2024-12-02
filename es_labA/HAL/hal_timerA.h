#ifndef _HAL_TIMERA_H_
#define _HAL_TIMERA_H_

#include <stdint.h>

#define PERIOD 10           //10Hz = 10 [1/s]
#define TICK_DISTANCE 5     //5mm

typedef struct
{
    volatile int32_t distance;
    volatile int32_t speed;
}hallMeasurement;


void HAL_TimerA_Init();

#endif
