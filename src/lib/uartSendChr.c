//
// Created by D. Rimron-Soutter on 13/06/2021.
//

#include <arch/zxn.h>

#include "uartSendChr.h"

void uartSendChr(const char c) {
    zx_border(4);
    while (IO_UART_STATUS & IUS_TX_BUSY);  // Busy wait to send a single byte.
    IO_UART_TX = c;
    zx_border(0);
}