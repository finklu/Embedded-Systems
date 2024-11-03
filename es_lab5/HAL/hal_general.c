#include <msp430.h>
#include "hal_general.h"
#include "hal_wdt.h"
#include "hal_pmm.h"
#include "hal_gpio.h"
#include "hal_ucs.h"
#include "hal_timerB0.h"
#include "hal_timerA.h"

ButtonCom CCButton;
TimerB0struct CCoperate;

void HAL_Init(void)
{
    HAL_Wdt_Init();
    HAL_PMM_Init();
    HAL_GPIO_Init();
    HAL_UCS_Init();
    __enable_interrupt();
    HAL_TimerB0_Init();
    HAL_TimerA_Init();
}
