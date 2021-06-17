//
// Created by D. Rimron-Soutter on 13/06/2021.
//

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <arch/zxn.h>

#include "uart.h"
#include "uartSendStr.h"
#include "uartWaitStr.h"
#include "uartDrain.h"
#include "zxn.h"


static unsigned char    uartBuff[256];

void uartSendChr(const char c) {
    zx_border(4);
    while (IO_UART_STATUS & IUS_TX_BUSY);  // Busy wait to send a single byte.
//    printf(">%d",c);
    IO_UART_TX = c;
    zx_border(0);
}


unsigned char uartGetChr() {
    forever:
    if (IO_UART_STATUS & IUS_RX_AVAIL) return IO_UART_RX;
    goto forever;
}


void uartGetStr(char* responseBuffer) {
    uint16_t res_len = 0;

    getchar:
    uartBufferChar = uartGetChr();

    if(uartBufferChar==10) return;

    responseBuffer[res_len++] = uartBufferChar;
    responseBuffer[res_len] = 0;
    goto getchar;
}