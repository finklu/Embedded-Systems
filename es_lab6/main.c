
#include <msp430.h> 
#include "HAL/hal_general.h"
#include "HAL/hal_gpio.h"
#include "HAL/hal_timerB0.h"
#include "HAL/hal_usciB1.h"
#include "DL/driver_general.h"
#include "DL/driver_aktorik.h"

extern ButtonCom CCButton;
extern TimerB0struct CCoperate;
extern USCIB1_SPICom SPI_LCD_Data;

void button(void);

void main(void)
{
    HAL_Init();
    SPI_LCD_Data.TxData.len = 3;
    SPI_LCD_Data.TxData.Data[0] = 0xAA;
    SPI_LCD_Data.TxData.Data[1] = 0xBB;
    SPI_LCD_Data.TxData.Data[2] = 0xCC;
    HAL_USCIB1_Transmit();
    while(1)
    {

    }
}


