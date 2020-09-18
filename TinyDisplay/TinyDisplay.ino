#define PIN_MOSI            1
#define PIN_SCL             0
#define PIN_CS              3
#define PIN_CD              2

#define ILI_RST             0x01
#define ILI_PW_CTL_1        0xC0
#define ILI_PW_CTL_2        0xC1
#define ILI_VM_CTL_1        0xC5
#define ILI_VM_CTL_2        0xC7
#define ILI_MEM_ACC_CTL     0x36
#define ILI_V_SC_ZERO       0x37
#define ILI_PIX_FRMT        0x3A
#define ILI_FR_CTL_1        0xB1
#define ILI_D_FUN_CTL       0xB6
#define ILI_GAMMA_SET       0x26
#define ILI_GM_CTL_POS_1    0xE0
#define ILI_GM_CTL_NEG_1    0xE1
#define ILI_SLP_OUT         0x11
#define ILI_DISP_ON         0x29
#define ILI_COL_ADDR_SET    0x2A
#define ILI_PG_ADDR_SET     0x2B
#define ILI_RAM_WRITE       0x2C

#define BLACK       0x0000      //   0,   0,   0
#define NAVY        0x000F      //   0,   0, 123
#define DARKGREEN   0x03E0      //   0, 125,   0
#define DARKCYAN    0x03EF      //   0, 125, 123
#define MAROON      0x7800      // 123,   0,   0
#define PURPLE      0x780F      // 123,   0, 123
#define OLIVE       0x7BE0      // 123, 125,   0
#define LIGHTGREY   0xC618      // 198, 195, 198
#define DARKGREY    0x7BEF      // 123, 125, 123
#define BLUE        0x001F      //   0,   0, 255
#define GREEN       0x07E0      //   0, 255,   0
#define CYAN        0x07FF      //   0, 255, 255
#define RED         0xF800      // 255,   0,   0
#define MAGENTA     0xF81F      // 255,   0, 255
#define YELLOW      0xFFE0      // 255, 255,   0
#define WHITE       0xFFFF      // 255, 255, 255
#define ORANGE      0xFD20      // 255, 165,   0
#define GREENYELLOW 0xAFE5      // 173, 255,  41
#define PINK        0xFC18      // 255, 130, 198

//#define digitalWrite        delay(1); digitalWrite

const uint8_t iliInitCmds[110] = {
    0xEF, 3, 0x03, 0x80, 0x02,
    0xCF, 3, 0x00, 0xC1, 0x30,
    0xED, 4, 0x64, 0x03, 0x12, 0x81,
    0xE8, 3, 0x85, 0x00, 0x78,
    0xCB, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,
    0xF7, 1, 0x20,
    0xEA, 2, 0x00, 0x00,
    ILI_PW_CTL_1, 1, 0x23,                  // Power control VRH[5:0]
    ILI_PW_CTL_2, 1, 0x10,                  // Power control SAP[2:0];BT[3:0]
    ILI_VM_CTL_1, 2, 0x3e, 0x28,            // VCM control
    ILI_VM_CTL_2, 1, 0x86,                  // VCM control2
    ILI_MEM_ACC_CTL, 1, 0x20 | 0x08,        // Memory Access Control
    ILI_V_SC_ZERO, 1, 0x00,                 // Vertical scroll zero
    ILI_PIX_FRMT, 1, 0x55,
    ILI_FR_CTL_1, 2, 0x00, 0x18,
    ILI_D_FUN_CTL, 3, 0x08, 0x82, 0x27,     // Display Function Control
    0xF2, 1, 0x00,                          // 3Gamma Function Disable
    ILI_GAMMA_SET , 1, 0x01,                // Gamma curve selected
    ILI_GM_CTL_POS_1,                       // Set Gamma Positive
        15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10,
        0x03, 0x0E, 0x09, 0x00,
    ILI_GM_CTL_NEG_1,                       // Set Gamma Negative
        15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F,
        0x0C, 0x31, 0x36, 0x0F,
    ILI_SLP_OUT, 0x80,                      // Exit Sleep
    ILI_DISP_ON, 0x80,                      // Display on
};

