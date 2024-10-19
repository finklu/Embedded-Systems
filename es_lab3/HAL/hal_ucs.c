#include <msp430.h>
#include "hal_ucs.h"

void HAL_UCS_Init()
{
    UCSCTL6 &= ~XT2OFF;                                          // activate XT2 (dont turn off)
    UCSCTL6 &= XT2BYPASS;                                        // sourced from external crystal

    UCSCTL3 |= SELREF_2;                                         // FLL (frequency-locked loop) on REFOCLK (Reference Clock)
    UCSCTL4 |= SELA_2;                                           // ACLK (Auxillary clock) on REFOCLK

    while(SFRIFG1 & OFIFG)                                       // Wait until all error flags are cleared and not set again
    {
    UCSCTL7 &= ~(XT2OFFG + DCOFFG + XT1HFOFFG + XT1LFOFFG);
    SFRIFG1 &= ~OFIFG;
    }

    UCSCTL6 &= ~XT2DRIVE_3;                                      // reduce drive strength
    UCSCTL6 |= XT2DRIVE_2;                                       // operating range of 16MHz - 24MHz, 20MHz Oszillator

    UCSCTL4 |= SELS__XT2CLK;                                     // Selects the SMCLK source, 101b = XT2CLK
    UCSCTL4 |= SELM__XT2CLK;                                     // Selects the MCLK source, 101b = XT2CLK

    UCSCTL5 |= DIVS_3;                                           // SMCLK to 2.5MHz -> Source divider 8 (MCLK 20MHz)
}
