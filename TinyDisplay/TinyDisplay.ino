#define PIN_MOSI            1
#define PIN_SCL             2
#define PIN_CS              3
#define PIN_CD              0

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

//#define digitalWrite        delay(1); digitalWrite

void setup() {
    dispInit();
}

void loop() {

}

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
    ILI_MEM_ACC_CTL, 1, 0x48,               // Memory Access Control
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

void dispInit() {
    spiInit();

    spiSendCommand(ILI_RST);
    delay(150);

    for(uint8_t ind = 0; ind < 110; ind++) {
        uint8_t cmd = iliInitCmds[ind++];
        uint8_t numData = iliInitCmds[ind++];
        spiSendCommand(cmd, iliInitCmds + ind, numData & 0x7F);
        if(numData & 0x80) {
            delay(150);
        }
        ind += (numData & 0x7F) - 1;
    }
}

void spiInit() {
    pinMode(PIN_MOSI, OUTPUT);
    pinMode(PIN_SCL, OUTPUT);
    pinMode(PIN_CS, OUTPUT);
    pinMode(PIN_CD, OUTPUT);

    digitalWrite(PIN_CS, HIGH);
    digitalWrite(PIN_SCL, LOW);
}

void spiSendCommand(uint8_t cmd, uint8_t *data, uint8_t len) {
    digitalWrite(PIN_CS, LOW);

    digitalWrite(PIN_CD, LOW);
    spiWrite(cmd);

    digitalWrite(PIN_CD, HIGH);
    for(uint8_t i = 0; i < len; i++) {
        spiWrite(data[i]);
    }

    digitalWrite(PIN_CS, HIGH);
}

void spiSendCommand(uint8_t cmd) {
    digitalWrite(PIN_CS, LOW);
    digitalWrite(PIN_CD, LOW);
    spiWrite(cmd);
    digitalWrite(PIN_CS, HIGH);
}

void spiWrite(uint8_t byte) {
    for(int8_t bit = 7; bit >= 0; bit--) {
        digitalWrite(PIN_MOSI, (byte >> bit) & 0x01);
        digitalWrite(PIN_SCL, HIGH);
        digitalWrite(PIN_SCL, LOW);
    }
}