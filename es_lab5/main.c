
#include <msp430.h> 
#include "HAL/hal_general.h"
#include "HAL/hal_gpio.h"
#include "HAL/hal_timerB0.h"
#include "HAL/hal_usciB1.h"
#include "DL/driver_general.h"
#include "DL/driver_aktorik.h"

#define LCD_BL_ON P8OUT |= LCD_BL
#define LCD_BL_OFF P8OUT &= ~LCD_BL

extern ButtonCom CCButton;
extern TimerB0struct CCoperate;
extern USCIB1_SPICom SPIData;




void button(void);

void main(void)
{
    HAL_Init();
    Driver_Init();
    /*
    SPIData.TxData.len = 3;
    SPIData.TxData.Data[0] = 0xAA;
    SPIData.TxData.Data[1] = 0xBB;
    SPIData.TxData.Data[2] = 0xCC;
    HAL_USCIB1_Transmit();
    */
    while(1)
       {
           if(CCButton.active)
              {
                  switch(CCButton.button)
                  {
                      case START_BUTTON:
                      Driver_SetSteering(100);
                      CCButton.active = 0;
                      break;


                      case STOP_BUTTON:
                      Driver_SetSteering(-100);
                      CCButton.active = 0;
                      break;
                  }
              }
       }

}


