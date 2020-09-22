#pragma once

#include <Arduino.h>

namespace Spi {
    void init();
    uint8_t transfer(uint8_t data);
    void select();
    void deselect();
}
