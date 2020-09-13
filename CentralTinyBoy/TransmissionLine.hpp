#ifndef _TRANSMISSION_LINE_HPP_
#define _TRANSMISSION_LINE_HPP_

#include <SoftWire.h>

class TransmissionLine {
    private:
        int _addr;
        SoftWire _interface;
    
    public:
        TransmissionLine(int clockPin, int dataPin, int addr)
                : _addr(addr), _interface(dataPin, clockPin) {
            _interface.setTimeout_ms(40);
            _interface.begin();
        }
        
        void writeByte(char data) {
            _interface.startWrite(_addr);
            _interface.write(data);
        }
        void readByte(char data) {
            _interface.startRead(_addr);
            _interface.write(data);
        }
};

#endif