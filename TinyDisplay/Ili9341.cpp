#include <Arduino.h>
#include "Ili9341.hpp"
#include "TinySpi.hpp"

using namespace Display;

void Display::init() {
    Spi::init();

    Spi::sendCommand(ILI_RST);
    delay(150);

    for(uint8_t ind = 0; ind < 110; ind++) {
        uint8_t cmd = iliInitCmds_g[ind++];
        uint8_t numData = iliInitCmds_g[ind++];
        Spi::sendCommand(cmd, iliInitCmds_g + ind, numData & 0x7F);
        if(numData & 0x80) {
            delay(100);
        }
        ind += (numData & 0x7F) - 1;
    }
}

void Display::setAddrWindow(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h) {
    uint16_t x2 = (x1 + w - 1), y2 = (y1 + h - 1);
    uint8_t dataX[4] = {
        x1 >> 8, x1 & 0x00FF, x2 >> 8, x2 & 0x00FF
    };
    uint8_t dataY[4] = {
        y1 >> 8, y1 & 0x00FF, y2 >> 8, y2 & 0x00FF
    };
    Spi::sendCommand(ILI_COL_ADDR_SET, dataX, 4);
    Spi::sendCommand(ILI_PG_ADDR_SET, dataY, 4);
}

void Display::fillRect(
        uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    setAddrWindow(x, y, w, h);
    Spi::fillRam16(ILI_RAM_WRITE, color, (uint32_t ) w * h);
}