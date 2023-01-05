#include "SPI_Slave.h"

void spiSetSlave(uint8_t mode)
{
    DDRB = (1<<PB4);
    SPCR = (1 << SPE);
    if (mode == 0)
        SPCR |= 0x00;
    if (mode == 1)
        SPCR |= 0x08;
    else if (mode == 2)
        SPCR |= 0x04;
    else if (mode == 3)
        SPCR |= 0x0C;
}

void spiSendSlave(unsigned char sdata)
{
    // send data to data register
    SPDR = sdata;
    // wait for data to transfer
    while (!(SPSR & (1<<SPIF)));
}

uint8_t spiReceiveSlave()
{
    uint8_t xdata = 0x00;
    // check if flag spif is set as an indication that the data is received
    while (!(SPSR & (1<<SPIF)));
    xdata = (uint8_t) SPDR;
    SPSR = 0x00;
    return xdata;
}