uint8_t maskMosi, maskScl, maskCd, maskCs;

void setup() {
    dispInit();
    dispFillRect(0, 0, 320, 240, BLACK);
    dispFillRect(50, 50, 50, 50, BLUE);
}

void loop() {

}

void dispInit() {
    spiInit();

    spiSendCommand(ILI_RST);
    delay(150);

    for(uint8_t ind = 0; ind < 110; ind++) {
        uint8_t cmd = iliInitCmds[ind++];
        uint8_t numData = iliInitCmds[ind++];
        spiSendCommand(cmd, iliInitCmds + ind, numData & 0x7F);
        if(numData & 0x80) {
            delay(100);
        }
        ind += (numData & 0x7F) - 1;
    }
}

void dispFillRect(
        uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    dispSetAddrWindow(x, y, w, h);
    spiFill16(color, (uint32_t) w * h);
}

void spiFill16(uint16_t data, uint32_t len) {
    PORTB &= ~maskCs;
    PORTB |= maskCd;
    for(uint32_t i = 0; i < len; i++) {
        spiWrite(data >> 8);
        spiWrite(data & 0x00FF);
    }
    spiWrite(0x00);
    PORTB |= maskCs;
}

void spiFill8(uint8_t data, uint32_t len) {
    PORTB &= ~maskCs;
    PORTB |= maskCd;
    for(uint32_t i = 0; i < len; i++) {
        spiWrite(data);
    }
    spiWrite(0x00);
    PORTB |= maskCs;
}

void dispSetAddrWindow(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h) {
    uint16_t x2 = (x1 + w - 1), y2 = (y1 + h - 1);
    uint8_t dataX[4] = {
        x1 >> 8, x1 & 0x00FF, x2 >> 8, x2 & 0x00FF
    };
    uint8_t dataY[4] = {
        y1 >> 8, y1 & 0x00FF, y2 >> 8, y2 & 0x00FF
    };
    spiSendCommand(ILI_COL_ADDR_SET, dataX, 4);
    spiSendCommand(ILI_PG_ADDR_SET, dataY, 4);
    spiSendCommand(ILI_RAM_WRITE);
}

void spiInit() {
    maskMosi = 0x01 << PIN_MOSI;
    maskScl = 0x01 << PIN_SCL;
    maskCs = 0x01 << PIN_CS;
    maskCd = 0x01 << PIN_CD;

    DDRB |= maskMosi;
    DDRB |= maskScl;
    DDRB |= maskCs;
    DDRB |= maskCd;

    PORTB |= maskCs;
    PORTB &= ~maskScl;
}

void spiSendData16(uint16_t data) {
    PORTB &= ~maskCs;
    PORTB |= maskCd;
    spiWrite(data >> 8);
    spiWrite(data & 0x00FF);
    PORTB |= maskCs;
}

void spiSendData8(uint8_t data) {
    PORTB &= ~maskCs;
    PORTB |= maskCd;
    spiWrite(data);
    PORTB |= maskCs;
}

void spiSendCommand(uint8_t cmd, uint8_t *data, uint8_t len) {
    PORTB &= ~maskCs;

    PORTB &= ~maskCd;
    spiWrite(cmd);

    PORTB |= maskCd;
    for(uint8_t i = 0; i < len; i++) {
        spiWrite(data[i]);
    }

    PORTB |= maskCs;
}

void spiSendCommand(uint8_t cmd) {
    PORTB &= ~maskCs;
    PORTB &= ~maskCd;
    spiWrite(cmd);
    PORTB |= maskCs;
}

void spiWrite(uint8_t byte) {
    for(int8_t bit = 7; bit >= 0; bit--) {
        PORTB = ((byte >> bit) & 0x01) ?
            (PORTB | maskMosi) :
            (PORTB & ~maskMosi);
        PORTB |= maskScl;
        PORTB &= ~maskScl;
    }
}
