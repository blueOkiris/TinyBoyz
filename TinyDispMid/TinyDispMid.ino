#include "Color.hpp"
#include "Display.hpp"

void setup() {
    Display::init(Display::TinyDispType::MidBits);
    Display::fillRect(0, 0, 50, 50, BLACK);
}

void loop() {

}
