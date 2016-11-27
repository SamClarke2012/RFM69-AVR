#include <stdint.h>
#include <avr/io.h>
#include "Spi.h"

void SPI_master_init(uint8_t div) {
    /* Set MOSI & CLK to ouput - native CS as output for AVR */
    DDRB |= ((1 << PB3)|(1 << PB2)|(1 << PB5));  
    /* Set SPI double speed register if needed */
    if (div == FOSC2 || div == FOSC8 || div == FOSC32 || div == FOSC64) {
        SPSR |= (1 << SPI2X);
    } else { SPSR |= (0 << SPI2X); }
    SPCR |= div;
}

uint8_t SPI_transfer(uint8_t byte) {
    /* CS pin handled at higher level */
    SPDR = byte;
    while (!(SPSR & (1 << SPIF)));
    return SPDR;
}