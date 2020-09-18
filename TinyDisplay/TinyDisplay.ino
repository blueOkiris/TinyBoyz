#define PIN_CD              0
#define PIN_MOSI            1
#define PIN_SCL             2
#define PIN_CS              3

#define CMD_SLPOUT          0x11
#define CMD_DISPON          0x29
#define CMD_COL_ADDR_SET    0x2A
#define CMD_ROW_ADDR_SET    0x2B
#define CMD_RAMWR           0x2C
#define CMD_RST             0x01
#define CMD_MEM_ACC_CTL     0x36

#define MEM_CTL_REV         0x20
#define MEM_CTL_BGR         0x08

#define BLUE                0x001F
#define BLACK               0x0000
#define GREEN               0x07E0

uint8_t maskCommData, maskMosi, maskScl, maskCs;
uint8_t i;
const uint16_t screenWidth = 320, screenHeight = 240;

void setup() {
    displayInit();
    displayFillRect(49, 49, 50, 50, GREEN);
}

void loop() {
    
}

void displayInit() {
    spiInit(PIN_CD, PIN_MOSI, PIN_SCL, PIN_CS);

    PORTB &= ~maskCommData;
    spiFlashByte(CMD_RST);
    delay(120);
    spiFlashByte(CMD_SLPOUT);
    delay(120);
    spiFlashByte(CMD_DISPON);

    spiFlashByte(CMD_MEM_ACC_CTL);
    PORTB |= maskCommData;
    spiFlashByte(MEM_CTL_REV | MEM_CTL_BGR);

    //displayFillRect(0, 0, screenWidth, screenHeight, BLACK);
}

void displaySetAddr(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    PORTB &= ~maskCommData;
    spiFlashByte(CMD_COL_ADDR_SET);
    PORTB |= maskCommData;
    spiFlashByte(x0 >> 8);
    spiFlashByte(x0 & 0x00FF);
    spiFlashByte(x1 >> 8);
    spiFlashByte(x1 & 0x00FF);

    PORTB &= ~maskCommData;
    spiFlashByte(CMD_ROW_ADDR_SET);
    PORTB |= maskCommData;
    spiFlashByte(y0 >> 8);
    spiFlashByte(y0 & 0x00FF);
    spiFlashByte(y1 >> 8);
    spiFlashByte(y1 & 0x00FF);
}

void displayFillRect(
        uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    displaySetAddr(x, y, x + w - 1, y + h - 1);

    PORTB &= ~maskCommData;
    spiFlashByte(CMD_RAMWR);
    PORTB |= maskCommData;
    for(y = h; y >= 0; y--) {
        for(x = w; x >= 0; x++) {
            spiFlashByte(color >> 8);
            spiFlashByte(color & 0x00FF);
        }
    }
    PORTB |= maskCs;
}

void spiInit(uint8_t commDataPin, uint8_t mosiPin, uint8_t sclPin, uint8_t csPin) {
    maskCommData = 1 << commDataPin;
    maskMosi = 1 << mosiPin;
    maskScl = 1 << sclPin;
    maskCs = 1 << csPin;

    DDRB |= maskCommData;
    DDRB |= maskMosi;
    DDRB |= maskScl;
    DDRB |= maskCs;

    PORTB |= maskCs;
    PORTB &= ~maskScl;
    PORTB &= ~maskCommData;
}

void spiFlashByte(uint8_t data) {
    PORTB &= ~maskCs;
    for(i = 0; i < 8; i++) {
        PORTB = ((data >> 7 - i) & 0x01) ?
            (PORTB | maskMosi) :
            (PORTB & ~maskMosi);
        spiPulseClock();
    }
    PORTB |= maskCs;
}

void spiPulseClock() {
    PORTB |= maskScl;
    PORTB &= ~maskScl;
}
