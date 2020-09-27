#include <Arduino.h>
#include "Ili9341.hpp"
#include "Display.hpp"

using namespace Ili9341Parallel;

static Board dispId_g;

static const uint8_t maskTinyWr = 0x01;
static const uint8_t maskTinyRs = 0x02;
static const uint8_t maskTinyLowData = 0x06;
static const uint8_t maskTinyHighMidData = 0x0E;

#if defined(__AVR_ATmega328P__)
static const uint8_t pinUnoRd = A0;
static const uint8_t pinUnoWr = A1;
static const uint8_t pinUnoRs = A2;
static const uint8_t pinUnoCs = A3;
static const uint8_t pinUnoReset = A4;
#else
static const uint8_t pinUnoRd = 0;
static const uint8_t pinUnoWr = 0;
static const uint8_t pinUnoRs = 0;
static const uint8_t pinUnoCs = 0;
static const uint8_t pinUnoReset = 0;
#endif
static const uint8_t unoPinsData[8] = { 8, 9, 2, 3, 4, 5, 6, 7 };

void Ili9341Parallel::writeByte(uint8_t data) {
    switch(dispId_g) {
        case Board::Uno:
            for(int8_t pin = 0; pin < 8; pin++) {
                digitalWrite(unoPinsData[pin], (data >> pin) & 0x01);
            }
            digitalWrite(pinUnoWr, LOW);
            digitalWrite(pinUnoWr, HIGH);
            break;
        
        case Board::HighTiny:
            PORTB = (data >> 5) & maskTinyHighMidData;
            while((PINB & maskTinyWr) != 0); // Wait for low on clock
            break;
        
        case Board::MidTiny:
            PORTB = (data >> 2) & maskTinyHighMidData;
            while((PINB & maskTinyWr) != 0); // Wait for low on clock
            break;
        
        case Board::LowAndCtlTiny:
            PORTB = data & maskTinyLowData;
            PORTB &= ~maskTinyWr;
            PORTB |= maskTinyWr;
            break;
    }
}

void Ili9341Parallel::writeRegByte(uint8_t cmd, uint8_t data) {
    switch(dispId_g) {
        case Board::Uno:
            digitalWrite(pinUnoRs, LOW);
            break;
        
        case Board::LowAndCtlTiny:
            PORTB &= ~maskTinyRs;
            break;
    }
    writeByte(cmd);
    
    switch(dispId_g) {
        case Board::Uno:
            digitalWrite(pinUnoRs, HIGH);
            break;
        
        case Board::LowAndCtlTiny:
            PORTB |= maskTinyRs;
            break;
    }
    writeByte(cmd);
}

void Ili9341Parallel::writeRegShort(uint16_t cmd, uint16_t data) {
    switch(dispId_g) {
        case Board::Uno:
            digitalWrite(pinUnoRs, LOW);
            break;
        
        case Board::LowAndCtlTiny:
            PORTB &= ~maskTinyRs;
            break;
    }
    writeByte(cmd >> 8);
    writeByte(cmd);
    
    switch(dispId_g) {
        case Board::Uno:
            digitalWrite(pinUnoRs, HIGH);
            break;
        
        case Board::LowAndCtlTiny:
            PORTB |= maskTinyRs;
            break;
    }
    writeByte(data >> 8);
    writeByte(data);
}

void Ili9341Parallel::writeRegLong(uint8_t cmd, uint32_t data) {
    switch(dispId_g) {
        case Board::Uno:
            digitalWrite(pinUnoRs, LOW);
            break;
        
        case Board::LowAndCtlTiny:
            PORTB &= ~maskTinyRs;
            break;
    }
    writeByte(cmd);
    
    switch(dispId_g) {
        case Board::Uno:
            digitalWrite(pinUnoRs, HIGH);
            break;
        
        case Board::LowAndCtlTiny:
            PORTB |= maskTinyRs;
            break;
    }
    writeByte(data >> 24);
    writeByte(data >> 16);
    writeByte(data >> 8);
    writeByte(data);
}

