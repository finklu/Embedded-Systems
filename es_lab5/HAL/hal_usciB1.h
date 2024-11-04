#ifndef _HAL_USCIB1_H_
#define _HAL_USCIB1_H_

#define SPI_FREQ 25 //reg wert 25000000/100000

typedef struct {

struct{
    unsigned char TxSuc;        // Bit=1 wenn Daten �bertragen wurden
 }Status;

struct {
    unsigned char len;          // L�nge der Daten in Bytes die �bertragen werden
    unsigned char cnt;          // Index auf momentan �bertragene Daten
    unsigned char Data[256];    // Tx Daten Array
 }TxData;

struct {
    unsigned char len;          // L�nge der empfangenen Daten
    unsigned char Data[256];    // Rx Daten Array
 }RxData;
}USCIB1_SPICom;

void HAL_USCIB1_Init(void);
void HAL_USCIB1_Transmit(void);

#endif
