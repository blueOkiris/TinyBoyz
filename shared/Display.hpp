#ifndef _DISPLAY_HPP_
#define _DISPLAY_HPP_

namespace Display {
    enum class TinyDispType {
        HighBits,
        MidBits,
        LowAndCtlBits
    };
    
    void init(TinyDispType tinyDispId);
    void setAddrWindow(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h);
    void fillRect(
        uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color
    );
}

#endif
