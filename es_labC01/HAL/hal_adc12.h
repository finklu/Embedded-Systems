#ifndef _HAL_ADC12_H_
#define _HAL_ADC12_H_

typedef struct {
    union{
        unsigned char R;
    struct {
            unsigned char ADCrdy:1; // Bit=1 wenn Daten übertragen wurden
            unsigned char dummy:7;
            }B;
        }Status;
    unsigned short ADCBuffer[4];
    unsigned short SensorFront;
    unsigned short SensorLeft;
    unsigned short SensorRight;
   }ADC12Com;

   ADC12Com ADC12_Data;

void HAL_ADC12_Init(void);

#endif
