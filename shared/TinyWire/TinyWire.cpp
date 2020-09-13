#include <Arduino.h>
#include <TinyWire.hpp>

using namespace TinyWire;

void MasterInterface::_pulseClock() {
    PORTB |= _sclMask;
    PORTB &= ~_sclMask;
}
            
void MasterInterface::_waitForAcknowledge() {
    DDRB &= ~_sdaMask;
    while((PINB & _sdaMask) != 0);
    _pulseClock();
    DDRB |= _sdaMask;
}

void MasterInterface::_writeAddr(char addr, char readNWrite) {
    DDRB |= _sdaMask;
    
    PORTB &= ~_sdaMask;
    PORTB &= ~_sclMask;
    
    for(i = 6; i >= 0; i--) {
        PORTB = ((addr >> i) & 0x01) ?
            (PORTB | _sdaMask) :
            (PORTB & _sdaMask);
        _pulseClock();
    }
    
    // Write 'read/not write' signal
    PORTB &= ~_sdaMask;
    _pulseClock();
    
    _waitForAcknowledge();
}

// Send stop signal
void MasterInterface::_stop() {
    DDRB |= _sdaMask;
    PORTB &= ~_sdaMask;
    PORTB |= _sclMask;
}

MasterInterface::MasterInterface(char sdaPin, char sclPin) {
    _readByte = 0;
    _sdaMask = 0x01 << sdaPin;
    _sclMask = 0x01 << sclPin;
    
    DDRB |= _sclMask;
    PORTB |= _sclMask;
}

char MasterInterface::read(char addr) {
    _readByte = 0;
    
    _writeAddr(addr, 1);
    DDRB &= ~_sdaMask;
    
    for(i = 7; i >= 0; i--) {
        PORTB &= ~_sclMask;
        _readByte |= (PINB & _sdaMask != 0) << i;
        PORTB |= _sclMask;
    }
    
    _waitForAcknowledge();
    _stop();
    
    return _readByte;
}

void MasterInterface::write(char addr, char data) {
    _writeAddr(addr, 0);
    DDRB |= _sdaMask;
    
    for(i = 7; i >= 0; i--) {
        PORTB = ((data >> i) & 0x01) ?
            (PORTB | _sdaMask) :
            (PORTB & _sdaMask);
        _pulseClock();
    }
    
    _waitForAcknowledge();
    _stop();
}
