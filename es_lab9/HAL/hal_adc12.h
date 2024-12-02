#ifndef _HAL_ADC12_H_
#define _HAL_ADC12_H_

typedef struct {
    union{
        unsigned char R;
    struct {
            unsigned char ADCrdy:1; // Bit=1 wenn Daten �bertragen wurden
            unsigned char dummy:7;
            }B;
        }Status;
    unsigned short SensorLeft;
    unsigned short SensorRight;
    unsigned short SensorFront;
    unsigned short VBat;
   }ADC12Com;


void HAL_ADC12_Init(void);

#endif
