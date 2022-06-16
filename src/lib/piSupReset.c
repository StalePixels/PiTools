//
// Created by D. Rimron-Soutter on 14/06/2021.
//

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <arch/zxn.h>

#include "piSupReset.h"
#include "uartDrain.h"
#include "uartSendStr.h"
#include "uartSendChr.h"
#include "uartWaitStr.h"
#include "uart.h"

void piSupReset(bool verbose) {
    uint8_t reset = 0;
    uartSendStr("\nreset\n");
    goto drain;

    get_again:
    uartSendChr('3');         // send a Control D - to reset the supervisor
    uartSendStr("\nreset\n");

    drain:
    if(!uartDrain()) {
        reset++;
        if(verbose==true) printf("\nResetting SUPervisor... \n");
        uartSendChr(4);         // send a Control D - to kill, and retry
        if(!uartDrain()) {
            goto get_again;
        }
    } else if(reset==2) {
        if(verbose) printf("\nERROR draining UART\n");
        exit(1);
    }

    uartSendChr('\n');         // Send ENTER, wait for SUP prompt
    uartWaitStr("SUP>");
}
