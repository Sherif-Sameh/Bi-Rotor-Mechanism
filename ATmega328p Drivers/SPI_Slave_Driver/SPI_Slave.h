#ifndef SPI_M
#define SPI_M
#include <avr/io.h>
#define miso 12
#define mosi 11
#define ss 10
#define sck 13

void spiSetSlave(uint8_t mode);

void spiSendSlave(unsigned char sdata);

uint8_t spiReceiveSlave();
#endif