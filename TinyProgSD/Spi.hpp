#pragma once

#include <Arduino.h>

namespace Spi {
    const uint8_t maskClk = 0x01;           // D0
    const uint8_t maskMiso = 0x02;          // D1
    const uint8_t maskMosi = 0x04;          // D2
    const uint8_t maskCs = 0x08;            // D3

    void init();
    void sendData(uint8_t *data, uint32_t len);
    void pulseClock();
}
