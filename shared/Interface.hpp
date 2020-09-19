#pragma once

#include <Arduino.h>
#include "Display.hpp"

namespace Parallel {
    void init(Display::TinyDispType tinyDispId);
    void sendCommand(uint8_t cmd);
    void sendCommand(uint8_t cmd, uint8_t *data, uint32_t len);
    void sendData(uint8_t data);
    void pulseOrWaitClock();
}
