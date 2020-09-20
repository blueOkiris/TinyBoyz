#include <Arduino.h>
#include "Interface.hpp"

using namespace Parallel;

static Display::TinyDispType tinyDispId_g;

static const uint8_t maskD[3] = { 0x01, 0x02, 0x04 };   // D0, D1, D2
static const uint8_t maskClk = 0x08;                    // D3
static const uint8_t maskDnC = 0x04;                    // D2

void Parallel::init(Display::TinyDispType tinyDispId) {
    tinyDispId_g = tinyDispId;

    // Pins D0, D1, D2, and D3 are all used, but D3 could be input
    // D3 is output if we're the control one
    DDRB |= 0xFF;                                       // D[7:0] -> output
    switch(tinyDispId_g) {
        case Display::TinyDispType::HighBits:
        case Display::TinyDispType::MidBits:
            DDRB &= ~maskClk;                           // input
            break;
        
        case Display::TinyDispType::LowAndCtlBits:
            DDRB |= maskClk;                            // output
            PORTB &= ~maskClk;                          // off
            break;
    }
}

void Parallel::pulseOrWaitClock() {
    if(tinyDispId_g == Display::TinyDispType::LowAndCtlBits) {
        // Wait for just a handful of steps to make sure all three have written
        delay(100);
        PORTB |= maskClk;
        delay(100);
        PORTB &= ~maskClk;
    } else {
        // Wait for clk pulse
        while((PINB & maskClk) != 0);                   // To get enough voltage
                                                        // And inverter was used
    }
}

void Parallel::setDataMode() {
    if(tinyDispId_g == Display::TinyDispType::LowAndCtlBits) {
        PORTB |= maskDnC;
    }
}

void Parallel::setCommandMode() {
    if(tinyDispId_g == Display::TinyDispType::LowAndCtlBits) {
        PORTB &= ~maskDnC;
    }
}

void Parallel::sendData(uint8_t data) {
    // Write data
    int8_t iStart, iEnd, i;
    switch(tinyDispId_g) {
        case Display::TinyDispType::HighBits:
            iStart = 7;
            iEnd = 5;
            break;

        case Display::TinyDispType::MidBits:
            iStart = 4;
            iEnd = 2;
            break;
        
        case Display::TinyDispType::LowAndCtlBits:
            iStart = 1;
            iEnd = 0;
            break;
    }
    for(i = iStart; i >= iEnd; i--) {
        PORTB = ((data >> i) & 0x01) ?
            (PORTB | maskD[i - iEnd]) :
            (PORTB & ~maskD[i - iEnd]);
    }
    pulseOrWaitClock();
    for(i = iStart; i >= iEnd; i--) {
        PORTB &= ~maskD[i - iEnd];
    }
}

void Parallel::sendCommand(uint8_t cmd) {
    setCommandMode();
    sendData(cmd);
}

void Parallel::sendCommand(uint8_t cmd, uint8_t *data, uint32_t len) {
    setCommandMode();
    sendData(cmd);

    if(tinyDispId_g == Display::TinyDispType::LowAndCtlBits) {
        PORTB |= maskDnC;
    }
    for(int32_t i = 0; i < len; i++) {
        sendData(data[i]);
    }
}
