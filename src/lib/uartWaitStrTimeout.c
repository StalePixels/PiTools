//
// Created by D. Rimron-Soutter on 13/11/2026.
//

#include <stdint.h>
#include <stdbool.h>
#include <arch/zxn.h>

#include "uartWaitStrTimeout.h"
#include "uart.h"
#include "zxn.h"

bool uartWaitStrTimeout(const char* str, uint16_t frames) {
    uint16_t str_char = 0;
    uint8_t  line_high = 0;
    uint8_t  c;

    while(frames) {
        if (IO_UART_STATUS & IUS_RX_AVAIL) {
            c = IO_UART_RX;
            if(c==13) continue;

            if (c == str[str_char]) {
                str_char++;
            } else {
                str_char = (c == str[0]) ? 1 : 0;
            }
            if(str[str_char]==0) return true;
        }

        // A frame ends when the active video line wraps: bit 8 of the line
        // (NextReg 0x1E bit 0) goes from 1 back to 0.
        if (ZXN_READ_REG(REG_ACTIVE_VIDEO_LINE_H) & 1) {
            line_high = 1;
        } else if (line_high) {
            line_high = 0;
            --frames;
        }
    }
    return false;
}
