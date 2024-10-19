#include <msp430.h>
#include "hal_gpio.h"

// #####   PORT1   #####
#define RPM_SENSOR          BIT3
#define RPM_SENSOR_DIR      BIT4
#define I2C_INT_MOTION      BIT5
#define START_BUTTON        BIT6
#define STOP_BUTTON         BIT7

// #####   PORT2   #####
#define DEBUG_TXD           BIT0
#define DEBUG_RXD           BIT1
#define AUX_PIN_1           BIT2
#define AUX_PIN_2           BIT3
#define AUX_PIN_3           BIT4
#define AUX_PIN_4           BIT5
#define I2C_SDA_MOTION      BIT6
#define I2C_SCL_MOTION      BIT7

// #####   PORT3   #####
#define THROTTLE            BIT2
#define STEERING            BIT3
#define SMCLK               BIT4
#define DISTANCE_FRONT_EN   BIT7

// #####   PORT4   #####
#define LINE_FOLLOW_2       BIT3
#define LINE_FOLLOW_1       BIT4
#define LINE_FOLLOW_3       BIT5
#define LINE_FOLLOW_4       BIT6
#define LINE_FOLLOW_5       BIT7

// #####   PORT6   #####
#define DISTANCE_RIGHT      BIT0
#define DISTANCE_LEFT       BIT1
#define DISTANCE_FRONT      BIT2
#define VBAT_MEASURE        BIT3
#define DISTANCE_LEFT_EN    BIT4

// #####   PORT7   #####
#define XT2IN               BIT2
#define XT2OUT              BIT3

// #####   PORT8   #####
#define LCD_BL              BIT0
#define LCD_SPI_CS          BIT1
#define UART_TXD_AUX        BIT2
#define UART_RXD_AUX        BIT3
#define LCD_SPI_CLK         BIT4
#define LCD_SPI_MOSI        BIT5
#define LCD_SPI_MISO        BIT6
#define LCD_DATACMD         BIT7

// #####   PORT9   #####
#define LCD_RESET           BIT0
#define DISTANCE_RIGHT_EN   BIT7



void HAL_GPIO_Init(void)
{


 // #####   PORT1   #####
  P1DIR &= ~RPM_SENSOR;
  P1DIR &= ~RPM_SENSOR_DIR;
  P1DIR |= I2C_INT_MOTION;
  P1DIR &= ~START_BUTTON;
  P1DIR &= ~STOP_BUTTON;

  P1DIR &= ~0x00;
  P1REN |= 0xFF;
  P1OUT &= ~0x00;

 // #####   PORT2   #####
  P2DIR &= ~DEBUG_TXD;
  P2DIR |= DEBUG_RXD;
  P2DIR &= ~AUX_PIN_1;
  P2DIR &= ~AUX_PIN_2;
  P2DIR &= ~AUX_PIN_3;
  P2DIR &= ~AUX_PIN_4;
  P2DIR &= ~I2C_SDA_MOTION;
  P2DIR |= I2C_SCL_MOTION;

 // #####   PORT3   #####
  P3DIR |= THROTTLE;
  P3DIR |= STEERING;
  P3DIR |= DISTANCE_FRONT_EN;

 // #####   PORT4   #####
  P4DIR &= ~LINE_FOLLOW_2;
  P4DIR &= ~LINE_FOLLOW_1;
  P4DIR &= ~LINE_FOLLOW_3;
  P4DIR &= ~LINE_FOLLOW_4;
  P4DIR &= ~LINE_FOLLOW_5;

 // #####   PORT6   #####
  P6DIR &= ~DISTANCE_RIGHT;
  P6DIR &= ~DISTANCE_LEFT;
  P6DIR &= ~DISTANCE_FRONT;
  P6DIR &= ~VBAT_MEASURE;
  P6DIR |= DISTANCE_LEFT_EN;

 // #####   PORT7   #####
  P7DIR &= ~XT2IN;
  P7DIR |= XT2OUT;

 // #####   PORT8   #####
  P8DIR |= LCD_BL;
  P8DIR |= LCD_SPI_CS;
  P8DIR |= UART_TXD_AUX;
  P8DIR &= ~UART_RXD_AUX;
  P8DIR |= LCD_SPI_CLK;
  P8DIR |= LCD_SPI_MOSI;
  P8DIR &= ~LCD_SPI_MISO;
  P8DIR |= LCD_DATACMD;

 // #####   PORT9   #####
  P9DIR |= LCD_RESET;
  P9DIR |= DISTANCE_RIGHT_EN;



}
