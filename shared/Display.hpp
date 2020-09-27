#pragma once

#include <Arduino.h>

namespace Ili9341Parallel {
    enum class Board {
        HighBits,
        MidBits,
        LowAndCtlBits,
        Uno
    };
    
    void init(Board dispId);
    void reset();
    void begin();
    
    void writeByte(uint8_t data);
    void writeRegByte(uint8_t cmd, uint8_t data);
    void writeRegShort(uint16_t cmd, uint16_t data);
    void writeRegLong(uint8_t cmd, uint32_t data);
    
    void setAddrWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
    void flood(uint16_t color, uint32_t len);
    void fillRect(
        uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color
    );
}
