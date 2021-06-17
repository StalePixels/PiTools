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
#include "uart.h"

void piSupReset() {
    checkFlag = 10;
    uartSendChr(3);         // send a Control C - to kill anything that may be running on the SUP
    uartDrain();
    uartSendStr("reset\n");
    checkFlag = 0;
    get_again:
    uartDrain();
    if(!checkFlag) {
        printf("Resetting... ");
        checkFlag = 255;
        uartSendChr(4);         // send a Control D - to reset the uart
        goto get_again;
    } else if(checkFlag==255) {
        printf("ERROR draining UART\n");
        exit(1);
    }
}