void Ili9341Parallel::reset() {
    switch(dispId_g) {
        case Board::Uno:
            digitalWrite(pinUnoCs, HIGH);
            digitalWrite(pinUnoWr, HIGH);
            digitalWrite(pinUnoRd, HIGH);
            digitalWrite(pinUnoReset, HIGH);
            pinMode(pinUnoReset, OUTPUT);
            
            digitalWrite(pinUnoReset, LOW);
            delay(2);
            digitalWrite(pinUnoReset, HIGH);
            
            digitalWrite(pinUnoCs, LOW);
            digitalWrite(pinUnoRs, LOW);
        
        case Board::LowAndCtlTiny:
            PORTB |= maskTinyWr;
            PORTB &= ~maskTinyRs;
            break;
    }
    writeByte(0x00);
    switch(dispId_g) {
        case Board::Uno:
            for(int8_t i = 0; i < 3; i++) {
                digitalWrite(pinUnoWr, LOW);
                digitalWrite(pinUnoWr, HIGH);
            }
            digitalWrite(pinUnoCs, HIGH);
            break;
            
        case Board::LowAndCtlTiny:
            for(int8_t i = 0; i < 3; i++) {
                PORTB &= ~maskTinyWr;
                PORTB |= maskTinyWr;
            }
            break;
    }
}

void Ili9341Parallel::begin() {
    switch(dispId_g) {
        case Board::Uno:
            digitalWrite(pinUnoCs, LOW);
            break;
    }

    writeRegByte(ILI_SOFT_RST, 0);
    delay(50);
    writeRegByte(ILI_DISP_OFF, 0);
    
    writeRegByte(ILI_PWR_CTL_1, 0x23);
    writeRegByte(ILI_PWR_CTL_2, 0x10);
    writeRegShort(ILI_V_COM_CTL_1, 0x2B2B);
    writeRegByte(ILI_V_COM_CTL_2, 0xC0);
    writeRegByte(ILI_MEM_CTL, ILI_MAD_CTL_MY | ILI_MAD_CTL_BGR);
    writeRegByte(ILI_PXL_FRMT, 0x55);
    writeRegShort(ILI_FRAME_CTL, 0x001B);
    
    writeRegByte(ILI_ENTRY_MODE, 0x07);
    
    writeRegByte(ILI_SLEEP_OUT, 0);
    delay(150);
    writeRegByte(ILI_DISP_ON, 0);
    delay(500);
}

void Ili9341Parallel::init(Board dispId) {
    dispId_g = dispId;
    
    switch(dispId_g) {
        case Board::Uno:
            pinMode(pinUnoCs, OUTPUT);
            pinMode(pinUnoRs, OUTPUT);
            pinMode(pinUnoWr, OUTPUT);
            pinMode(pinUnoRd, OUTPUT);
            for(int8_t pin = 0; pin < 8; pin++) {
                pinMode(unoPinsData[pin], OUTPUT);
            }
            break;
        
        case Board::HighTiny:
        case Board::MidTiny:
            DDRB = 0xFE;
            break;
        
        case Board::LowAndCtlTiny:
            DDRB = 0xFF;
            break;
    }
            
    reset();
    delay(200);
    
    begin();
}

void Ili9341Parallel::setAddrWindow(
        uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    switch(dispId_g) {
        case Board::Uno:
            digitalWrite(pinUnoCs, LOW);
            break;
    }
    
    uint32_t col = x1;
    col <<= 16;
    col |= x2;
    
    uint32_t page = y1;
    page <<= 16;
    page |= y2;
    
    writeRegLong(ILI_COL_ADDR_SET, col);
    writeRegLong(ILI_PAGE_ADDR_SET, col);
    
    switch(dispId_g) {
        case Board::Uno:
            digitalWrite(pinUnoCs, HIGH);
            break;
    }
}

void Ili9341Parallel::flood(uint16_t color, uint32_t len) {
    uint16_t blocks;
    uint8_t i, high = color >> 8, low = color;
    
    switch(dispId_g) {
        case Board::Uno:
            digitalWrite(pinUnoCs, LOW);
            digitalWrite(pinUnoRs, LOW);
            break;
        
        case Board::LowAndCtlTiny:
            PORTB &= ~maskTinyRs;
            break;
    }
    writeByte(0x2C);
    
    switch(dispId_g) {
        case Board::Uno:
            digitalWrite(pinUnoRs, HIGH);
            break;
        
        case Board::LowAndCtlTiny:
            PORTB |= maskTinyRs;
            break;
    }
    writeByte(high);
    writeByte(low);
    len--;
    blocks = (uint16_t) (len / 64);
    while(blocks--) {
        i = 16;
        do {
            writeByte(high);
            writeByte(low);
            writeByte(high);
            writeByte(low);
            writeByte(high);
            writeByte(low);
            writeByte(high);
            writeByte(low);
        } while(--i);
        for (i = (uint8_t) len & 63; i--;) {
            writeByte(high);
            writeByte(low);
        }
    }
    
    switch(dispId_g) {
        case Board::Uno:
            digitalWrite(pinUnoCs, HIGH);
            break;
    }
}

void Ili9341Parallel::fillRect(
        uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    setAddrWindow(x, y, w, h);
    flood(color, (uint32_t) w * (uint32_t) h);
}
