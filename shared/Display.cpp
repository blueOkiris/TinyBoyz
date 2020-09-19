#include "Ili9341.hpp"
#include "Display.hpp"

using namespace Display;

static TinyDispType tinyDispId_g;

void Display::init(TinyDispType tinyDispId) {
    tinyDispId_g = tinyDispId;

    
}

void Display::setAddrWindow(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h) {

}

void Display::fillRect(
        uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    
}
