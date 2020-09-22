#pragma once

#include <Arduino.h>

namespace SdCard {
    bool init();
    uint8_t command(uint8_t cmd, uint32_t arg, uint8_t crc, uint8_t read);
    void read(uint32_t sector, uint16_t offset, uint8_t *buffer, uint16_t len);
}
