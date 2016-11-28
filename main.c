#include <stdlib.h>
#include "Usart.h"
#include "Delay.h"
#include <stdint.h>
#include "MB85RS1MT.h"
#include "RFM69HW.h"
#include <avr/io.h>

void error_function( Bool state ) {
   // LED to indicate a fatal error
   // when operating the radio.
   // Called with 1 before a 'risky' operation
   // Called again with 0 afterward.
   // Ipso facto, blinking is normal, solid is an error.
   if( state == 1 ) { PORTB |= (1 << PB1); }
   else { PORTB &= ~(1 << PB1); }
}


// Embedded software gernerally doesn't return from main.
void main(  int argc, char const **argv ){
   // error LED as output
   DDRB |= (1 << PB1);
   // Set low
   PORTB &= ~(1 << PB1);
   wait_seconds(3);
   uint8_t byte;
   // Initialise FRAM
   FRAMinit();
   // Basic comms for debug.
   USARTinit(9600);
   // Init with error LED
   RFM69HW_init(433, 300.0, 1, 1, 
               RFM69HW_MODE_STANDBY,//RFM69HW_MODE_LISTEN ,
               &error_function);
   // Get the FRAM details a proof of life
   USARTsendString("FRAM Man ID: ");
   USARTwriteHex8(FRAMgetManId());
   USARTsendString("\n");

   USARTsendString("FRAM Prod ID: ");
   USARTwriteHex16(FRAMgetProdId());
   USARTsendString("\n");

   // Get/set some radio registers as proof of life

   RFM69HW_setMode( RFM69HW_MODE_RX );
   uint8_t rssi;
   while( 1 ) {
      rssi = RFM69HW_getRSSI();
      USARTsendString("Measured RSSI = 0x");
      USARTwriteHex8( rssi );
      USARTsendString(" dBm\n");
   }

   // uint8_t mode = RFM69HW_getMode();
   // USARTsendString("Mode = 0x");
   // USARTwriteHex8( mode );
   // USARTsendString("\n");

   // float fq = RFM69HW_getCarrierFrequency();
   // uint16_t fqc = fq;
   // USARTsendString("Carrier Frequency = 0x");
   // USARTwriteHex16( fqc );
   // USARTsendString(" MHz\n");

   // float br = RFM69HW_getBitRate();
   // uint16_t brc = br;
   // USARTsendString("Bit Rate = 0x");
   // USARTwriteHex16( brc );
   // USARTsendString(" Kbps\n");

   // RFM69HW_setCipherKey("thisisacipherkey");
   // char *key = RFM69HW_getCipherKey();
   // USARTsendString(key);

}