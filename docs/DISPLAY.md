# Display Information

## Original Design

So originally, I just used 4-bit SPI (no MISO and a C/D pin).

This worked. And with internal 16MHz clock and direct port manipulation, I could get small shapes to be relatively fast, but filling the screen was like 4-5s. That's just *WAY* to slow. But it works, so there's that.

## Speed Issues

SPI is too slow w/ bit-bashing on the tiny. It doesn't have a true spi bus either.

What's the solution to get game-ready graphics? It doesn't have to be that fast. Noticible flicker is fine, but I can't wait 4 seconds for the display to fill with black!

I need to use parallel, but how with only 5 I/O (really 4 since I need an analog input for communication).

It's simple. I use three tinies.

## Redesign

Alright so first of all, all the tinies can use the same data line. If I send 'R,' and that means draw a rectangle, the SD tiny can just ignore it, while the three displays can all react, and vice versa. This frees up 3 pins on the main tiny, which I can use for fast input.

Then the three tinies can be split into three programs. The first bit-bashes the upper three bits on a received clock pulse (4 - pins). The middle does the same. The bottom bit-bashes the last 2, and sends the clock signal and the C/D signal. All of them take the same commands and do the same thing, just with different parts of the display.

The SD card will still use SPI of course. So you get something like:

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

So yeah. That's how that will work.
