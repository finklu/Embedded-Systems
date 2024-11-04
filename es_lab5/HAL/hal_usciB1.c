#include <msp430.h>
#include "hal_usciB1.h"
#include "hal_gpio.h"

extern USCIB1_SPICom SPIData;

void HAL_USCIB1_Init(void)
{
   P8DIR |= LCD_SPI_CS;
   P8OUT |= LCD_SPI_CS;

   UCB1CTL1 |= UCSWRST;                 //software reset enable

   UCB1CTL0 &= ~(UCCKPH + UC7BIT);      //Clk phase 0 + 8-bit data
   UCB1CTL0 |= UCCKPL + UCMSB + UCMST;  //clk polarity 1 + MSB first + master mode
   UCB1CTL0 |= UCMODE_0 + UCSYNC;       //3-pin SPI with UCxSTE active low + synchronous mode

   UCB1CTL1 |= UCSSEL_2;                //SMCLK source

   UCB1BR0 = SPI_FREQ;                  //SMCLK/100kHz
   UCB1BR1 = 0;


   UCB1CTL1 &= ~UCSWRST;                //software reset disabled
   UCB1IE |= UCRXIE;                    //Receive Interrupt enable


}

void HAL_USCIB1_Transmit()
{
    P8OUT &= ~LCD_SPI_CS;
    SPIData.Status.TxSuc = 0;
    UCB1TXBUF = SPIData.TxData.Data[SPIData.TxData.cnt];
}

#pragma vector = USCI_B1_VECTOR
__interrupt void USCIB1(void)
{

 if(UCB1IE & UCRXIE)
 {
     if(SPIData.TxData.cnt > SPIData.TxData.len-1)
     {
         P8OUT |= LCD_SPI_CS;
         SPIData.TxData.cnt = 0;
         SPIData.TxData.len = 0;

         SPIData.Status.TxSuc = 1;
     }
     else
     {
         SPIData.RxData.Data[SPIData.TxData.cnt] = UCB1RXBUF;
         SPIData.RxData.len = SPIData.TxData.cnt;
         SPIData.TxData.cnt++;

         UCB1TXBUF = SPIData.TxData.Data[SPIData.TxData.cnt];
     }
 }

}
