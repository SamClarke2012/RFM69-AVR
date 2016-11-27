#include "RFM69HW.h"
#include "RFM69HW_DEFS.h"
#include "Usart.h"
#include "Delay.h"
#include "Spi.h"
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <avr/io.h>

// WSIWYG
static uint8_t RFM69HW_readReg(  uint8_t reg );
static void    RFM69HW_writeReg( uint8_t reg, uint8_t value );
// Calculate carrier freq bytes
static uint32_t freqConversion( uint16_t freqMhz );
// Calculate bit rate bytes
static uint16_t bitRateConversion( float bitRateKbps );
// Calculate rssiThreshold bytes
static uint8_t rssiThreshConversion( uint8_t threshDbm );
// A handy array of register addresses for key updates
static uint8_t CipherKeyRegs[16];

/*
   Error handling function.
   Called before RF based while loops
   Cancelled on success.
*/
static void (* _err)( Bool );

static void _raiseFatalError( void ) {
   _err( ON );
   while( ON );
}

/*

*/
void RFM69HW_init( uint16_t cFreqMhz,  uint32_t bitRateKbps,
                   uint8_t  networkId, uint8_t  nodeId,
                   void (* error_function)( Bool )  ) {

   // Set CS pin to output
   DDRB |= RF_CHIPSEL_PIN;
   // Set error function
   _err = error_function;
   // Set frequency
   RFM69HW_setCarrierFrequency( cFreqMhz );
   // Set bit rate
   RFM69HW_setBitRate( bitRateKbps );
   // Set network ID
   // set node ID
   // Set mode to stand-by.
   RFM69HW_setMode( RFM69HW_MODE_STANDBY );
   // Setup ISR / modulator settings
   return;
}


/*
   Bits 7-4 give full rev number
   Bits 3-0 give metal mask rev num 
*/
uint8_t RFM69HW_getVersion( void ) {
   return RFM69HW_readReg( REGVERSION );
}
/*
   Set the AES key registers.
   Must be exactly 16 bytes.
*/
void RFM69HW_setCipherKey( char *key ) {
   uint8_t i, regCopy;
   // Must be 16bytes in length
   if( strnlen( key, 17) != 16 ) {
      // No false security for you!
      _raiseFatalError();
   }
   // Save current mode register state
   regCopy = RFM69HW_readReg( REGOPMODE );
   // Key change needs stand-by
   RFM69HW_setMode( RFM69HW_MODE_STANDBY );
   // Update key registers
   for( i=0; i<16; i++) RFM69HW_writeReg( CipherKeyRegs[i], key[i] );
   // Set opmode register back to original state
   RFM69HW_writeReg( REGOPMODE, regCopy );
   return;
}

char *RFM69HW_getCipherKey( void ) {
   int i;
   char *key = calloc(17, sizeof( char ));
   for( i=0; i<16; i++) key[i] = RFM69HW_readReg( CipherKeyRegs[i] );
   return key;
} 

void RFM69HW_setCarrierFrequency( uint16_t freqMhz ) {
   // Convert and shift out
   uint32_t conv = freqConversion( freqMhz );
   RFM69HW_writeReg( REGFRFMSB, ((conv & 0xFF000000) >> 24) );
   RFM69HW_writeReg( REGFRFMID, ((conv & 0x00FF0000) >> 16) );
   RFM69HW_writeReg( REGFRFLSB, ((conv & 0x0000FF00) >> 8) );
   return;
}

float RFM69HW_getCarrierFrequency( void ) {
   // Shift in and convert
   uint32_t freqBytes = 0;
   freqBytes |= (uint32_t)(RFM69HW_readReg( REGFRFMSB )) << 16;
   freqBytes |= (uint32_t)(RFM69HW_readReg( REGFRFMID )) << 8;
   freqBytes |= (uint32_t)(RFM69HW_readReg( REGFRFLSB )) << 0;  
   return (freqBytes * FSTEP) / MHZ_HZ;
}

void RFM69HW_setFrequencyDeviation( float deviationKhz ) {
   //TODO
   return;
}

float RFM69HW_getFrequencyDeviation( void ) {
   //TODO
   return 0.0f;
}

void RFM69HW_setBitRate( uint16_t bitRate ) {
   uint16_t registerValue = bitRateConversion( bitRate );
   RFM69HW_writeReg( REGBITRATEMSB, ((registerValue & 0xFF00) >> 8));
   RFM69HW_writeReg( REGBITRATELSB, ((registerValue & 0x00FF) >> 0));
   return;
}

