#include "Color.hpp"
#include "Display.hpp"

void setup() {
    Ili9341Parallel::init(Ili9341Parallel::Board::Uno);
    Ili9341Parallel::fillRect(0, 0, 320, 240, BLACK);
}

void loop() {
    
}
