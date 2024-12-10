#include <msp430.h>
#include "hal_dma.h"
#include "hal_adc12.h"

extern ADC12Com ADC12_Data;

void HAL_DMA_Init(void)
{
    DMACTL0 |= DMA0TSEL_24;     //trigger: ADC12IF
    DMA0CTL |= DMADT_5;         //repeated burst block transfer

    DMA0CTL |= DMADSTINCR_3;    //destination address incremented
    DMA0CTL |= DMASRCINCR_3;    //source address incremented

    DMA0CTL &= ~DMADSTBYTE;     //word as destination
    DMA0CTL &= ~DMASRCBYTE;     //word as source

    DMA0CTL |= DMAIE;           //interrupt enable

    DMA0SA = &ADC12MEM0;                    //Source address -> ADC Memory Register
    DMA0DA = &ADC12_Data.ADCBuffer[0];      //destination address -> ADCBuffer
    DMA0SZ = 0x04;

    DMA0CTL |= DMAEN;           //dma enable

}

#pragma vector = DMA_VECTOR
__interrupt void DMA_ISR(void)
{
   if (DMA0CTL & DMAIFG)
       {
           ADC12_Data.Status.B.ADCrdy = 1;
           DMA0CTL &= ~DMAIFG;
       }
}
