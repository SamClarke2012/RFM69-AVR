

/*_________________________________________________________________
 |__SPIE___|_ SPE___|__DORD__|_MSTR__|_CPOL_|_CPHA__|_SPR1__|_SPR0_|
 |Interrupt|_Enable_|___LSB__|_Master|_Edge_|_Phase_|_Speed_|_Speed|
 |____7____|___6____|____5___|___4___|___3__|___2___|___1___|___0__|
SPI freq dividers, Big-endian natively, Polarity/Phase as standard, No ISR. */
   #define FOSC2   ((1 << SPE)|(1 << MSTR)|(0 << SPR1)|(0 << SPR0)) // 8MHz
   #define FOSC4   ((1 << SPE)|(1 << MSTR)|(0 << SPR1)|(0 << SPR0)) // 4MHz
   #define FOSC8   ((1 << SPE)|(1 << MSTR)|(0 << SPR1)|(1 << SPR0)) // 2MHz
   #define FOSC16  ((1 << SPE)|(1 << MSTR)|(0 << SPR1)|(1 << SPR0)) // 1MHz
   #define FOSC32  ((1 << SPE)|(1 << MSTR)|(1 << SPR1)|(0 << SPR0)) // 500KHz
   #define FOSC64  ((1 << SPE)|(1 << MSTR)|(1 << SPR1)|(1 << SPR0)) // 250KHz
   #define FOSC128 ((1 << SPE)|(1 << MSTR)|(1 << SPR1)|(1 << SPR0)) // 125KHz

   void SPI_master_init( uint8_t div );
   uint8_t SPI_transfer( uint8_t byte );