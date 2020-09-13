#include <TinyWire.hpp>

#define SDA_PIN 4
#define SCL_PIN 3
#define ADDR    0x01

char dataBuff[8];

void setup() {
    TinyWire::interfaceMask.sdaMask = 0x01 << SDA_PIN;
    TinyWire::interfaceMask.sclMask = 0x01 << SCL_PIN;
    TinyWire::AsSlave::initInterface();

    dataBuff[0] = 0x00;
    while(dataBuff[0] != '?') {
        TinyWire::AsSlave::read(ADDR);
    }
    TinyWire::AsSlave::write(ADDR, 'R');
}

void loop() {
    
}
