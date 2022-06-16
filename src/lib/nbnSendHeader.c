//
// Created by D. Rimron-Soutter on 17/06/2021.
//
#include <stdint.h>
#include <arch/zxn.h>
#include "uartSendChr.h"
#include "uartSendStr.h"
#include "nbn.h"
#include "nbnSendHeader.h"

void nbnSendHeader(uint32_t filesize, const char *name) {
    // re
    uint32_t blockcount = filesize / NBN_MAX_BLOCKSIZE;
    uint16_t finalblock = filesize % NBN_MAX_BLOCKSIZE;

    // Send size
    uartSendChr_FASTMACRO((filesize >> 24) & 255);
    uartSendChr_FASTMACRO((filesize >> 16) & 255);
    uartSendChr_FASTMACRO((filesize >> 8 ) & 255);
    uartSendChr_FASTMACRO((filesize      ) & 255);

    // Send blocks
    uartSendChr_FASTMACRO((blockcount >> 24) & 255);
    uartSendChr_FASTMACRO((blockcount >> 16) & 255);
    uartSendChr_FASTMACRO((blockcount >> 8 ) & 255);
    uartSendChr_FASTMACRO((blockcount      ) & 255);

    // Send remainder
    uartSendChr_FASTMACRO((finalblock >> 8 ) & 255);
    uartSendChr_FASTMACRO((finalblock      ) & 255);

    // Filename
    uartSendStr(name);

    // Terminator byte
    uartSendChr_FASTMACRO(0);
}
