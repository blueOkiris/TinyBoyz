#ifndef _CPU_HPP_
#define _CPU_HPP_

#include "TransmissionLine.hpp"

class Cpu {
    private:
        int _clockPin = 3;
        TransmissionLine _display;
        TransmissionLine _memFram;
        TransmissionLine _progMemSd;
        TransmissionLine _input;
        
    public:
        Cpu(int clockPin, int dispPin, int framPin, int progMemPin, int inpPin,
                int dispAddr, int framAddr, int progMemAddr, int inpAddr)
                : _clockPin(clockPin), _display(clockPin, dispPin, dispAddr),
                _memFram(clockPin, framPin, framAddr),
                _progMemSd(clockPin, progMemPin, progMemAddr),
                _input(clockPin, inpPin, inpAddr) {
        }
};

#endif