//
// Created by D. Rimron-Soutter on 13/06/2021.
//

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <arch/zxn.h>

#include "uartDump.h"
#include "uart.h"
#include "zxn.h"


bool uartDump() {
    bool          checkFlag = false;
    for(uint8_t checking=255;checking;--checking) {
        MACRO__WAIT_FOR_SCANLINE(1); //waste some time, in case there's LOTS of output, let it flood the buffer if required!
        get_more:
        if (IO_UART_STATUS & IUS_RX_AVAIL) {  // Busy wait && get a single byte.
            zx_border((uint8_t )checking%8);
            checkFlag = true;
//            IO_UART_RX;  // just take a character, and throw it away
            printf("%c",IO_UART_RX);  // just take a character, and throw it away
            goto get_more;
        }
    }
    return checkFlag;
}
