#include <msp430.h>
#include "hal_adc12.h"

extern ADC12Com ADC12_Data;

void HAL_ADC12_Init()
{
    ADC12CTL0 &= ~ADC12ENC;
    ADC12CTL0 |= ADC12ON;

    REFCTL0 |= REFON + REFOUT;
    REFCTL0 |= REFVSEL_3;           //2.5V


    ADC12CTL0 |= ADC12SHT0_15;      //1024 ADC12CLK cycles
    ADC12CTL0 &= ~ADC12MSC;         //Trigger on rising edge
    ADC12CTL0 |= ADC12REF2_5V;      //2.5V
    ADC12CTL0 |= ADC12REFON;        //reference on

    ADC12CTL1 |= ADC12SHS_3;        //Timer Sauce TimerB0
    ADC12CTL1 |= ADC12SHP;          //SAMPCON Signal von sampling timer
    ADC12CTL1 |= ADC12SSEL_3;       //SMCLK as Sauce
    ADC12CTL1 |= ADC12DIV_3;        //clock divided by 4 = 0.625MHz
    ADC12CTL1 |= ADC12CONSEQ_3;     //Repeat-sequence of channels

    ADC12CTL2 |= ADC12RES_2;        //12-bit resolution
    ADC12MCTL0 |= ADC12SREF_1 + ADC12INCH_0;                 // ref+=AVcc, channel = A0
    ADC12MCTL1 |= ADC12SREF_1 + ADC12INCH_1;                 // ref+=AVcc, channel = A1
    ADC12MCTL2 |= ADC12SREF_1 + ADC12INCH_2;                 // ref+=AVcc, channel = A2
    ADC12MCTL3 |= ADC12SREF_1 + ADC12INCH_3 + ADC12EOS;        // ref+=AVcc, channel = A3, end seq.

    ADC12IE |= 0x08;                //enable interrupt of A3

    ADC12CTL0 |= ADC12ENC;
    ADC12CTL0 |= ADC12SC;           // Start conversion

    ADC12_Data.Status.B.ADCrdy = 0;
}

#pragma vector = ADC12_VECTOR
__interrupt void ADC12(void)
{
    if(ADC12IFG & ADC12IFG3)
    {
        ADC12_Data.SensorRight = ADC12MEM0;
        ADC12_Data.SensorLeft = ADC12MEM1;
        ADC12_Data.SensorFront = ADC12MEM2;
        ADC12_Data.VBat = ADC12MEM3;

        ADC12_Data.Status.B.ADCrdy = 1;
    }
}
