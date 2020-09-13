#ifndef _TINY_WIRE_HPP_
#define _TINY_WIRE_HPP_

namespace TinyWire {
    class Interface {
        protected:
            char _sdaMask, _sclMask;
            char _readByte, i;
        
        public:
            Interface(char sdaPin, char sclPin);
            virtual char read(char addr) = 0;
            virtual void write(char addr, char data) = 0;
    };
    
    class MasterInterface : Interface {
        private:
            void _pulseClock();
            void _waitForAcknowledge();            
            void _writeAddr(char addr, char readNWrite);
            void _stop();
            
        public:
            MasterInterface(char sdaPin, char sclPin);
            char read(char addr) override;
            void write(char addr, char data) override;
    };
    
    class SlaveInterface : Interface {
        private:
            void _waitForMaster();
            void _waitUntilMasterTalking(char addr);
            void _sendAcknowledge();
            void _retrieveData();
            
        public:
            SlaveInterface(char sdaPin, char sclPin);
            char read(char addr);
            void write(char addr, char data);
    };
}

#endif