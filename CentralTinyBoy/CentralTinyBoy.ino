#include "TinyWire.hpp"

#define SDA_PIN     4
#define SCL_PIN     3
#define DISP_ADDR   0x00
#define SD_ADDR     0x01
#define FRAM_ADDR   0x02
#define INPUT_ADDR  0x03

char registers[4] = { 0, 0, 0, 0 };
unsigned char dataBuffer[8];
TinyWire::MasterInterface i2c(SDA_PIN, SCL_PIN);

void waitForSdReady() {
    dataBuffer[0] = 0x00;
    while(dataBuffer[0] != 'R') {
        i2c.write(SD_ADDR, '?');
        dataBuffer[0] = i2c.read(SD_ADDR);
    }
}

void setup() {
    waitForSdReady();
}

void loop() {
    
}

