#include <Arduino.h>
#include "Interface.hpp"

using namespace Parallel;

static Display::TinyDispType tinyDispId_g;

static const uint8_t maskD[3] = { 0x08, 0x02, 0x04 };   // D3, D1, D2
static const uint8_t maskClk = 0x01;                    // D0
static const uint8_t maskDnC = 0x04;                    // D2

static const uint8_t unoPinWr = A1;
static const uint8_t unoPinRs = A2;
static const uint8_t unoPinsData = { 8, 9, 2, 3, 4, 5, 6, 7 };

void Parallel::init(Display::TinyDispType tinyDispId) {
    tinyDispId_g = tinyDispId;

    // Pins D0, D1, D2, and D3 are all used, but D3 could be input
    // D3 is output if we're the control one
    switch(tinyDispId_g) {
        case Display::DispType::HighBits:
        case Display::DispType::MidBits:
            DDRB |= 0xFF;                               // D[7:0] -> output
            DDRB &= ~maskClk;                           // input
            break;
        
        case Display::DispType::LowAndCtlBits:
            DDRB |= 0xFF;                               // D[7:0] -> output
            DDRB |= maskClk;                            // output
            PORTB &= ~maskClk;                          // off
            break;
        
        // Do it all for the Uno test
        case Display::DispType::Uno:
            pinMode(A3, OUTPUT);
            digitalWrite(A3, LOW);
            
            pinMode(unoPinWr, OUTPUT);
            pinMode(unoPinRs, OUTPUT);
            for(uint8_t pin : unoPinsData) {
                pinMode(pin, OUTPUT);
                digitalWrite(pin, LOW);
            }            
            break;
    }
}

void Parallel::pulseOrWaitClock() {
    if(tinyDispId_g == Display::DispType::LowAndCtlBits) {
        // Wait for just a handful of steps to make sure all three have written
        delay(1);
        PORTB |= maskClk;
        delay(1);
        PORTB &= ~maskClk;
    } else if(Display::DispType::Uno) {
        delay(1);
        digitalWrite(unoPinWr, HIGH);
        delay(1);
        digitalWrite(unoPinWr, LOW);
    } else {
        // Wait for clk pulse
        while((PINB & maskClk) != 0);                   // To get enough voltage
                                                        // And inverter was used
    }
}

void Parallel::setDataMode() {
    if(tinyDispId_g == Display::DispType::LowAndCtlBits) {
        PORTB |= maskDnC;
    } else if(tinyDispId_g == Display::DispType::Uno) {
        digitalWrite(unoPinRs, HIGH);
    }
}

void Parallel::setCommandMode() {
    if(tinyDispId_g == Display::DispType::LowAndCtlBits) {
        PORTB &= ~maskDnC;
    } else if(tinyDispId_g == Display::DispType::Uno) {
        digitalWrite(unoPinRs, LOW);
    }
}

void Parallel::sendData(uint8_t data) {
    // Write data
    if(tinyDispId_g == Display::DispType::Uno) {
        for(int8_t pin = 7; pin >= 0; pin--) {
            digitalWrite(unoPinsData[pin], (data >> pin) & 0x01);
        }
        pulseOrWaitClock();
        for(int8_t pin = 7; pin >= 0; pin--) {
            digitalWrite(unoPinsData[pin], LOW);
        }
    } else {
        int8_t iStart, iEnd, i;
        switch(tinyDispId_g) {
            case Display::DispType::HighBits:
                iStart = 7;
                iEnd = 5;
                break;

            case Display::DispType::MidBits:
                iStart = 4;
                iEnd = 2;
                break;
            
            case Display::DispType::LowAndCtlBits:
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
}

void Parallel::sendCommand(uint8_t cmd) {
    setCommandMode();
    sendData(cmd);
}

void Parallel::sendCommand(uint8_t cmd, uint8_t *data, uint32_t len) {
    setCommandMode();
    sendData(cmd);

    setDataMode();
    for(int32_t i = 0; i < len; i++) {
        sendData(data[i]);
    }
}
