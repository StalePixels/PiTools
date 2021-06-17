//
// Created by D. Rimron-Soutter on 13/06/2021.
//


#include <stdint.h>
#include <stdio.h>

#include "uartWaitStr.h"
#include "uartGetChr.h"

void uartWaitStr(const char* cmd) {
    uint16_t cmd_char = 0;

    getchar:
    uartBufferChar = uartGetChr();

    if(uartBufferChar==13) goto getchar;

    if (uartBufferChar == cmd[cmd_char]) {
        cmd_char++;
    } else {
        cmd_char = 0;
    }
printf("%c", uartBufferChar);
    if(cmd[cmd_char]==0) return;

    goto getchar;

}
