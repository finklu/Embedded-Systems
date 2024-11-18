#include <msp430.h>
#include "hal_usciB1.h"
#include "hal_gpio.h"

extern USCIB1_SPICom SPI_LCD_Data;

void HAL_USCIB1_Init(void)
{
   UCB1CTL1 |= UCSWRST;                 //software reset enable
   UCB1CTL1 |= UCSSEL__SMCLK;           //SMCLK source

   UCB1CTL0 &= ~(UCCKPH + UC7BIT);      //Clk phase 0 + 8-bit data
   UCB1CTL0 |= UCCKPL + UCMSB + UCMST;  //clk polarity 1 + MSB first + master mode
   UCB1CTL0 |= UCSYNC;                  //synchronous mode

   UCB1BR0 = SPI_FREQ;                  //SMCLK/100kHz
   UCB1BR1 = 0x00;


   UCB1CTL1 &= ~UCSWRST;                //software reset disabled
   UCB1IE |= UCRXIE;                    //Receive Interrupt enable
   SPI_LCD_Data.Status.TxSuc = 1;
}

void HAL_USCIB1_Transmit(void)
{
    SPI_LCD_Data.TxData.cnt = 0;
    SPI_LCD_Data.Status.TxSuc = 0;
    while(UCB1STAT & UCBUSY);

    LCD_CS_LOW;
    UCB1TXBUF = SPI_LCD_Data.TxData.Data[SPI_LCD_Data.TxData.cnt];
}

#pragma vector = USCI_B1_VECTOR
__interrupt void USCIB1(void)
{
//UCB1IFG &=  ~ UCTXIFG;
 if(UCB1IFG & UCRXIFG)
 {
     LCD_CS_HIGH;                               //cancel CS
     SPI_LCD_Data.RxData.Data[SPI_LCD_Data.TxData.cnt++] = UCB1RXBUF;
     SPI_LCD_Data.RxData.len++;

     if(SPI_LCD_Data.TxData.cnt > (SPI_LCD_Data.TxData.len-1))
     {
         SPI_LCD_Data.Status.TxSuc = 1;         // communication complete
         SPI_LCD_Data.TxData.cnt = 0;
         SPI_LCD_Data.TxData.len = 0;
     }
     else
     {
         LCD_CS_LOW;                            //continue CS
         UCB1TXBUF = SPI_LCD_Data.TxData.Data[SPI_LCD_Data.TxData.cnt];
     }
 }

}
