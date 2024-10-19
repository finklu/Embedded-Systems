#include <msp430.h>
#include "hal_gpio.h"

extern ButtonCom CCButton;

void HAL_GPIO_Init()
{
    /*
      P1DIR &= ~0x00;
      P1REN |= 0xFF;
      P1OUT &= ~0x00;
    */

 // #####   PORT1   #####
  P1DIR &= ~(RPM_SENSOR + RPM_SENSOR_DIR + START_BUTTON + STOP_BUTTON); //inputs
  P1DIR |= I2C_INT_MOTION;                                              //outputs
  P1IE  |= START_BUTTON + STOP_BUTTON;                                  //interrupt enable
  P1IES &= ~(START_BUTTON + STOP_BUTTON);                               //edge select: rise
  P1REN |= START_BUTTON + STOP_BUTTON;                                  //resistor enable
  P1OUT |= START_BUTTON + STOP_BUTTON;                                  //pull-up resistor

 // #####   PORT2   #####
  P2DIR &= ~(DEBUG_TXD + AUX_PIN_1 + AUX_PIN_2 + AUX_PIN_3 +
          AUX_PIN_4 + I2C_SDA_MOTION) ;                                 //inputs
  P2DIR |= DEBUG_RXD + I2C_SCL_MOTION;                                  //outputs


 // #####   PORT3   #####
  P3DIR |= THROTTLE + STEERING + DISTANCE_FRONT_EN;                     //outputs


 // #####   PORT4   #####
  P4DIR &= ~(LINE_FOLLOW_2 + LINE_FOLLOW_1 + LINE_FOLLOW_3 +
          LINE_FOLLOW_4 + LINE_FOLLOW_5);                               //inputs


 // #####   PORT6   #####
  P6DIR &= ~(DISTANCE_RIGHT + DISTANCE_LEFT + DISTANCE_FRONT +
          VBAT_MEASURE);                                                //inputs
  P6DIR |= DISTANCE_LEFT_EN;                                            //outputs


 // #####   PORT7   #####                                                      //inputs
  P7SEL |= XT2IN + XT2OUT;                                                      //outputs


 // #####   PORT8   #####
  P8DIR &= ~(UART_RXD_AUX + LCD_SPI_MISO);                              //inputs
  P8DIR |= LCD_BL + LCD_SPI_CS + UART_TXD_AUX + LCD_SPI_CLK +
          LCD_SPI_MOSI + LCD_DATACMD;                                   //outputs


 // #####   PORT9   #####
  P9DIR |= LCD_RESET + DISTANCE_RIGHT_EN;                               //outputs

   __enable_interrupt();
}


#pragma vector = PORT1_VECTOR
__interrupt void P1_ISR(void)
{
    switch(P1IFG)
    {
    case START_BUTTON:
        CCButton.active = 1;
        CCButton.button = START_BUTTON;
        P1IFG &= ~START_BUTTON;
    break;

    case STOP_BUTTON:
        CCButton.active = 1;
        CCButton.button = STOP_BUTTON;
        P1IFG &= ~STOP_BUTTON;
    break;

    default: P1IFG = 0x00;
    break;
    }

}
