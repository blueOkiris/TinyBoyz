#include <Arduino.h>
#include "Spi.hpp"
#include "Sd.hpp"

using namespace SdCard;

static int16_t i, j;
static uint8_t readByte;
static uint8_t buff[8];

bool SdCard::init() {
    Spi::init();

    // read 10 bytes
    Spi::deselect();
    for(i = 0; i < 10; i++) {
        Spi::transfer(0xFF);
    }
    
    // Try to go into spi mode
    for(i = 0; i < 10 && SdCard::command(0x40, 0x00000000, 0x95, 8) != 1; i++) {
        delay(100);
    }
    if(i == 10) {
        return false;
    }

    // Try to wake up card
    for(i = 0; i < 10 && SdCard::command(0x41, 0x00000000, 0xFF, 8) != 0; i++) {
        delay(100);
    }
    if(i == 10) {
        return false;
    }

    // Set block size to 512
    SdCard::command(0x50, 0x00000200, 0xFF, 8);
    
    return true;
}

uint8_t SdCard::command(
        uint8_t cmd, uint32_t arg, uint8_t crc, uint8_t readLen) {
    Spi::select();

    Spi::transfer(cmd);
    Spi::transfer(arg >> 24);
    Spi::transfer(arg >> 16);
    Spi::transfer(arg >> 8);
    Spi::transfer(arg);
    Spi::transfer(crc);

    readByte = 0xFF;
    for(j = 0; j < readLen; j++) {
        buff[j] = Spi::transfer(0xFF);
        if(buff[j] != 0xFF) {
            readByte = buff[j];
        }
    }

    Spi::deselect();

    return readByte;
}

void SdCard::read(
        uint32_t sector, uint16_t offset, uint8_t *buffer, uint16_t len) {
    j = 0;

    Spi::select();

    Spi::transfer(0x51);
    Spi::transfer(sector >> 15);                        // sector * 512 >> 24
    Spi::transfer(sector >> 7);                         // sector * 512 >> 16
    Spi::transfer(sector << 1);                         // sector * 512 >> 8
    Spi::transfer(0);                                   // sector * 512
    Spi::transfer(0xFF);

    // Wait for 0
    for(j = 0; j < 10 && Spi::transfer(0xFF) != 0x00; j++) {}
    // Wair for data start
    for(j = 0; j < 10 && Spi::transfer(0xFF) != 0xFE; j++) {}

    for(j = 0; j < offset; j++) {                       // Skip bytes
        Spi::transfer(0xFF);
    }
    for(j = 0; j < len; j++) {
        buffer[i] = Spi::transfer(0xFF);
    }
    for(j += offset; j < 512; j++) {                    // Skip again
        Spi::transfer(0xFF);
    }

    // Skip checksum
    Spi::transfer(0xFF);
    Spi::transfer(0xFF);

    Spi::deselect();
}
