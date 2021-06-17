//
// Created by D. Rimron-Soutter on 13/06/2021.
//

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <arch/zxn.h>

#include "uartDrain.h"
#include "uartSendStr.h"
#include "uartWaitStr.h"

void uartSendCmd(const char* cmd) {
    uartDrain();
    uartSendStr(cmd);
    uartWaitStr(cmd);
}