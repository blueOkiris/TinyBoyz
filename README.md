# TinyBoyz

A gameboy-shaped Arduino-based game console made out of parallel processed ATtiny85s.

## Hardware Architecture:

```
                                                       --------------------------
   -----------                   -------------         |                        |
   |  Memory |              -----| High Disp |--D[7:5]-| D[7:5]                 |
   -----------              |    |   Tiny    |<-Clk    |                        |
        |                   |    -------------         |                        |
 /-MISO-| 4-bit SPI         |    -------------         |                        |
|       |                   |----| Mid  Disp |--D[4:2]-| D[4:2]                 |
|  -----------    -------  /     |           |<-Clk    |          Ili9341       |
|  | Tiny SD |----| DAC |--      -------------         |          Display       |
|  -----------    -------  \     -------------         |                        |
|                   | |     |----|  Low/Ctl  |--D[1:0]-| D[1:0]                 |
|                   | |          |           |--Clk----| WR                     |
|              -------------     |           |--C/D----| RS                     |
 \-------------| I/O  Tiny |     -------------    Gnd->| CS                     |
               -------------                           --------------------------
                    | |
          Vcc       | |
           |   -------------
           |   | Main Tiny |
           Z   -------------
           Z    |   ||
           |    |   ||
    /--o \-------   ||
   |                ||
  Gnd            ---------
                 |  Joy  |
                 | stick |
                 ---------
```

Basically, I use three tinies to get a "fast" parallel interface to a display (more about that in docs/DISPLAY.md). Then I use a tiny to handle SPI to an SD Card and speed up memory access. It will hold a chunk of instructions, pass that in and get the next chunk, then wait for another receive. Then I have another tiny which tells the IO ones what to do, and receives data from the SD card/FRAM (more about that in docs/MEMORY.md). And I have one central tiny to handle input, handle interpretation, and control the IO tiny.
