#include <msp430.h>
#include "driver_lcd.h"
#include "driver_fonttable.h"
#include "HAL/hal_gpio.h"
#include "HAL/hal_usciB1.h"

extern USCIB1_SPICom SPI_LCD_Data;
unsigned char LCD_Pos_Array[];

void Driver_LCD_Init(void)
{
  LCD_BL_ON;
  P9OUT = (P9OUT & ~LCD_RESET_PIN);     //Reset Line LOW
  __delay_cycles(500000);           //Wait for Display
  P9OUT = (P9OUT | LCD_RESET_PIN);      //Reset Line HIGH

 unsigned char LCD_Init_Array[9] = {
   LCD_RESET,
   LCD_BIAS,
   ADC_SEL_NORMAL,
   COMMON_REVERSE,
   RES_RATIO,
   ELEC_VOL_MODE,
   ELEC_VOL_VALUE,
   POWER_CONT,
   DISPLAY_ON };

 Driver_LCD_WriteCommand(LCD_Init_Array , 9);
 while(SPI_LCD_Data.Status.TxSuc == 0);

 Driver_LCD_Clear();                            // lcd alle pixel löschen
 while(SPI_LCD_Data.Status.TxSuc == 0);
 LCD_BL_ON;
 Driver_LCD_WriteText("ELEKTROTECHNIK", 15, 0,0);
 Driver_LCD_WriteText("DAS", 4, 1,0);
 Driver_LCD_WriteText("IST", 4, 2,0);
 Driver_LCD_WriteText("EIN", 4, 3,0);
 Driver_LCD_WriteText("TEST", 5, 4,0);
 while(SPI_LCD_Data.Status.TxSuc == 0);

}

void Driver_LCD_WriteCommand(unsigned char *data , unsigned char data_length)
{
 unsigned char i;
 while(SPI_LCD_Data.Status.TxSuc == 0);
 LCD_COMMAND;

 for(i=0; i< data_length; i++)
  {
    SPI_LCD_Data.TxData.Data[i] = *data;
    data++;
  }

 SPI_LCD_Data.TxData.len = data_length;
 SPI_LCD_Data.TxData.cnt = 0;
 HAL_USCIB1_Transmit();
 while(SPI_LCD_Data.Status.TxSuc == 0);
}

void Driver_LCD_SetPosition(unsigned char page_sel , unsigned char col_sel)
{

  unsigned char msn_col = (col_sel >> 4); //0001 xxxx shiften zu MSNibble Wert
  unsigned char lsn_col = col_sel & 0x0F; //0000 xxxx maskieren auf LSNibble Wert

  LCD_Pos_Array[0] = SET_PAGE + page_sel;                // Page-Wert muss als erstes gesetzt werden
  LCD_Pos_Array[1] = MSB_COL_ADDR + msn_col;             // MSB Column Address
  LCD_Pos_Array[2] = LSB_COL_ADDR + lsn_col;             // LSB Column Address

  Driver_LCD_WriteCommand(LCD_Pos_Array,3);              // Position ans LCD senden
  while(SPI_LCD_Data.Status.TxSuc == 0);                 // Warten bis Daten übertragen wurden

}

void Driver_LCD_Clear(void)
{
    unsigned char i,j;

    for(j=0x00; (SET_PAGE+j) <= LAST_PAGE; j++)
    {
     LCD_Pos_Array[0] = SET_PAGE+j;
     Driver_LCD_WriteCommand(LCD_Pos_Array, 3);

    //Wait for Transmission of the command
     LCD_DATA;

     for(i=0; i< LCD_MAX_COLM; i++)
      {
       SPI_LCD_Data.TxData.Data[i] = 0x00;
      }

     SPI_LCD_Data.TxData.len = LCD_MAX_COLM;
     HAL_USCIB1_Transmit();
     }
}


void Driver_LCD_WriteText(char *text , unsigned char text_length , unsigned char page , unsigned char col)
{
  unsigned char i, text_length_cnt, col_pos = col;

  Driver_LCD_SetPosition(page,col);
  while(SPI_LCD_Data.Status.TxSuc == 0);
  // Wait for Display
  LCD_DATA;

  for(text_length_cnt=0;text_length_cnt < text_length; text_length_cnt++)
 // Textlänge des Strings abarbeiten
   {
    for(i=0; i < fonts_width_max; i++) // Jedes Character des Strings
     {
      SPI_LCD_Data.TxData.Data[i] = font_table[*text][i];
      col_pos++;
     }

    SPI_LCD_Data.TxData.len =fonts_width_max;
    SPI_LCD_Data.TxData.cnt = 0;
    HAL_USCIB1_Transmit();
    while(SPI_LCD_Data.Status.TxSuc == 0); // Daten an das Display senden // Warten bis Übertragung fertig ist
/*
    if(col_pos >LCD_MAX_COLM)
     {
     Driver_LCD_SetPosition(++page, 1); // Column „overflow“?
     col_pos = 1;
     }
*/
   text++;
   }
}


void Driver_LCD_WriteUInt (unsigned int num, unsigned char page, unsigned char col)
{

}

