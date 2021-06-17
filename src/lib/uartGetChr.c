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

unsigned char           uartBufferChar;

unsigned char uartGetChr() {
    forever:
    if (IO_UART_STATUS & IUS_RX_AVAIL) return IO_UART_RX;
    goto forever;
}