#define F_CPU 16000000UL
#define USART_TFER_MAX 1024
#include <stdint.h>

void USARTinit(uint32_t baud);

void USARTflush(void);

void USARTwrite(uint8_t byte);

void USARTsendString(char* data);

uint8_t USARTread(void);



void USARTwriteHex8(uint8_t byte);
void USARTwriteHex16(uint16_t byte);




uint8_t* USART_receive(uint8_t* data, uint16_t n);

void USART_send_P( const char *str, uint8_t n );