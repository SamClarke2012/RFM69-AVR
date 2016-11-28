
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
                      uint8_t  networkId, uint8_t  nodeId,
                      Mode mode,          void (* error_function)( Bool )  ); 
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
   uint8_t   RFM69HW_getRSSIthresh( void );
   // Settings for timeout IRQ
   void      RFM69HW_setRxTimeout( uint8_t timeout );
   uint8_t   RFM69HW_getRxTimeout( void );
   void      RFM69HW_setRSSItimeout( uint8_t timeout );
   uint8_t   RFM69HW_getRSSItimeout( void );
   // Settings for preamble size
   void      RFM69HW_setPreambleSize( uint16_t size );
   uint16_t  RFM69HW_getPreambleSize( void );
   // Get / Set the fifo fill condition
   void      RFM69HW_setFifoFillCondition( Bool condition );
   Bool      RFM69HW_getFifoFillCondition( void );
   // Set / Get the sync config
   void      RFM69HW_setSync( Bool state );
   Bool      RFM69HW_getSync( void );
   void      RFM69HW_setSyncSize( uint8_t syncWordSize ); // max 7 - 0 based
   uint8_t   RFM69HW_getSyncSize( void ); // max 7 - 0 based
   void      RFM69HW_setSyncTolerance( uint8_t tolerance ); // max 7
   uint8_t   RFM69HW_getSyncTolerance( void ); // max 7
   void      RFM69HW_setSyncWord( char *word );
   char     *RFM69HW_getSyncWord( void );
   // Set / Get packet configurations
   #define   RFM69HW_PKTFMT_FIXED    0
   #define   RFM69HW_PKTFMT_VARIABLE 1
   void      RFM69HW_setPacketFormat( Mode format );
   Mode      RFM69HW_getPacketFormat( void );

   #define   RFM69HW_ENCODING_NONE       0
   #define   RFM69HW_ENCODING_MANCHESTER 1
   #define   RFM69HW_ENCODING_WHITENING  2

   void      RFM69HW_setEncoding( Mode encoding );
   Mode      RFM69HW_getEncoding( void );

   void      RFM69HW_setCRCchecking( Bool state );
   Bool      RFM69HW_getCRCchecking( void );

   void      RFM69HW_setCRCautoClear( Bool state );
   Bool      RFM69HW_getCRCautoClear( void );

   #define   RFM69HW_ADDRESSFILTER_OFF       0 // No filtering
   #define   RFM69HW_ADDRESSFILTER_NODE      1 // Node address only
   #define   RFM69HW_ADDRESSFILTER_BROADCAST 2 // Node and broadcast adresses

   void      RFM69HW_setAdressFilter( Mode addrFilter );
   Mode      RFM69HW_getAddressFilter( void );

   void      RFM69HW_setPayloadLength( uint8_t length ); // If fixed packet it's len, if variable it's max Rx
   uint8_t   RFM69HW_getPayloadLength( void );

   void      RFM69HW_setNodeAddress( uint8_t address );
   uint8_t   RFM69HW_getNodeAddress( void );

   void      RFM69HW_setBroadcastAddress( uint8_t address );
   uint8_t   RFM69HW_getBroadcastAddress( void );

   // TODO - Automodes & packetconfig2 (inter rx delay times etc)

   // Tx start conditions
   //  MS 4 bits rev num, LS 4 bits metal rev num 
   uint8_t   RFM69HW_getVersion( void );
   uint8_t   RFM69HW_test( void );
#endif