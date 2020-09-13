#include <Arduino.h>
#include <TinyWire.hpp>

static char readByte, i;
void masterWriteAddr(char addr, char readNWrite);
void masterPulseClock();
void masterWaitForAck();
void masterStop();
void slaveWaitMasterTalkSelf(char addr);
void slaveRetrieveData();
void slaveSendAck();

void TinyWire::AsMaster::initInterface() {
    DDRB |= TinyWire::interfaceMask.sclMask;
    PORTB |= TinyWire::interfaceMask.sclMask;
}

void TinyWire::AsSlave::initInterface() {
    DDRB &= ~TinyWire::interfaceMask.sclMask;
}

char TinyWire::AsMaster::read(char addr) {
    readByte = 0;
    
    masterWriteAddr(addr, 1);
    DDRB &= ~TinyWire::interfaceMask.sdaMask;
    
    for(i = 7; i >= 0; i--) {
        PORTB &= ~TinyWire::interfaceMask.sclMask;
        readByte |= (PINB & TinyWire::interfaceMask.sdaMask != 0) << i;
        PORTB |= TinyWire::interfaceMask.sclMask;
    }
    
    masterWaitForAck();
    masterStop();
    
    return readByte;
}

void TinyWire::AsMaster::write(char addr, char data) {
    masterWriteAddr(addr, 0);
    DDRB |= TinyWire::interfaceMask.sdaMask;
    
    for(i = 7; i >= 0; i--) {
        PORTB = ((data >> i) & 0x01) ?
            (PORTB | TinyWire::interfaceMask.sdaMask) :
            (PORTB & TinyWire::interfaceMask.sdaMask);
        masterPulseClock();
    }
    
    masterWaitForAck();
    masterStop();
}

char TinyWire::AsSlave::read(char addr) {
    slaveWaitMasterTalkSelf(addr);
    slaveSendAck();
    slaveRetrieveData();
    
    return readByte;
}

void TinyWire::AsSlave::write(char addr, char data) {
    slaveWaitMasterTalkSelf(addr);
    slaveSendAck();
    
    DDRB |= TinyWire::interfaceMask.sdaMask;
    
    for(i = 7; i >= 0; i--) {
        while(PORTB & TinyWire::interfaceMask.sclMask != 0);
        PORTB = ((data >> i) & 0x01) ?
            (PORTB | TinyWire::interfaceMask.sdaMask) :
            (PORTB & TinyWire::interfaceMask.sdaMask);
        while(PORTB & TinyWire::interfaceMask.sclMask == 0);
    }
}

void slaveRetrieveData() {
    DDRB &= ~TinyWire::interfaceMask.sdaMask;
    readByte = 0;
    
    for(i = 7; i >= 0; i--) {
        while(PORTB & TinyWire::interfaceMask.sclMask != 0);
        readByte |= (PINB & TinyWire::interfaceMask.sdaMask != 0) << i;
        while(PORTB & TinyWire::interfaceMask.sclMask == 0);
    }
}

void slaveSendAck() {
    while(PORTB & TinyWire::interfaceMask.sclMask != 0);
    DDRB |= TinyWire::interfaceMask.sdaMask;
    PORTB &= ~TinyWire::interfaceMask.sdaMask;
    DDRB &= ~TinyWire::interfaceMask.sdaMask;
    while(PORTB & TinyWire::interfaceMask.sclMask == 0);
}

// Check that the master is talking to self
void slaveWaitMasterTalkSelf(char addr) {
    while(1) {
        // Wait for start signal
        DDRB &= ~TinyWire::interfaceMask.sdaMask;
        while(1) {
            if(PINB & TinyWire::interfaceMask.sclMask != 0 && PINB & TinyWire::interfaceMask.sdaMask == 0) {
                while(PINB & TinyWire::interfaceMask.sclMask != 0) {
                    if(PINB & TinyWire::interfaceMask.sdaMask != 0) {
                        break;
                    }
                }
                
                if(PINB & TinyWire::interfaceMask.sdaMask == 0) {
                    break;
                }
            }
        }

        slaveRetrieveData(); // Will be address
        if(readByte == addr) {
            break;
        }
    }
}

void masterPulseClock() {
    PORTB |= TinyWire::interfaceMask.sclMask;
    PORTB &= ~TinyWire::interfaceMask.sclMask;
}

void masterWriteAddr(char addr, char readNWrite) {
    DDRB |= TinyWire::interfaceMask.sdaMask;
    
    PORTB &= ~TinyWire::interfaceMask.sdaMask;
    PORTB &= ~TinyWire::interfaceMask.sclMask;
    
    for(i = 6; i >= 0; i--) {
        PORTB = ((addr >> i) & 0x01) ?
            (PORTB | TinyWire::interfaceMask.sdaMask) :
            (PORTB & TinyWire::interfaceMask.sdaMask);
        masterPulseClock();
    }
    
    // Write 'read/not write' signal
    PORTB &= ~TinyWire::interfaceMask.sdaMask;
    masterPulseClock();
    
    masterWaitForAck();
}
            
void masterWaitForAck() {
    DDRB &= ~TinyWire::interfaceMask.sdaMask;
    while((PINB & TinyWire::interfaceMask.sdaMask) != 0);
    masterPulseClock();
    DDRB |= TinyWire::interfaceMask.sdaMask;
}

// Send stop signal
void masterStop() {
    DDRB |= TinyWire::interfaceMask.sdaMask;
    PORTB &= ~TinyWire::interfaceMask.sdaMask;
    PORTB |= TinyWire::interfaceMask.sclMask;
}
