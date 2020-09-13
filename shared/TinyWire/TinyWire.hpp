#ifndef _TINY_WIRE_HPP_
#define _TINY_WIRE_HPP_

namespace TinyWire {
    static struct {
        char sdaMask;
        char sclMask;
    } interfaceMask = { 0, 0 };

    namespace AsMaster {
        void initInterface();
        char read(char addr);
        void write(char addr, char data);
    }

    namespace AsSlave {
        void initInterface();
        char read(char addr);
        void write(char addr, char data);
    }
}

#endif