#ifndef _HAL_TIMERB0_H_
#define _HAL_TIMERB0_H_


typedef struct
{
    unsigned char active;
    unsigned char state;
}TimerB0struct;

void HAL_TimerB0_Init(void);

#endif
