#include <Arduino.h>
#include "Spi.hpp"

using namespace Spi;

static const uint8_t maskCs = 0x08;     // D3
static const uint8_t maskMosi = 0x02;   // D1
static const uint8_t maskMiso = 0x04;   // D2
static const uint8_t maskSck = 0x01;    // D0

static int8_t bit;
static uint8_t readData;

void Spi::init() {
    DDRB = 0xFF & (~maskMiso);                  // Just miso as input
    PORTB |= maskCs;                            // Set cs to high by default
    PORTB |= maskSck;                           // Set clk to high by default
}

uint8_t Spi::transfer(uint8_t data) {
    readData = 0;

    for(bit = 7; bit >= 0; bit--) {
        PORTB = ((data >> bit) & 0x01) ?        // Set to data
            (PORTB | maskMosi) :
            (PORTB & ~maskMosi);
        readData = ((PINB & maskMiso) != 0) ?   // Read on MISO
            (readData | (0x01 << bit)) :
            (readData & ~(0x01 << bit));
    }

    return readData;
}

void Spi::select() {
    PORTB &= ~maskCs;
}

void Spi::deselect() {
    PORTB |= maskCs;
}
