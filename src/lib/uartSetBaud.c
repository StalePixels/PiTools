//
// Created by D. Rimron-Soutter on 13/06/2021.
//

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <arch/zxn.h>

#include "uartSetBaud.h"

const unsigned long uart_clock[] = { CLK_28_0, CLK_28_1, CLK_28_2, CLK_28_3, CLK_28_4, CLK_28_5, CLK_28_6, CLK_28_7 };

void uartSetBaud(unsigned long speed) {
    unsigned long prescalar;

    // Work out our real speed, based on video timing
    IO_NEXTREG_REG = REG_VIDEO_TIMING;
    prescalar = uart_clock[IO_NEXTREG_DAT] / speed;

    // Set baud at 115,200
    IO_UART_BAUD_RATE = prescalar & 0x7f;                   // lower 7 bits
    IO_UART_BAUD_RATE = ((prescalar >> 7) & 0x7f) | 0x80;   // upper 7 bits
}