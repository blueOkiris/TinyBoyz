#include <TinyWire.hpp>

#define SDA_PIN     4
#define SCL_PIN     3
#define DISP_ADDR   0x00
#define SD_ADDR     0x01
#define FRAM_ADDR   0x02
#define INPUT_ADDR  0x03

char registers[4] = { 0, 0, 0, 0 };
unsigned char dataBuffer[8];

void waitForSdReady() {
    dataBuffer[0] = 0x00;
    while(dataBuffer[0] != 'R') {
        TinyWire::AsMaster::write(SD_ADDR, '?');
        dataBuffer[0] = TinyWire::AsMaster::read(SD_ADDR);
    }
}

void setup() {
    TinyWire::interfaceMask.sdaMask = 0x01 << SDA_PIN;
    TinyWire::interfaceMask.sclMask = 0x01 << SCL_PIN;
    TinyWire::AsMaster::initInterface();

    waitForSdReady();
}

void loop() {
    
}

