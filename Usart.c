#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h> 
#include <stdio.h>
// #include <string.h>
#include <avr/pgmspace.h>

#define F_CPU 16000000UL
/*
_______________________________________16_Mhz__________________________________
__________________U2X=0___________________|______________U2X=1_________________|
Baud___|__UBRR(dec)_|__UBRR(hex)_|__%err__|__UBRR(dec)__|__UBRR(hex)__|__% err_|
300____|____3332____|___0x0D04___|__0.0___|____6666_____|____0x1A0A___|___0.0__|
600____|____1666____|___0x0682___|__0.0___|____3332_____|____0x0D04___|___0.0__|
1200___|____832_____|___0x0340___|__0.0___|____1666_____|____0x0682___|___0.0__|
2400___|____416_____|___0x01A0___|__0.0___|____832______|____0x0340___|___0.0__|
4800___|____207_____|___0x00CF___|__0.2___|____416______|____0x01A0___|___0.0__|
9600___|____103_____|___0x0067___|__0.2___|____207______|____0x00CF___|___0.2__|
14400__|____68______|___0x0044___|__0.6___|____138______|____0x008A___|___0.0__|
19200__|____51______|___0x0033___|__0.2___|____103______|____0x0067___|___0.2__|
28800__|____34______|___0x0022___|__0.8___|____68_______|____0x0044___|___0.6__|
38400__|____25______|___0x0019___|__0.2___|____51_______|____0x0033___|___0.2__|
57600_#|____16______|___0x0010___|__2.1___|____34_______|____0x0022___|___0.8__|
76800__|____12______|___0x000C___|__0.2___|____25_______|____0x0019___|___0.2__|
115200*|____8_______|___0x0008___|__3.7___|____16_______|____0x0010___|___2.1__|
230400*|____3_______|___0x0003___|__7.8___|____8________|____0x0008___|___3.7__|
250000_|____3_______|___0x0003___|__0.0___|____7________|____0x0007___|___0.0__|
  * = High error
  # = High error in normal mode, accep_hex_table in double speed mode
  _________________________________________________________________
 |_UMSELn1_|_UMSELn0|__UPMn1_|_UPMn0_|_USBSn|_UCSZn1|_UCSZn0|UCPOLn|
 |__Mode1__|__Mode2_|_Parity_|_Parity|StopBt|Charsz_|_Charsz|Polarity
 |____7____|___6____|____5___|___4___|___3__|___2___|___1___|___0__|
*/

void USARTinit(uint32_t baud) {
    /* Set baud rate */
    uint16_t prescale = F_CPU/16/baud-1;
    UBRR0H = (uint8_t)(prescale >> 8);
    UBRR0L = (uint8_t)prescale;
    //UCSR0A &= ~(1 << U2X0);
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    /* 1 Stop bit, 8 Bit size, default no parity */
    UCSR0C = (0 << USBS0)|(1 << UCSZ01)|(1 << UCSZ00);
    /* Enable internal pull-up on pin Rx, to supress line noise */
    DDRD &= ~(1 << 0);
    PORTD |= (1 << 0);
}

void USARTflush(void) {
    /* Read out USART buffer */
    volatile uint8_t x;
    while (UCSR0A & (1 << RXC0)) {
        x = UDR0;
        __asm__ __volatile__ ("nop" ::"m"(x));
    }
}

void USARTwrite( uint8_t byte ) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = byte;
}

uint8_t USARTread( void ) {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

void USARTsendString( char *data ) {
    /* Send a null terminated string */
    while( *data != '\0' ) {
        USARTwrite((uint8_t) *data);
        data++;
    }
    //USARTwrite((uint8_t) *data);
}

// char *USARTreceiveString(uint8_t *data) {
//     /* Recieve a null terminated string */
//     int i = 0;
//     char *data;
//     char c = USARTread();
//     while (c != '\0' && i < USART_TFER_MAX ) {
//         data[i] = USART_read();
//         i++;
//     }
//     data[i] = USART_read();
//     return data;
// }

void USARTwriteHex8(uint8_t byte) {
    const char *_hex_table = "0123456789ABCDEF";
    USARTwrite(_hex_table[(uint8_t)(byte >> 4)]);
    USARTwrite(_hex_table[(byte & 0xF)]);  
}

void USARTwriteHex16(uint16_t data) {
    const char *_hex_table = "0123456789ABCDEF";
    uint8_t msb = (data >> 8) & 0xFF;
    uint8_t lsb = data & 0xFF;
    USARTwriteHex8(msb);
    USARTwriteHex8(lsb);
}

// static inline void USART_write_float( float f ) {
//     char* output;
//     USART_send(dtostrf( f, 10, 4, output));
// }

// static inline void USART_write_long( uint32_t l ) {
//     char* output;
//     USART_send(ltoa( l, output, 10));
// }

// void USART_send_P( const char *str, uint8_t n ) {
//     for(int i = 0; i < n; i++){
//         uint8_t b = pgm_read_byte( str++ );
//         USART_write( b );
//     }
//     // char *buff;
//     // buff = (char *)malloc( strlen_P(str) + 1 );
//     // strcpy_P( buff, str );
//     // USART_send( buff );
//     // free( buff );
// }