float RFM69HW_getBitRate( void ) {
   uint16_t brRegister = 0;
   brRegister |= (RFM69HW_readReg( REGBITRATEMSB ) << 8);
   brRegister |= (RFM69HW_readReg( REGBITRATELSB ) << 0);
   return (FXOSC / KBPS_BPS) / brRegister;
}

void RFM69HW_setAFClowBeta( Bool state ) {
   // TODO
   return;
} // Has macro

Bool RFM69HW_getAFClowBeta( void ) {
   // TODO
   return OFF;
}

void RFM69HW_setListenCoefIdle( uint8_t idleCoef ) {
   // TODO
   return;
}

uint8_t RFM69HW_getListenCoefIdle( void ) {
   // TODO
   return 0;
}

// Get/Set Listen Coeficients - idleTime = ListenCoefIdle*ListenResolIdle
void RFM69HW_setListenCoefRx( uint8_t rxCoef ) {
   // TODO
   return;
}

uint8_t RFM69HW_getListenCoefRx( void ) {
   // TODO
};

// Get/Set OOK fixed threshold value in dBm, max 15. - only for fixed mode 
void RFM69HW_setOOKfixedThresh( uint8_t thresh ) {
   // TODO
   return;
}

uint8_t RFM69HW_getOOKfixedThresh( void ) {
   // TODO
   return OFF;
}



Bool RFM69HW_getIRQ( IRQ flag ) {
   uint8_t IRQregister;
   // todo check 0 <= flag <= 13
   // Read appropriate IRQ register
   if( flag > 7 ) {
      IRQregister = RFM69HW_readReg( REGIRQFLAGS2 );
   } else {
      IRQregister = RFM69HW_readReg( REGIRQFLAGS1 );
   }
   // Return the appropriate bit
   switch( flag ) {
      case RFM69HW_IRQFLAG_MODEREADY:
         return IRQMASK_MODEREADY( IRQregister );
      case RFM69HW_IRQFLAG_RXREADY:
         return IRQMASK_RXREADY( IRQregister );
      case RFM69HW_IRQFLAG_TXREADY:
         return IRQMASK_TXREADY( IRQregister );
      case RFM69HW_IRQFLAG_PLLLOCK:
         return IRQMASK_PLLLOCK( IRQregister );
      case RFM69HW_IRQFLAG_RSSI:
         return IRQMASK_RSSI( IRQregister );
      case RFM69HW_IRQFLAG_TIMEOUT:
         return IRQMASK_TIMEOUT( IRQregister );
      case RFM69HW_IRQFLAG_AUTOMODE:
         return IRQMASK_AUTOMODE( IRQregister );
      case RFM69HW_IRQFLAG_SYNCADDR:
         return IRQMASK_SYNCADDR( IRQregister );
      case RFM69HW_IRQFLAG_FIFOFULL:
         return IRQMASK_FIFOFULL( IRQregister );
      case RFM69HW_IRQFLAG_FIFONOTEMPTY:
         return IRQMASK_FIFONOTEMPTY( IRQregister );
      case RFM69HW_IRQFLAG_FIFOOVERRUN:
         return IRQMASK_FIFOOVERRUN( IRQregister );
      case RFM69HW_IRQFLAG_PACKETSENT:
         return IRQMASK_PACKETSENT( IRQregister );
      case RFM69HW_IRQFLAG_PAYLOADREADY:
         return IRQMASK_PAYLOADREADY( IRQregister );
      case RFM69HW_IRQFLAG_CRCOK:
         return IRQMASK_CRCOK( IRQregister );
      default:
         return FALSE;
   }
}

