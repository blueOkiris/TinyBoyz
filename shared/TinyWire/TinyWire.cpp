#include <Arduino.h>
#include <TinyWire.hpp>

using namespace TinyWire;

Interface::Interface(char sdaPin, char sclPin) {
    _readByte = 0;
    _sdaMask = 0x01 << sdaPin;
    _sclMask = 0x01 << sclPin;
}

MasterInterface::MasterInterface(char sdaPin, char sclPin) :
        Interface(sdaPin, sclPin) {
    DDRB |= _sclMask;
    PORTB |= _sclMask;
}

SlaveInterface::SlaveInterface(char sdaPin, char sclPin) :
        Interface(sdaPin, sclPin) {
    DDRB &= ~_sclMask;
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

char SlaveInterface::read(char addr) {
    _waitUntilMasterTalking(addr);
    _sendAcknowledge();
    _retrieveData();
    
    return _readByte;
}

void SlaveInterface::write(char addr, char data) {
    _waitUntilMasterTalking(addr);
    _sendAcknowledge();
    
    DDRB |= _sdaMask;
    
    for(i = 7; i >= 0; i--) {
        while(PORTB & _sclMask != 0);
        PORTB = ((data >> i) & 0x01) ?
            (PORTB | _sdaMask) :
            (PORTB & _sdaMask);
        while(PORTB & _sclMask == 0);
    }
}

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

void SlaveInterface::_waitForMaster() {
    // Wait for start signal
    DDRB &= ~_sdaMask;
    while(1) {
        if(PINB & _sclMask != 0 && PINB & _sdaMask == 0) {
            while(PINB & _sclMask != 0) {
                if(PINB & _sdaMask != 0) {
                    break;
                }
            }
            
            if(PINB & _sdaMask == 0) {
                break;
            }
        }
    }
}

void SlaveInterface::_retrieveData() {
    DDRB &= ~_sdaMask;
    _readByte = 0;
    
    for(i = 7; i >= 0; i--) {
        while(PORTB & _sclMask != 0);
        _readByte |= (PINB & _sdaMask != 0) << i;
        while(PORTB & _sclMask == 0);
    }
}

void SlaveInterface::_sendAcknowledge() {
    while(PORTB & _sclMask != 0);
    DDRB |= _sdaMask;
    PORTB &= ~_sdaMask;
    DDRB &= ~_sdaMask;
    while(PORTB & _sclMask == 0);
}

// Check that the master is talking to self
void SlaveInterface::_waitUntilMasterTalking(char addr) {
    while(1) {
        _waitForMaster();
        _retrieveData(); // Will be address
        
        if(_readByte == addr) {
            break;
        }
    }
}
