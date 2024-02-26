//
// Created by D. Rimron-Soutter on 13/06/2021.
//

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <arch/zxn.h>

#include "uart.h"
#include "uartSendStr.h"
#include "zxn.h"

uint16_t uart_idx;

void uartSendStr(const char* str) {
    zx_border(4);
    uart_idx = 0;
    while(str[uart_idx]) {
        while (IO_UART_STATUS & IUS_TX_BUSY);  // Busy wait to send a single byte.
        IO_UART_TX = str[uart_idx++];
    }
    zx_border(0);
}
