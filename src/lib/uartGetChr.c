//
// Created by D. Rimron-Soutter on 13/06/2021.
//

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <arch/zxn.h>
#include <stdlib.h>

#include "uartGetChr.h"
#include "zxn.h"

unsigned char           uartBufferChar;

unsigned char uartGetChr() {
    uint32_t timeout = 65535<<1;

    repeat:

    if (IO_UART_STATUS & IUS_RX_AVAIL) {
        return IO_UART_RX;
    }

    if(--timeout == 0) {
//        printf("\nTIMED OUT WAITING FOR PI\n");
        exit(20);
    }

    goto repeat;
}