#include <msp430.h>
#include "hal_general.h"
#include "hal_wdt.h"
#include "hal_pmm.h"
#include "hal_gpio.h"
#include "hal_ucs.h"
#include "hal_timerB0.h"
#include "hal_timerA.h"
#include "hal_usciB1.h"

ButtonCom CCButton;
TimerB0struct CCoperate;

USCIB1_SPICom SPI_LCD_Data;

void HAL_Init(void)
{
    HAL_Wdt_Init();
    HAL_PMM_Init();
    HAL_GPIO_Init();
    HAL_UCS_Init();
    __enable_interrupt();
    HAL_TimerB0_Init();
    HAL_TimerA_Init();
    HAL_USCIB1_Init();
}
