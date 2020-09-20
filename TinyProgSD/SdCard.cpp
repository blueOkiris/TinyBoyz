#include <Arduino.h>
#include "SdCard.hpp"
#include "Spi.hpp"

using namespace SdCard;

void SdCard::init() {
    Spi::init();

    // Put the sd card in SPI mode
    PORTB |= Spi::maskCs;
    PORTB |= Spi::maskMosi;
    for(int i = 0; i < 80; i++) {
        Spi::pulseClock();
    }

    uint8_t resetCommand[6] = { 0x04, 0x00, 0x00, 0x00, 0x00, 0x95 };
    Spi::sendData(resetCommand, 6);
}