void RFM69HW_setMode( Mode mode ) {
   // Read current op-mode register
   uint8_t opModeRegister = RFM69HW_readReg( REGOPMODE );
   // Set appropriate bits
   switch( mode ) {
      case RFM69HW_MODE_SLEEP:
         OPMODE_MODE_SLEEP( opModeRegister ); 
         break;
      case RFM69HW_MODE_STANDBY:
         OPMODE_MODE_STANDBY( opModeRegister ); 
         break;
      case RFM69HW_MODE_FQSYNTH:
         OPMODE_MODE_FQSYNTH( opModeRegister ); 
         break;
      case RFM69HW_MODE_TX:
         OPMODE_MODE_TX( opModeRegister ); 
         break;
      case RFM69HW_MODE_RX:
         OPMODE_MODE_SLEEP( opModeRegister ); 
         break;
      case RFM69HW_MODE_LISTEN:
         // TODO - set stand by, set options, set listen bit
         break;
      default:
         break;
   }
   // Write back to op-mode register
   RFM69HW_writeReg( REGOPMODE, opModeRegister );
   // wait for IRQ flag mode-ready if needed
   _err( ON ); // Toggle error LED on
   while( ! RFM69HW_getIRQ( RFM69HW_IRQFLAG_MODEREADY) );
   _err( OFF ); // We survived, turn it off.
   return;
}

/*
   Returns the current mode of the chipset.
   e.g  RFM69HW_MODE_RX
*/
Mode RFM69HW_getMode( void ) {
   uint8_t opModeRegister = RFM69HW_readReg( REGOPMODE );
   // Check for listen mode first
   if( OPMODE_GETLISTEN( opModeRegister) ) {
      return RFM69HW_MODE_LISTEN;
   } else {
      // Not in listen mode, return mode index bits
      return OPMODE_GETMODE( opModeRegister );
   }
}

/* 
   Measure and return RSSI
      - RSSI val can only be read when > than RSSI thresh
      - Given in 0.5dBm steps - integer rounding to keep it light weight

*/
uint8_t RFM69HW_getRSSI( void ) {
   // Check not in sleep mode or we'll loop f'ever
   // Get the current RSSI register
   uint8_t RSSIconfig = 0;// RFM69HW_readReg( REGRSSICONFIG );
   // Toggle the start bit
   RSSI_START( RSSIconfig );
   // Write it back to the register
   RFM69HW_writeReg( REGRSSICONFIG, RSSIconfig );
   // Wait for sampling to complete
   _err( ON ); // Toggle error LED to on.
   while( ! RSSI_DONE( RFM69HW_readReg( REGRSSICONFIG ) ) );
   _err( OFF ); // We survived, turn it off
   // Return true RSSI value
   return RFM69HW_readReg( REGRSSIVALUE )*2;
}

void RFM69HW_setRSSIthresh( uint8_t threshDbm ) {
   // TODO
   return;
}

// Settings for timeout IRQ
void RFM69HW_setRxTimeout( uint8_t timeout ) {
   // TODO
   return;
}

void RFM69HW_setRSSItimeout( uint8_t timeout ) {
   // TODO
   return;
}

uint8_t RFM69HW_test( void ) {
   return RFM69HW_readReg( REGVERSION );
}






static uint8_t CipherKeyRegs[16] = { REGAESKEY1, REGAESKEY2, REGAESKEY3, REGAESKEY4,
                                     REGAESKEY5, REGAESKEY6, REGAESKEY7, REGAESKEY8,
                                     REGAESKEY9, REGAESKEY10,REGAESKEY11,REGAESKEY12,
                                     REGAESKEY13,REGAESKEY14,REGAESKEY15,REGAESKEY16 };
// Default is 114dBm - register holds 0xE4 = 228 dBm
static uint8_t rssiThreshConversion( uint8_t threshDbm ) {
   return 2*threshDbm;
}
// Calculate carrier freq bytes
static uint32_t freqConversion( uint16_t freqMhz ) {
   uint32_t fqhz  = freqMhz*MHZ_HZ;
   // Integer rounding, shift up to MSB of uint32_t
   return ((uint32_t)( round((float)(fqhz/FSTEP)) )  << 8 );
}
// Calculate bit rate bytes
static uint16_t bitRateConversion( float bitRateKbps ) {
   return round((float)( FXOSC/(bitRateKbps*KBPS_BPS) ) );
}


static void RFM69HW_writeReg( uint8_t reg, uint8_t value ) {
   RF_CHIPSEL_LOW;
   SPI_transfer( reg | 0x80 );
   SPI_transfer( value );
   RF_CHIPSEL_HGH;
   return;
}

static uint8_t RFM69HW_readReg(  uint8_t reg ) {
   uint8_t value;
   RF_CHIPSEL_LOW;
   SPI_transfer( reg );
   value = SPI_transfer(0);
   RF_CHIPSEL_HGH;
   return value;
}