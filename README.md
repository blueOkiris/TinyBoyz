# TinyBoyz

A gameboy-shaped Arduino-based game console made out of parallel processed ATtiny85s.

## Hardware Architecture:

```
                                                     --------------------------
 -----------                   -------------         |                        |
 | Sd Card |              -----| High Disp |--D[7:5]-| D[7:5]                 |
 -----------              |    |   Tiny    |<-Clk    |                        |
      |                   |    -------------         |                        |
      | 4-bit SPI         |    -------------         |                        |
      |                   |----| Mid  Disp |--D[4:2]-| D[4:2]                 |
 -----------    -------  /     |           |<-Clk    |          Ili9341       |
 | Tiny SD |----| DAC |--      -------------         |          Display       |
 -----------    -------  \     -------------         |                        |
                  | |     |----|  Low/Ctl  |--D[1:0]-| D[1:0]                 |
                  | |          |           |--Clk----| WR                     |
             -------------     |           |--C/D----| RS                     |
             | Main Tiny |     -------------    Gnd->| CS                     |
             -------------                           --------------------------
```

Basically, I use three tinies to get a "fast" parallel interface to a display (more about that in docs/DISPLAY.md). Then I use a tiny to handle SPI to an SD Card and speed up memory access. It will hold a chunk of instructions, pass that in and get the next chunk, then wait for another receive. And then one central tiny to control interpretation and telling the other ones what to do.
