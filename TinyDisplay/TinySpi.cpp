#include <Arduino.h>
#include "TinySpi.hpp"

using namespace Spi;

void Spi::init() {
    maskMosi = 0x01 << PIN_MOSI;
    maskScl = 0x01 << PIN_SCL;
    maskCs = 0x01 << PIN_CS;
    maskCd = 0x01 << PIN_CD;

    DDRB |= maskMosi;
    DDRB |= maskScl;
    DDRB |= maskCs;
    DDRB |= maskCd;

    PORTB |= maskCs;
    PORTB &= ~maskScl;
}

void Spi::writeByte(uint8_t byte) {
    for(int8_t bit = 7; bit >= 0; bit--) {
        PORTB = ((byte >> bit) & 0x01) ?
            (PORTB | maskMosi) :
            (PORTB & ~maskMosi);
        PORTB |= maskScl;
        PORTB &= ~maskScl;
    }
}

void Spi::sendCommand(uint8_t cmd, uint8_t *data, uint8_t len) {
    PORTB &= ~maskCs;

    PORTB &= ~maskCd;
    writeByte(cmd);

    PORTB |= maskCd;
    for(uint8_t i = 0; i < len; i++) {
        writeByte(data[i]);
    }

    PORTB |= maskCs;
}

void Spi::sendCommand(uint8_t cmd) {
    PORTB &= ~maskCs;
    PORTB &= ~maskCd;
    writeByte(cmd);
    PORTB |= maskCs;
}

void Spi::fillRam16(uint8_t cmd, uint16_t data, uint32_t len) {
    PORTB &= ~maskCs;

    PORTB &= ~maskCd;
    writeByte(cmd);

    uint8_t hi = data >> 8;
    uint8_t lo = data & 0x00FF;

    PORTB |= maskCd;
    for(uint32_t i = 0; i < len; i++) {
        for(int8_t bit = 7; bit >= 0; bit--) {
            PORTB = ((hi >> bit) & 0x01) ?
                (PORTB | maskMosi) :
                (PORTB & ~maskMosi);
            PORTB |= maskScl;
            PORTB &= ~maskScl;
        }
        for(int8_t bit = 7; bit >= 0; bit--) {
            PORTB = ((lo >> bit) & 0x01) ?
                (PORTB | maskMosi) :
                (PORTB & ~maskMosi);
            PORTB |= maskScl;
            PORTB &= ~maskScl;
        }
    }

    PORTB &= ~maskMosi;
    for(int8_t bit = 7; bit >= 0; bit--) {
        PORTB |= maskScl;
        PORTB &= ~maskScl;
    }

    PORTB |= maskCs;
}