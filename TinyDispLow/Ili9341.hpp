#ifndef _ILI9341_HPP_
#define _ILI9341_HPP_

#include <Arduino.h>

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

static const uint8_t iliInitCmds_g[110] = {
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

#endif