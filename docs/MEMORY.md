# Memory Redesign

## Problem with Memory

Whether I end up writing a whol Fat driver for an SD card, or switching to FRAM, there's one big issue with the current design: I can't read data *back* to the Main tiny.

## Redesign 3

I can get around this by adding an Inter-Device communication Tiny. It talks directly over some sort of TWI to the main, and then sends the DAC signal as well as takes a copy of the MISO line back from the Memory, so it can send memory data back to the main tiny.

Here's the redone design:

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

## Sd Card vs FRAM

I've got two options and two tradeoffs. Sd Cards make it easy to add programs from my computer, *but* they are really complicated with using a filesystem or even their Spi mode.

However, FRAM is nice and easy to read and write to, yet I can't interface with my computer directly. I'd have to add a port to the device that let's another microcontroller transfer files from a computer, which is kind of lame.
