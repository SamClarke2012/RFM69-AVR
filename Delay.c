#include <stdint.h>

void nop(void) {
    __asm__ __volatile__ ("nop\n\t"::);
}

void nop_milli(void) {
    for (uint32_t i = 0; i <= 293; i++) { nop(); }
}

void nop_sec(void) {
    for (uint32_t i = 0; i <= 291783; i++) { nop(); }
}

void wait_seconds(uint8_t seconds) {
    for (int i = 1; i <= seconds; i++) {
        nop_sec();
    }
}

void wait_millis(uint16_t millis) {
    for (int i = 1; i <= millis; i++) {
        nop_milli();
    }
}

