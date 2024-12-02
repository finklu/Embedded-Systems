#ifndef _DRIVER_LCD_H_
#define _DRIVER_LCD_H_

#define LCD_RESET       0xE2
#define LCD_BIAS        0xA3
#define ADC_SEL_NORMAL  0xA0
#define COMMON_REVERSE  0xC8
#define RES_RATIO       0x24
#define ELEC_VOL_MODE   0x81
#define ELEC_VOL_VALUE  0x0F   //Kontrast Einstellung
#define POWER_CONT      0x2F
#define DISPLAY_ON      0xAF

#define LCD_DATA        (P8OUT=(P8OUT | LCD_DATACMD))       //A1 = 1
#define LCD_COMMAND     (P8OUT=(P8OUT & ~LCD_DATACMD))      //A0 = 0

#define SET_PAGE        0xB0   //0xB0 für erste Page, 0xB1 für zweite Page
#define LAST_PAGE       0xB7
#define MSB_COL_ADDR    0x10   //0x10, 0x11,.., 0x1F für spalten über 16
#define LSB_COL_ADDR    0x00   //0x01, 0x02,.., 0x0F für spalten bis 15

#define LCD_MAX_COLM    128
#define LCD_MAX_ROW     7

void Driver_LCD_Init(void);
void Driver_LCD_WriteCommand(unsigned char *data , unsigned char data_length);
void Driver_LCD_SetPosition(unsigned char page_sel , unsigned char col_sel);
void Driver_LCD_Clear(void);
void Driver_LCD_WriteText(char *text , unsigned char text_length , unsigned char page , unsigned char col);
void Driver_LCD_WriteUInt (unsigned int num, unsigned char page, unsigned char col);

#endif
