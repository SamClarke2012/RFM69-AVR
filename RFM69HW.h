
#ifndef RFM69HW_H
   #define RFM69HW_H

   #include <stdint.h>

   #ifndef TRUE
      #define TRUE  1
   #endif

   #ifndef FALSE
      #define FALSE 0 
   #endif
   
   #ifndef ON
      #define ON  1
   #endif

   #ifndef OFF
      #define OFF 0 
   #endif
   // ON/OFF - TRUE/FALSE
   typedef uint8_t Bool;
   // IRQ flags
   typedef uint8_t IRQ;
   // Standard modes
   typedef uint8_t Mode;

   #define RFM69HW_IRQFLAG_MODEREADY    0
   #define RFM69HW_IRQFLAG_RXREADY      1
   #define RFM69HW_IRQFLAG_TXREADY      2
   #define RFM69HW_IRQFLAG_PLLLOCK      3
   #define RFM69HW_IRQFLAG_RSSI         4
   #define RFM69HW_IRQFLAG_TIMEOUT      5
   #define RFM69HW_IRQFLAG_AUTOMODE     6
   #define RFM69HW_IRQFLAG_SYNCADDR     7
   #define RFM69HW_IRQFLAG_FIFOFULL     8
   #define RFM69HW_IRQFLAG_FIFONOTEMPTY 9
   #define RFM69HW_IRQFLAG_FIFOOVERRUN  10
   #define RFM69HW_IRQFLAG_PACKETSENT   11
   #define RFM69HW_IRQFLAG_PAYLOADREADY 12
   #define RFM69HW_IRQFLAG_CRCOK        13

   #define RFM69HW_MODE_SLEEP     0
   #define RFM69HW_MODE_STANDBY   1
   #define RFM69HW_MODE_FQSYNTH   2
   #define RFM69HW_MODE_TX        3
   #define RFM69HW_MODE_RX        4
   #define RFM69HW_MODE_LISTEN    5

   // Spi specific definitions
   #define RF_CHIPSEL_PIN  (1 << PB2)
   #define RF_CHIPSEL_LOW  PORTB &= ~RF_CHIPSEL_PIN
   #define RF_CHIPSEL_HGH  PORTB |=  RF_CHIPSEL_PIN

   #define KBPS_BPS (1e3)
   #define MHZ_HZ   (1e6)
   #define FXOSC    (32e6)
   #define FSTEP    (61.03515625)

   void RFM69HW_init( uint16_t cFreqMhz,  uint32_t bitRateKbps,
                   uint8_t  networkId,    uint8_t  nodeId,
                   void (* error_function)( Bool )  ); 
   // Get/Set Carrier Frequency in MHz - e.g. 433Mhz
   void      RFM69HW_setCarrierFrequency( uint16_t freqMhz );
   float     RFM69HW_getCarrierFrequency( void );
   // Get/Set Frequency Deviation in KHz
   void      RFM69HW_setFrequencyDeviation( float deviationKhz );
   float     RFM69HW_getFrequencyDeviation( void );
   // Get/Set Bit rate in Kbps - e.g. 300 or 156.3 etc
   void      RFM69HW_setBitRate( uint16_t bitRate );
   float     RFM69HW_getBitRate( void );   
   // Get/Set AES Cipher Key registers
   void      RFM69HW_setCipherKey( char *key ); 
   char      *RFM69HW_getCipherKey( void );
   // Improved AFC routine for signals with modulation index
   // ower than 2. Refer to section 3.4.16 for details
   void      RFM69HW_setAFClowBeta( Bool state ); // Has macro
   Bool      RFM69HW_getAFClowBeta( void );
   // Get/Set Listen Coeficients - idleTime = ListenCoefIdle*ListenResolIdle
   void      RFM69HW_setListenCoefIdle( uint8_t idleCoef );
   uint8_t   RFM69HW_getListenCoefIdle( void );
   // Get/Set Listen Coeficients - idleTime = ListenCoefIdle*ListenResolIdle
   void      RFM69HW_setListenCoefRx( uint8_t rxCoef );
   uint8_t   RFM69HW_getListenCoefRx( void );
   // Get/Set OOK fixed threshold value in dBm, max 15. - only for fixed mode 
   void      RFM69HW_setOOKfixedThresh( uint8_t thresh );
   uint8_t   RFM69HW_getOOKfixedThresh( void );
   // Get/Set DIO mappings - maybe later for cont mode HW interrupts
   Bool      RFM69HW_getIRQ( IRQ flag );
   void      RFM69HW_setMode( Mode mode );
   Mode      RFM69HW_getMode( void );
   // Measure and return RSSI
   uint8_t   RFM69HW_getRSSI( void );
   void      RFM69HW_setRSSIthresh( uint8_t threshDbm );
   // Settings for timeout IRQ
   void      RFM69HW_setRxTimeout( uint8_t timeout );
   void      RFM69HW_setRSSItimeout( uint8_t timeout );
   //  MS 4 bits rev num, LS 4 bits metal rev num 
   uint8_t   RFM69HW_getVersion( void );
   uint8_t   RFM69HW_test( void );
#endif