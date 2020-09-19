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
    DDRB |= 0x07;                                       // D[0:2] -> output
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
        for(uint8_t i = 0; i < 4; i++);

        PORTB |= maskClk;
        PORTB &= ~maskClk;
    } else {
        // Wait for clk pulse
        while((PINB & maskClk) == 0);
    }
}

void Parallel::sendData(uint8_t data) {
    // Write data
    uint8_t iStart, iEnd, i;
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
            PORTB |= maskDnC;
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

    if(tinyDispId_g != Display::TinyDispType::LowAndCtlBits) {
        for(i = iStart; i >= iEnd; i--) {
            PORTB &= ~maskD[i - iEnd];
        }
    }
}

void Parallel::sendCommand(uint8_t cmd) {
    // Write data
    uint8_t iStart, iEnd, i;
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
            PORTB &= ~maskDnC;
            iStart = 1;
            iEnd = 0;
            break;
    }
    for(uint8_t i = iStart; i >= iEnd; i--) {
        PORTB = ((cmd >> i) & 0x01) ?
            (PORTB | maskD[i - iEnd]) :
            (PORTB & ~maskD[i - iEnd]);
    }

    pulseOrWaitClock();
}

void Parallel::sendCommand(uint8_t cmd, uint8_t *data, uint32_t len) {
    sendCommand(cmd);
    for(uint32_t i = 0; i < len; i++) {
        sendData(data[i]);
    }
}
