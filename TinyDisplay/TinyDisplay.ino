#include "Ili9341.hpp"

void setup() {
    Display::init();
    Display::fillRect(0, 0, 320, 240, BLACK);
    Display::fillRect(50, 50, 50, 50, BLUE);
}

void loop() {

}
