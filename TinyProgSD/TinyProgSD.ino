#include <TinyWire.hpp>

#define SDA_PIN 4
#define SCL_PIN 3
#define ADDR    0x01

TinyWire::SlaveInterface i2c(SDA_PIN, SCL_PIN);
char dataBuff[8];

void setup() {
    dataBuff[0] = 0x00;
    while(dataBuff[0] != '?') {
        i2c.read(ADDR);
    }
    i2c.write(ADDR, 'R');
}

void loop() {
    
}
