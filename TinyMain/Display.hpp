#pragma once

#include <Arduino.h>

namespace Display {
    enum class DispType {
        HighBits,
        MidBits,
        LowAndCtlBits,
        Uno
    };
    
    void init(DispType tinyDispId);
    void setAddrWindow(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h);
    void fillRect(
        uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color
    );
}
