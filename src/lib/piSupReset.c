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
    bool reset = false;
    get_again:
    uartSendChr(4);         // send a Control D - to reset the supervisor
    if(!uartDrain()) {
        reset = true;
        if(verbose) printf("\nResetting SUPervisor... \n");
        uartSendChr(3);         // send a Control C - to kill, and retry
        goto get_again;
    } else if(reset) {
        if(verbose) printf("\nERROR draining UART\n");
        exit(1);
    }

    uartWaitStr("SUP>");
}
