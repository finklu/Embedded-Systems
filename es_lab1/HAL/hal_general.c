#include <msp430.h>
#include "hal_general.h"
#include "hal_wdt.h"
#include "hal_pmm.h"
#include "hal_gpio.h"


void HAL_Init(void)
{
    HAL_Wdt_Init();
    HAL_PMM_Init();
    HAL_GPIO_Init();
}
