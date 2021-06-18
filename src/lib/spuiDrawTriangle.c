//
// Created by D. Rimron-Soutter on 18/06/2021.
//

#include <stdint.h>
#include <stdio.h>
#include "ula.h"

void spuiDrawTriangle(uint8_t x, uint8_t y) {
    x = --x<<3; y = --y<<3;
    uint8_t x7 = x+7;
    uint8_t x6 = x+6;
    uint8_t x5 = x+5;
    uint8_t x4 = x+4;
    uint8_t x3 = x+3;
    uint8_t x2 = x+2;
    uint8_t x1 = x+1;

    plot(x7, y);
    y++;
    plot(x7, y); plot(x6, y);
    y++;
    plot(x7, y); plot(x6, y); plot(x5, y);
    y++;
    plot(x7, y); plot(x6, y); plot(x5, y); plot(x4, y);
    y++;
    plot(x7, y); plot(x6, y); plot(x5, y); plot(x4, y); plot(x3, y);
    y++;
    plot(x7, y); plot(x6, y); plot(x5, y); plot(x4, y); plot(x3, y); plot(x2, y);
    y++;
    plot(x7, y); plot(x6, y); plot(x5, y); plot(x4, y); plot(x3, y); plot(x2, y); plot(x1, y);
    y++;
    plot(x7, y); plot(x6, y); plot(x5, y); plot(x4, y); plot(x3, y); plot(x2, y); plot(x1, y); plot(x, y);
    y++;
}