#ifndef _TINY_WIRE_HPP_
#define _TINY_WIRE_HPP_

namespace TinyWire {
    class MasterInterface {
        private:
            char _sdaMask, _sclMask;
            char _readByte, i;
            
            void _pulseClock();
            void _waitForAcknowledge();            
            void _writeAddr(char addr, char readNWrite);
            void _stop();
            
        public:
            MasterInterface(char sdaPin, char sclPin);
            char read(char addr);
            void write(char addr, char data);
    };
}

#endif