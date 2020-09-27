#include "Color.hpp"
#include "Display.hpp"

void setup() {
    Ili9341Parallel::init(Ili9341Parallel::Board::LowAndCtlTiny);
    Ili9341Parallel::fillRect(0, 0, 320, 240, BLUE);
}

void loop() {
    
}
