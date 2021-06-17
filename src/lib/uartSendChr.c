//
// Created by D. Rimron-Soutter on 13/06/2021.
//


#include <arch/zxn.h>

#include "uartSendChr.h"

void uartSendChr(const char c) {
    zx_border(4);
    uartSendChr_FASTMACRO(c);
    zx_border(0);
}