#ifndef MB85RS1MT_H
#define MB85RS1MT_H
#include <stdint.h>
#include <avr/io.h>

#define OPCODE_WREN   0b0110      /* Write Enable Latch */
#define OPCODE_WRDI   0b0100      /* Reset Write Enable Latch */
#define OPCODE_RDSR   0b0101      /* Read Status Register */
#define OPCODE_WRSR   0b0001      /* Write Status Register */
#define OPCODE_READ   0b0011      /* Read Memory */
#define OPCODE_WRITE  0b0010      /* Write Memory */
#define OPCODE_RDID   0b10011111  /* Read Device ID */

#define CHIPSEL_PIN  (1 << PB0)
#define CHIPSEL_LOW  PORTB &= ~CHIPSEL_PIN
#define CHIPSEL_HGH  PORTB |=  CHIPSEL_PIN

#define TRUE  1
#define FALSE 0 

typedef uint8_t boolean;

void FRAMinit( void );
void FRAMsetWriteEnable( boolean enable );
void FRAMwrite( uint32_t addr, uint8_t value );
void FRAMsetStatus( uint8_t regValue );

uint8_t FRAMread( uint32_t addr );
uint8_t FRAMgetStatus( void );
uint8_t FRAMgetManId( void );

uint16_t FRAMgetProdId( void );

#endif
