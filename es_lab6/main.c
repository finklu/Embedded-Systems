
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
    Driver_Init();

        SPI_LCD_Data.TxData.Data[0] = 0x03;
        SPI_LCD_Data.TxData.Data[1] = 0xFF;
        SPI_LCD_Data.TxData.Data[2] = 0xB4;
        SPI_LCD_Data.TxData.Data[3] = 0x09;
     Driver_SetThrottle(20);
    while(1)
    {
        if(SPI_LCD_Data.Status.TxSuc)
        {
            SPI_LCD_Data.TxData.len = 4;
            HAL_USCIB1_Transmit();
        }
    }
}


