#include <Arduino.h>
#include "Ili9341.hpp"
#include "Interface.hpp"
#include "Display.hpp"

using namespace Display;

static TinyDispType tinyDispId_g;

void Display::init(TinyDispType tinyDispId) {
    tinyDispId_g = tinyDispId;
    Parallel::init(tinyDispId_g);

    Parallel::sendCommand(0b01010110);
    /*Parallel::sendCommand(ILI_RST);
    delay(100);

    for(uint8_t ind = 0; ind < 110; ind++) {
        uint8_t cmd = iliInitCmds_g[ind++];
        uint8_t numData = iliInitCmds_g[ind++];
        Parallel::sendCommand(cmd, iliInitCmds_g + ind, numData & 0x7F);
        if(numData & 0x80) {
            delay(100);
        }
        ind += (numData & 0x7F) - 1;
    }*/
}

void Display::setAddrWindow(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h) {
    uint16_t x2 = (x1 + w - 1), y2 = (y1 + h - 1);
    uint8_t dataX[4] = {
        x1 >> 8, x1 & 0x00FF, x2 >> 8, x2 & 0x00FF
    };
    uint8_t dataY[4] = {
        y1 >> 8, y1 & 0x00FF, y2 >> 8, y2 & 0x00FF
    };
    Parallel::sendCommand(ILI_COL_ADDR_SET, dataX, 4);
    Parallel::sendCommand(ILI_PG_ADDR_SET, dataY, 4);
}

void Display::fillRect(
        uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    /*setAddrWindow(x, y, w, h);

    Parallel::sendCommand(ILI_RAM_WRITE);

    Parallel::setDataMode();
    uint32_t len = static_cast<uint32_t>(w) * static_cast<uint32_t>(h);
    for(uint32_t i = 0; i < 5; i++) {
        Parallel::sendData(color >> 8);
        Parallel::sendData(color & 0x00FF);
    }
    Parallel::sendData(0x00);*/
}

/*
 * Old Code
 *
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
*/
