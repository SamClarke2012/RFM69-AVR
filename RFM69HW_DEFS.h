/*  HOPERF Example configuration  
    https://lowpowerlab.com/forum/rf-range-antennas-rfm69-library/rfm69-rx-configuration/

Primary Config:

   DataModul
   FDev
   BitRate
   OCP
   LNA
   RxBW
   PreAmble
   SyncWord
   Packet Config - (CRC etc)
   PayloadLength
   FiFo Thresh
   TestLNA - increase sensitivity
   TestDAGC - ""
   OpMode - Standby

Rx mode:

   PaLevel : Pa0=0n, Pa1 and Pa2=off, Pout=11111
   DIO Mapping for Rx : DIO0=Payload Ready, DIO1=Fifo level
   TestPa1=Normal mode and Rx mode
   TestPa2=Normal mode and Rx mode
   OpMode - Rx

Tx Mode:
   RegPaLevel 13dBm
   Normal and TRx
   OpMode - Tx






   soft interrupts using timer interrupt polling irqflags
   hard interrupt dio2/clk for continuous sniffing via data pin into FRAM
*/



#include <avr/io.h>
#include <avr/sfr_defs.h>
/*
   Register Definitions.
   RFM69HW-V1.3.pdf  pp.60-62

   Note     - Reset values are automatically refreshed in the chip at Power On Reset.
*/
                              // Default - Recommended
   // FStep is 32e6 / 2^19 -> congruent to FOSC/2^19
   // Carrier Frequency register is freq/fstep  e.g 433e6 / step
   // Bit rate register is fxosc/bitrate e.g 32e6/300000 = 0x006b
