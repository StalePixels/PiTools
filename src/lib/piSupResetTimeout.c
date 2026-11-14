//
// Created by D. Rimron-Soutter on 13/11/2026.
//

#include <stdint.h>
#include <stdbool.h>
#include <arch/zxn.h>

#include "piSupResetTimeout.h"
#include "uartDrain.h"
#include "uartSendChr.h"
#include "uartWaitStrTimeout.h"
#include "uart.h"

// uartDrain waits for 100 frames, longer while bytes keep coming.
#define DRAIN_FRAMES 100

bool piSupResetTimeout(uint16_t frames) {
    goto drain;

    get_again:
    uartSendChr('3');

    drain:
    if(frames < DRAIN_FRAMES) return false;
    frames -= DRAIN_FRAMES;
    if(!uartDrain()) {
        uartSendChr(4);         // send a Control D - to kill, and retry
        if(frames < DRAIN_FRAMES) return false;
        frames -= DRAIN_FRAMES;
        if(!uartDrain()) {
            goto get_again;
        }
    }

    uartSendChr('\n');         // Send ENTER, wait for SUP prompt
    return uartWaitStrTimeout("SUP>", frames);
}
