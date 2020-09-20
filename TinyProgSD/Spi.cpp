#include <Arduino.h>
#include "Spi.hpp"

using namespace Spi;

void Spi::init() {
    DDRB = 0x02;                        // MISO as input, else as output
    PORTB &= ~maskClk;
    PORTB |= maskCs;
}

void Spi::pulseClock() {
    PORTB |= maskClk;
    PORTB &= ~maskClk;
}

void Spi::sendData(uint8_t *data, uint32_t len) {
    PORTB &= ~maskCs;

    for(uint32_t i = 0; i < len; i++) {
        for(int8_t bit = 7; bit >= 0; bit--) {
            PORTB = ((data[i] >> bit) & 0x01) ?
                (PORTB | maskMosi) :
                (PORTB & ~maskMosi);
            pulseClock();
        }
    }

    PORTB |= maskCs;
}
