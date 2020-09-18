#ifndef _TINY_SPI_HPP_
#define _TINY_SPI_HPP_

#include <Arduino.h>

#define PIN_MOSI            1
#define PIN_SCL             0
#define PIN_CS              3
#define PIN_CD              2

static uint8_t maskMosi, maskScl, maskCd, maskCs;

namespace Spi {
    void init();
    void writeByte(uint8_t byte);
    void sendCommand(uint8_t cmd, uint8_t *data, uint8_t len);
    void sendCommand(uint8_t cmd);
    void fillRam16(uint8_t cmd, uint16_t data, uint32_t len);
}

#endif