#ifndef RFM69HW_DEFS_H
   #define RFM69HW_DES_h
   // Bit rate is 2 registers (uint16_t)
   #define BRATE( r ) { (FXOSC/r) }
   // Bit twiddling abstraction
   #define SBI( byte, bit ) ( byte |=  (1<<(bit)) ) 
   #define CBI( byte, bit ) ( byte &= ~(1<<(bit)) )


   // OP Mode masks. Table 24 pp.63
   // REGISTER: REGOPMODE
   // Creat the byte to fill the register
   // Run the appropriate macros over it
   // then send it over SPI.
   #define OPMODE_SEQ_OFF( operation_settings )      SBI( operation_settings, 7)//Default on
   #define OPMODE_SEQ_ON( operation_settings )       CBI( operation_settings, 7)//Default on
   #define OPMODE_LISTEN_ON( operation_settings )    SBI( operation_settings, 6)//Default off
   #define OPMODE_LISTEN_OFF( operation_settings )   CBI( operation_settings, 6)//Default off
   #define OPMODE_LISTEN_ABORT( operation_settings ) SBI( operation_settings, 5)//Used to turn off once on

   #define OPMODE_MODE_SLEEP( operation_settings ) { \
      CBI( operation_settings, 4);\
      CBI( operation_settings, 3);\
      CBI( operation_settings, 2);\
   }
   // 0b001
   #define OPMODE_MODE_STANDBY( operation_settings ) { \
      CBI( operation_settings, 4);\
      CBI( operation_settings, 3);\
      SBI( operation_settings, 2);\
   }
   // 0b010
   #define OPMODE_MODE_FQSYNTH( operation_settings ) { \
      CBI( operation_settings, 4);\
      SBI( operation_settings, 3);\
      CBI( operation_settings, 2);\
   }
   // 0b011
   #define OPMODE_MODE_TX( operation_settings ) { \
      CBI( operation_settings, 4);\
      SBI( operation_settings, 3);\
      SBI( operation_settings, 2);\
   }

   // 0b100
   #define OPMODE_MODE_RX( operation_settings ) { \
      SBI( operation_settings, 4);\
      CBI( operation_settings, 3);\
      CBI( operation_settings, 2);\
   }


   // 2^3 possible modes = Uknown reserve modes >:)
   // 0b101
   #define OPMODE_UNKWN1( operation_settings ) { \
      SBI( operation_settings, 4);\
      CBI( operation_settings, 3);\
      SBI( operation_settings, 2);\
   }
   // 0b110
   #define OPMODE_UNKWN2( operation_settings ) { \
      SBI( operation_settings, 4);\
      CBI( operation_settings, 3);\
      SBI( operation_settings, 2);\
   }


   #define OPMODE_GETLISTEN( regValue ) (( regValue & 0x20 ) >> 6)
   #define OPMODE_GETMODE( regValue )   (( regValue & 0x1C ) >> 2)


   // Modulation settings - REGISTER: REGDATAMODUL
   #define DATAMODUL_DATAMODE_PKT( modulator_settings ) { \
      CBI( modulator_settings, 6);\
      CBI( modulator_settings, 5);\
   }

   #define DATAMODUL_DATAMODE_RSRVD( modulator_settings ) { \
      CBI( modulator_settings, 6);\
      SBI( modulator_settings, 5);\
   }

   #define DATAMODUL_DATAMODE_CONTSYNC( modulator_settings ) { \
      SBI( modulator_settings, 6);\
      CBI( modulator_settings, 5);\
   }

   #define DATAMODUL_DATAMODE_CONT( modulator_settings ) { \
      SBI( modulator_settings, 6);\
      SBI( modulator_settings, 5);\
   }

   #define DATAMODUL_MODTYPE_FSK( modulator_settings ) { \
      CBI( modulator_settings, 4);\
      CBI( modulator_settings, 3);\
   }

   #define DATAMODUL_MODTYPE_OOK( modulator_settings ) { \
      CBI( modulator_settings, 4);\
      SBI( modulator_settings, 3);\
   }

   #define DATAMODUL_MODTYPE_RSRVD1( modulator_settings ) { \
      SBI( modulator_settings, 4);\
      CBI( modulator_settings, 3);\
   }

   #define DATAMODUL_MODTYPE_RSRVD2( modulator_settings ) { \
      SBI( modulator_settings, 4);\
      SBI( modulator_settings, 3);\
   }

   #define DATAMODUL_MODSHAPE_NONE( modulator_settings ) { \
      CBI( modulator_settings, 1);\
      CBI( modulator_settings, 0);\
   }

   #define DATAMODUL_MODSHAPE_FSK_GAUSS10( modulator_settings ) { \
      CBI( modulator_settings, 1);\
      SBI( modulator_settings, 0);\
   }

   #define DATAMODUL_MODSHAPE_FSK_GAUSS05( modulator_settings ) { \
      SBI( modulator_settings, 1);\
      CBI( modulator_settings, 0);\
   }

   #define DATAMODUL_MODSHAPE_FSK_GAUSS03( modulator_settings ) { \
      SBI( modulator_settings, 1);\
      SBI( modulator_settings, 0);\
   }

   #define DATAMODUL_MODSHAPE_OOK_BR( modulator_settings ) { \
      CBI( modulator_settings, 1);\
      SBI( modulator_settings, 0);\
   }

   #define DATAMODUL_MODSHAPE_OOK_2BR( modulator_settings ) { \
      SBI( modulator_settings, 1);\
      CBI( modulator_settings, 0);\
   }

   #define OSC1_RCCAL( osc1 ) {\
      CBI( osc1, 7 );\
      loop_until_bit_is_set( osc1, 6);\
   }

   #define AFCCTRL_LOWBETA_ON( afcctrl ) {\
      SBI( afcctrl, 5 );\
   }
   #define AFCCTRL_LOWBETA_OFF( afcctrl ) {\
      CBI( afcctrl, 5 );\
   }
   // Radio alternates between sleeping and RX
   // resolidle is the sleep time, resolrx is the rx time
   // listen coeff = 1 and min setting = 64us
   // listen coeff = 255 and min setting = 16 millis
   // ""                     max setting = 16ms
   // ""                     max setting = 67 seconds

   // for sniffing use listen mode, rssi only, set rssi thresh low,
   // listen coef 255 for 16ms idle 67secs rx times.
   // Set regrxtimeout to something and use timeout end mode rxidle or mode
   // If we can hit the niquist limit for the signal we can replay it.
   #define LISTEN1_RESOLIDLE_64US( listen_settings ) {\
      CBI( listen_settings, 7 );\
      SBI( listen_settings, 6 );\
   }

   #define LISTEN1_RESOLIDLE_4MS( listen_settings ) {\
      SBI( listen_settings, 7 );\
      CBI( listen_settings, 6 );\
   }

   #define LISTEN1_RESOLIDLE_262MS( listen_settings ) {\
      SBI( listen_settings, 7 );\
      SBI( listen_settings, 6 );\
   }

   #define LISTEN1_RESOLRX_64US( listen_settings ) {\
      CBI( listen_settings, 5 );\
      SBI( listen_settings, 4 );\
   }

   #define LISTEN1_RESOLRX_4MS( listen_settings ) {\
      SBI( listen_settings, 5 );\
      CBI( listen_settings, 4 );\
   }

   #define LISTEN1_RESOLRX_262MS( listen_settings ) {\
      SBI( listen_settings, 5 );\
      SBI( listen_settings, 4 );\
   }

   // Criteria for packet acceptance in Listen mode:
   #define LISTEN1_CRITERIA_RSSIONLY( listen_settings ) {\
      CBI( listen_settings, 3 );\
   }
   #define LISTEN1_CRITERIA_RSSIANDSYNC( listen_settings ) {\
      SBI( listen_settings, 3 );\
   }
   // Action taken after acceptance of a packet in Listen mode

   // 1. chip stays in Rx mode. Listen mode stops and must be disabled (see section 4.3).  
   #define LISTEN1_END_MANUAL( listen_settings ) {\
      CBI( listen_settings, 2 );\
      CBI( listen_settings, 1 );\
   }
   // 2. chip stays in Rx mode until PayloadReady or Timeout interrupt occurs. 
   //    It then goes to the mode defined by Mode. 
   //    Listen mode stops and must be disabled (see section 4.3). 
   #define LISTEN1_END_MODE( listen_settings ) {\
      CBI( listen_settings, 2 );\
      SBI( listen_settings, 1 );\
   }
   // 3. As above but ends in idle state
   #define LISTEN1_END_IDLE( listen_settings ) {\
      SBI( listen_settings, 2 );\
      CBI( listen_settings, 1 );\
   }

   #define PALEVEL_PA0_ON( pa_settings ) {\
      SBI( pa_settings, 7 );\
   }

   #define PALEVEL_PA0_OFF( pa_settings ) {\
      CBI( pa_settings, 7 );\
   }

   #define PALEVEL_PA1_ON( pa_settings ) {\
      SBI( pa_settings, 6 );\
   }

   #define PALEVEL_PA1_OFF( pa_settings ) {\
      CBI( pa_settings, 6 );\
   }

   #define PALEVEL_PA2_ON( pa_settings ) {\
      SBI( pa_settings, 5 );\
   }

   #define PALEVEL_PA2_OFF( pa_settings ) {\
      CBI( pa_settings, 5 );\
   }

   #define OVERCURRENT_OCP_ON( ocp_settings ) {\
      SBI( ocp_setting, 4 );\
   }

   #define OVERCURRENT_OCP_OFF( ocp_settings ) {\
      CBI( ocp_setting, 4 );\
   }

   #define LNA_IMPEDANCE_50R( lna_settings) {\
      CBI( lna_settings, 7 );\
   }

   #define LNA_IMPEDANCE_200R( lna_settings) {\
      SBI( lna_settings, 7 );\
   }

   #define LNA_CURRENTGAIN_50R( lna_settings) {\
      CBI( lna_settings, 7 );\
   }

   #define OOK_THRESHTYPE_FIXED( ook_peak_settings ) {\
      CBI(ook_peak_settings, 7);\
      CBI(ook_peak_settings, 6);\
   }

   #define OOK_THRESHTYPE_PEAK( ook_peak_settings ) {\
      CBI(ook_peak_settings, 7);\
      SBI(ook_peak_settings, 6);\
   }

   #define OOK_THRESHTYPE_AVG( ook_peak_settings ) {\
      SBI(ook_peak_settings, 7);\
      CBI(ook_peak_settings, 6);\
   }

   // Size of each decrement of the RSSI threshold in the OOK demodulator:
   #define OOK_PEAKTHRESHSTEP_05( ook_peak_settings ) {\
      CBI(ook_peak_settings, 5);\
      CBI(ook_peak_settings, 4);\
      CBI(ook_peak_settings, 3);\
   }

   #define OOK_PEAKTHRESHSTEP_10( ook_peak_settings ) {\
      CBI(ook_peak_settings, 5);\
      CBI(ook_peak_settings, 4);\
      SBI(ook_peak_settings, 3);\
   }

   #define OOK_PEAKTHRESHSTEP_15( ook_peak_settings ) {\
      CBI(ook_peak_settings, 5);\
      SBI(ook_peak_settings, 4);\
      CBI(ook_peak_settings, 3);\
   }

   #define OOK_PEAKTHRESHSTEP_20( ook_peak_settings ) {\
      CBI(ook_peak_settings, 5);\
      SBI(ook_peak_settings, 4);\
      SBI(ook_peak_settings, 3);\
   }

   #define OOK_PEAKTHRESHSTEP_30( ook_peak_settings ) {\
      SBI(ook_peak_settings, 5);\
      CBI(ook_peak_settings, 4);\
      CBI(ook_peak_settings, 3);\
   }

   #define OOK_PEAKTHRESHSTEP_40( ook_peak_settings ) {\
      SBI(ook_peak_settings, 5);\
      CBI(ook_peak_settings, 4);\
      SBI(ook_peak_settings, 3);\
   }

   #define OOK_PEAKTHRESHSTEP_50( ook_peak_settings ) {\
      SBI(ook_peak_settings, 5);\
      SBI(ook_peak_settings, 4);\
      CBI(ook_peak_settings, 3);\
   }

   #define OOK_PEAKTHRESHSTEP_60( ook_peak_settings ) {\
      SBI(ook_peak_settings, 5);\
      SBI(ook_peak_settings, 4);\
      SBI(ook_peak_settings, 3);\
   }

   //Period of decrement of the RSSI threshold in the OOK demodulator
   #define OOK_PEAKTHRESHDEC_1P1( ook_peak_settings ) {\
      CBI(ook_peak_settings, 2);\
      CBI(ook_peak_settings, 1);\
      CBI(ook_peak_settings, 0);\
   }

   #define OOK_PEAKTHRESHDEC_1P2( ook_peak_settings ) {\
      CBI(ook_peak_settings, 2);\
      CBI(ook_peak_settings, 1);\
      SBI(ook_peak_settings, 0);\
   }

   #define OOK_PEAKTHRESHDEC_1P4( ook_peak_settings ) {\
      CBI(ook_peak_settings, 2);\
      SBI(ook_peak_settings, 1);\
      CBI(ook_peak_settings, 0);\
   }

   #define OOK_PEAKTHRESHDEC_1P8( ook_peak_settings ) {\
      CBI(ook_peak_settings, 2);\
      SBI(ook_peak_settings, 1);\
      SBI(ook_peak_settings, 0);\
   }

   #define OOK_PEAKTHRESHDEC_2P1( ook_peak_settings ) {\
      SBI(ook_peak_settings, 2);\
      CBI(ook_peak_settings, 1);\
      CBI(ook_peak_settings, 0);\
   }

   #define OOK_PEAKTHRESHDEC_4P1( ook_peak_settings ) {\
      SBI(ook_peak_settings, 2);\
      CBI(ook_peak_settings, 1);\
      SBI(ook_peak_settings, 0);\
   }

   #define OOK_PEAKTHRESHDEC_8P1( ook_peak_settings ) {\
      SBI(ook_peak_settings, 2);\
      SBI(ook_peak_settings, 1);\
      CBI(ook_peak_settings, 0);\
   }

   #define OOK_PEAKTHRESHDEC_16P1( ook_peak_settings ) {\
      SBI(ook_peak_settings, 2);\
      SBI(ook_peak_settings, 1);\
      SBI(ook_peak_settings, 0);\
   }

   // Filter coefficients in average mode of the OOK demodulator:
   #define OOK_AVGTHRESHFILT_32PI( ook_avg_settings ) {\
      CBI(ook_avg_settings, 7);\
      CBI(ook_avg_settings, 6);\
   }

   #define OOK_AVGTHRESHFILT_8PI( ook_avg_settings ) {\
      CBI(ook_avg_settings, 7);\
      SBI(ook_avg_settings, 6);\
   }

   #define OOK_AVGTHRESHFILT_4PI( ook_avg_settings ) {\
      SBI(ook_avg_settings, 7);\
      CBI(ook_avg_settings, 6);\
   }

   #define OOK_AVGTHRESHFILT_2PI( ook_avg_settings ) {\
      SBI(ook_avg_settings, 7);\
      SBI(ook_avg_settings, 6);\
   }

   // IRQ masks - REGIRQFLAGS1
   //Set when the operation mode requested in Mode, is ready
   #define IRQMASK_MODEREADY( regValue ) (( regValue & 0x80 ) >> 7)
   // Set in Rx mode, after RSSI, AGC and AFC.
   #define IRQMASK_RXREADY( regValue )   (( regValue & 0x40 ) >> 6)
   // Set in Tx mode, after PA ramp-up.
   #define IRQMASK_TXREADY( regValue )   (( regValue & 0x20 ) >> 5)
   // Set (in FS, Rx or Tx) when the PLL is locked.
   #define IRQMASK_PLLLOCK( regValue )  (( regValue & 0x10 ) >> 4)
   // Set in Rx when the RssiValue exceeds RssiThreshold.
   #define IRQMASK_RSSI( regValue )      (( regValue & 0x08 ) >> 3)
   // Set when a timeout occurs (see TimeoutRxStart and TimeoutRssiThresh)
   #define IRQMASK_TIMEOUT( regValue )   (( regValue & 0x04 ) >> 2)
   // Set when entering Intermediate mode.
   #define IRQMASK_AUTOMODE( regValue )  (( regValue & 0x02 ) >> 1)
   // Set when Sync and Address (if enabled) are detected.
   #define IRQMASK_SYNCADDR( regValue )  (( regValue & 0x01 ) >> 0)

   // ""       - REGIRQFLAGS2
   // Set when fifo contains 66 bytes
   #define IRQMASK_FIFOFULL( regValue )      (( regValue & 0x80 ) >> 7)
   // Set when at least 1 byte in fifo
   #define IRQMASK_FIFONOTEMPTY( regValue )  (( regValue & 0x40 ) >> 6)
   // Set when over fifo thresh
   #define IRQMASK_FIFOLEVEL( regValue )     (( regValue & 0x20 ) >> 5)
   // fifo cleared when set
   #define IRQMASK_FIFOOVERRUN( regValue )   (( regValue & 0x10 ) >> 4)
   #define IRQMASK_PACKETSENT( regValue )    (( regValue & 0x08 ) >> 3)
   // last byte rec and or crc ok
   #define IRQMASK_PAYLOADREADY( regValue )  (( regValue & 0x04 ) >> 2)
   #define IRQMASK_CRCOK( regValue )         (( regValue & 0x02 ) >> 1)

   // Start RSSI measurement
   #define RSSI_START( RSSIconfig ) {\
      SBI( RSSIconfig, 0 );\
   }
   // Check if RSSI measurement is complete
   #define RSSI_DONE( RSSIconfig ) ( (RSSIconfig & 0x02) >> 1)

   // Register Definitions
   #define REGFIFO          0x00 // 0x00 
   #define REGOPMODE        0x01 // 0x04
   #define REGDATAMODUL     0x02 // 0x00
   #define REGBITRATEMSB    0x03 // 0x1a
   #define REGBITRATELSB    0x04 // 0x0b
   #define REGFDEVMSB       0x05 // 0x00
   #define REGFDEVLSB       0x06 // 0x52
   #define REGFRFMSB        0x07 // 0xe4
   #define REGFRFMID        0x08 // 0xc0
   #define REGFRFLSB        0x09 // 0x00
   #define REGOSC1          0x0A // 0x41
   #define REGAFCCTRL       0x0B // 0x00
   #define REGLOWBAT        0x0C // 0x02
   #define REGLISTEN1       0x0D // 0x92
   #define REGLISTEN2       0x0E // 0xf5
   #define REGLISTEN3       0x0F // 0x20
   #define REGVERSION       0x10 // 0x24
   #define REGPALEVEL       0x11 // 0x9f
   #define REGPARAMP        0x12 // 0x09
   #define REGOCP           0x13 // 0x1a
   /*RESERVED*/
   #define REGAGCREF        0x14 // 0x40
   /*RESERVED*/
   #define REGAGCTHRESH1    0x15 // 0xb0
   /*RESERVED*/
   #define REGAGCTHRESH2    0x16 // 0x7b
   /*RESERVED*/
   #define REGAGCTHRESH3    0x17 // 0x9b

   #define REGLNA           0x18 // 0x08 - 0x88
   #define REGRXBW          0x19 // 0x86 - 0x55
   #define REGAFCBW         0x1A // 0x8a - 0x8b
   #define REGOOKPEAK       0x1B // 0x40
   #define REGOOKAVG        0x1C // 0x80
   #define REGOOKFIX        0x1D // 0x06
   #define REGAFCFEI        0x1E // 0x10
   #define REGAFCMSB        0x1F // 0x00
   #define REGAFCLSB        0x20 // 0x00
   #define REGFEIMSB        0x21 // 0x00
   #define REGFEILSB        0x22 // 0x00
   #define REGRSSICONFIG    0x23 // 0x02
   #define REGRSSIVALUE     0x24 // 0xff
   #define REGDIOMAPPING1   0x25 // 0x00
   #define REGDIOMAPPING2   0x26 // 0x05 - 0x07
   #define REGIRQFLAGS1     0x27 // 0x80
   #define REGIRQFLAGS2     0x28 // 0x00
   #define REGRSSITHRESH    0x29 // 0xff - 0xe4
   #define REGRXTIMEOUT1    0x2A // 0x00
   #define REGRXTIMEOUT2    0x2B // 0x00
   #define REGPREAMBLEMSB   0x2C // 0x00
   #define REGPREAMBLELSB   0x2D // 0x03
   #define REGSYNCCONFIG    0x2E // 0x98
   #define REGSYNCVALUE1    0x2F // 0x00 - 0x01
   #define REGSYNCVALUE2    0x30 // 0x00 - 0x01
   #define REGSYNCVALUE3    0x31 // "       "
   #define REGSYNCVALUE4    0x32 // "       "
   #define REGSYNCVALUE5    0x33 // "       "
   #define REGSYNCVALUE6    0x34 // "       "
   #define REGSYNCVALUE7    0x35 // "       "
   #define REGSYNCVALUE8    0x36 // "       "
   #define REGPACKETCONFIG1 0x37 // 0x10
   #define REGPAYLOADLENGTH 0x38 // 0x40
   #define REGNODEADRS      0x39 // 0x00
   #define REGBROADCASTADRS 0x3A // 0x00
   #define REGAUTOMODES     0x3B // 0x00
   #define REGFIFOTHRESH    0x3C // 0x0f - 0x8f
   #define REGPACKETCONFIG2 0x3D // 0x02

   /* AES Key is all zeros by default >:) */
   #define REGAESKEY1       0x3E // 0x00
   #define REGAESKEY2       0x3F // 0x00
   #define REGAESKEY3       0x40 // 0x00
   #define REGAESKEY4       0x41 // 0x00
   #define REGAESKEY5       0x42 // 0x00
   #define REGAESKEY6       0x43 // 0x00
   #define REGAESKEY7       0x44 // 0x00
   #define REGAESKEY8       0x45 // 0x00
   #define REGAESKEY9       0x46 // 0x00
   #define REGAESKEY10      0x47 // 0x00
   #define REGAESKEY11      0x48 // 0x00
   #define REGAESKEY12      0x49 // 0x00
   #define REGAESKEY13      0x4A // 0x00
   #define REGAESKEY14      0x4B // 0x00
   #define REGAESKEY15      0x4C // 0x00
   #define REGAESKEY16      0x4D // 0x00

   #define REGTEMP1         0x4E // 0x01
   #define REGTEMP2         0x4F // 0x00
   #define REGTESTLNA       0x58 // 0x1b
   #define REGTESTPA1       0x5A // 0x55
   #define REGTESTPA2       0x5C // 0x70
   #define REGTESTDAGC      0x6F // 0x00 - 0x30
   #define REGTESTAFC       0x71 // 0x00







   // RegFdev - frequency deviation (Hz) - lowpower labs
   #define FDEVMSB_2000             0x00
   #define FDEVLSB_2000             0x21
   #define FDEVMSB_5000             0x00  // Default
   #define FDEVLSB_5000             0x52  // Default
   #define FDEVMSB_7500             0x00
   #define FDEVLSB_7500             0x7B
   #define FDEVMSB_10000            0x00
   #define FDEVLSB_10000            0xA4
   #define FDEVMSB_15000            0x00
   #define FDEVLSB_15000            0xF6
   #define FDEVMSB_20000            0x01
   #define FDEVLSB_20000            0x48
   #define FDEVMSB_25000            0x01
   #define FDEVLSB_25000            0x9A
   #define FDEVMSB_30000            0x01
   #define FDEVLSB_30000            0xEC
   #define FDEVMSB_35000            0x02
   #define FDEVLSB_35000            0x3D
   #define FDEVMSB_40000            0x02
   #define FDEVLSB_40000            0x8F
   #define FDEVMSB_45000            0x02
   #define FDEVLSB_45000            0xE1
   #define FDEVMSB_50000            0x03
   #define FDEVLSB_50000            0x33
   #define FDEVMSB_55000            0x03
   #define FDEVLSB_55000            0x85
   #define FDEVMSB_60000            0x03
   #define FDEVLSB_60000            0xD7
   #define FDEVMSB_65000            0x04
   #define FDEVLSB_65000            0x29
   #define FDEVMSB_70000            0x04
   #define FDEVLSB_70000            0x7B
   #define FDEVMSB_75000            0x04
   #define FDEVLSB_75000            0xCD
   #define FDEVMSB_80000            0x05
   #define FDEVLSB_80000            0x1F
   #define FDEVMSB_85000            0x05
   #define FDEVLSB_85000            0x71
   #define FDEVMSB_90000            0x05
   #define FDEVLSB_90000            0xC3
   #define FDEVMSB_95000            0x06
   #define FDEVLSB_95000            0x14
   #define FDEVMSB_100000           0x06
   #define FDEVLSB_100000           0x66
   #define FDEVMSB_110000           0x07
   #define FDEVLSB_110000           0x0A
   #define FDEVMSB_120000           0x07
   #define FDEVLSB_120000           0xAE
   #define FDEVMSB_130000           0x08
   #define FDEVLSB_130000           0x52
   #define FDEVMSB_140000           0x08
   #define FDEVLSB_140000           0xF6
   #define FDEVMSB_150000           0x09
   #define FDEVLSB_150000           0x9A
   #define FDEVMSB_160000           0x0A
   #define FDEVLSB_160000           0x3D
   #define FDEVMSB_170000           0x0A
   #define FDEVLSB_170000           0xE1
   #define FDEVMSB_180000           0x0B
   #define FDEVLSB_180000           0x85
   #define FDEVMSB_190000           0x0C
   #define FDEVLSB_190000           0x29
   #define FDEVMSB_200000           0x0C
   #define FDEVLSB_200000           0xCD
   #define FDEVMSB_210000           0x0D
   #define FDEVLSB_210000           0x71
   #define FDEVMSB_220000           0x0E
   #define FDEVLSB_220000           0x14
   #define FDEVMSB_230000           0x0E
   #define FDEVLSB_230000           0xB8
   #define FDEVMSB_240000           0x0F
   #define FDEVLSB_240000           0x5C
   #define FDEVMSB_250000           0x10
   #define FDEVLSB_250000           0x00
   #define FDEVMSB_260000           0x10
   #define FDEVLSB_260000           0xA4
   #define FDEVMSB_270000           0x11
   #define FDEVLSB_270000           0x48
   #define FDEVMSB_280000           0x11
   #define FDEVLSB_280000           0xEC
   #define FDEVMSB_290000           0x12
   #define FDEVLSB_290000           0x8F
   #define FDEVMSB_300000           0x13
   #define FDEVLSB_300000           0x33

#endif
