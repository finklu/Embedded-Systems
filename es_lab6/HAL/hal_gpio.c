#include <msp430.h>
#include "hal_gpio.h"

extern ButtonCom CCButton;

void HAL_GPIO_Init()
{

 // #####   PORT1   #####

  P1DIR = 0xFF;
  P1OUT = 0x00;
  P1SEL = 0x00;
  P1DS = 0x00;

  P1DIR &= ~(RPM_SENSOR + RPM_SENSOR_DIR + START_BUTTON + STOP_BUTTON); //inputs
  P1DIR |= I2C_INT_MOTION;                                              //outputs
  P1REN |= START_BUTTON + STOP_BUTTON;                                  //resistor enable
  P1OUT |= START_BUTTON + STOP_BUTTON;                                  //pull-up resistor
  P1SEL |= RPM_SENSOR;                                                  //activate module TA0.2

 //## Button Interrupt ##

  P1IE  |= START_BUTTON + STOP_BUTTON;                                  //interrupt enable
  P1IES &= ~(START_BUTTON + STOP_BUTTON);                               //edge select: rise

 // #####   PORT2   #####

  P2DIR = 0xFF;
  P2OUT = 0x00;
  P2SEL = 0x00;
  P2DS = 0x00;

  P2DIR &= ~(DEBUG_TXD + AUX_PIN_1 + AUX_PIN_2 + AUX_PIN_3 +
          AUX_PIN_4 + I2C_SDA_MOTION) ;                                 //inputs
  P2DIR |= DEBUG_RXD + I2C_SCL_MOTION;                                  //outputs


 // #####   PORT3   #####

  P3DIR = 0xFF;
  P3OUT = 0x00;
  P3SEL = 0x00;
  P3DS = 0x00;

  P3DIR |= THROTTLE + STEERING + DISTANCE_FRONT_EN + SMCLK;                     //outputs
  P3SEL |= THROTTLE + STEERING + SMCLK;


 // #####   PORT4   #####

  P4DIR = 0xFF;
  P4OUT = 0x00;
  P4SEL = 0x00;
  P4DS = 0x00;

  P4DIR &= ~(LINE_FOLLOW_2 + LINE_FOLLOW_1 + LINE_FOLLOW_3 +
          LINE_FOLLOW_4 + LINE_FOLLOW_5);                               //inputs

 // #####   PORT5   #####

  P5DIR = 0xFF;
  P5OUT = 0x00;
  P5SEL = 0x00;
  P5DS = 0x00;

 // #####   PORT6   #####

  P6DIR = 0xFF;
  P6OUT = 0x00;
  P6SEL = 0x00;
  P6DS = 0x00;

  P6DIR &= ~(DISTANCE_RIGHT + DISTANCE_LEFT + DISTANCE_FRONT +
          VBAT_MEASURE);                                                //inputs
  P6DIR |= DISTANCE_LEFT_EN;                                            //outputs


 // #####   PORT7   #####

  P7DIR = 0xFF;
  P7OUT = 0x00;
  P7SEL = 0x00;
  P7DS = 0x00;

  P7SEL |= XT2IN + XT2OUT;


 // #####   PORT8   #####

  P8DIR = 0xFF;
  P8OUT = 0x00;
  P8SEL = 0x00;
  P8DS = 0x00;

  P8DIR |= LCD_DATACMD + UART_TXD_AUX;
  P8DIR &= ~UART_RXD_AUX;

  P8SEL |= LCD_BL + LCD_SPI_CS + LCD_SPI_MISO +  + LCD_SPI_CLK + LCD_SPI_MOSI;


 // #####   PORT9   #####

  P9DIR = 0xFF;
  P9OUT = 0x00;
  P9SEL = 0x00;
  P9DS = 0x00;

  P9DIR |= LCD_RESET + DISTANCE_RIGHT_EN;                               //outputs

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
