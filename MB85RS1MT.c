#include "MB85RS1MT.h"
#include <stdint.h>
#include <stdint.h>
#include "Spi.h"


void FRAMinit( void ) {
	// Set pin as output
	DDRB |= CHIPSEL_PIN;
	// Pull it up
	CHIPSEL_HGH;
	// 8Mhz SPI
	SPI_master_init( FOSC2 ); 
}

void FRAMsetWriteEnable( boolean enable ) {
	CHIPSEL_LOW;
	uint8_t op = enable ? OPCODE_WREN : OPCODE_WRDI;
	SPI_transfer( op );
	CHIPSEL_HGH;
}

void FRAMwrite( uint32_t addr, uint8_t value ) {

	CHIPSEL_LOW;
	SPI_transfer(OPCODE_WREN);
	CHIPSEL_HGH;
	CHIPSEL_LOW;
	SPI_transfer(OPCODE_WRITE);
	SPI_transfer((uint8_t)((addr >> 16) & 0xFF));
	SPI_transfer((uint8_t)((addr >> 8)  & 0xFF));
	SPI_transfer((uint8_t)(addr & 0xFF));
	SPI_transfer(value);
	/* CS on the rising edge commits the WRITE */
	CHIPSEL_HGH;
	CHIPSEL_LOW;
	SPI_transfer(OPCODE_WRDI);
	CHIPSEL_HGH;
}


void FRAMsetStatus(uint8_t value) {
	CHIPSEL_LOW;
	SPI_transfer(OPCODE_WRSR);
	SPI_transfer(value);
	CHIPSEL_HGH;
}


uint8_t FRAMread( uint32_t addr ) {
	CHIPSEL_LOW;
	SPI_transfer(OPCODE_READ);
	SPI_transfer((uint8_t)((addr >> 16) & 0xFF));
	SPI_transfer((uint8_t)((addr >> 8)  & 0xFF));
	SPI_transfer((uint8_t)(addr & 0xFF));
	uint8_t x = SPI_transfer(0);
	CHIPSEL_HGH;
	return x;
}

uint8_t FRAMgetStatus( void ) {
	uint8_t reg = 0;
	CHIPSEL_LOW;
	SPI_transfer(OPCODE_RDSR);
	reg = SPI_transfer(0);
	CHIPSEL_HGH;
	return reg;
}

uint8_t FRAMgetManId( void ) {
	uint8_t a[4] = { 0, 0, 0, 0 };
	uint8_t results;

	CHIPSEL_LOW;
	SPI_transfer(OPCODE_RDID);
	a[0] = SPI_transfer(0);
	a[1] = SPI_transfer(0);
	a[2] = SPI_transfer(0);
	a[3] = SPI_transfer(0);
	CHIPSEL_HGH;

	return a[0];

}

uint16_t FRAMgetProdId( void ) {
	uint8_t a[4] = { 0, 0, 0, 0 };
	uint8_t results;

	CHIPSEL_LOW;
	SPI_transfer(OPCODE_RDID);
	a[0] = SPI_transfer(0);
	a[1] = SPI_transfer(0);
	a[2] = SPI_transfer(0);
	a[3] = SPI_transfer(0);
	CHIPSEL_HGH;

	return (a[2] << 8) + a[3];

}