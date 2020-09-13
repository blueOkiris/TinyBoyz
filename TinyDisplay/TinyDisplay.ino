#include <TinyWire.hpp>

#define SDA_PIN 4
#define SCL_PIN 3
#define ADDR    0x00

void setup() {
    TinyWire::interfaceMask.sdaMask = 0x01 << SDA_PIN;
    TinyWire::interfaceMask.sclMask = 0x01 << SCL_PIN;
    TinyWire::AsSlave::initInterface();
}

void loop() {
    
}
