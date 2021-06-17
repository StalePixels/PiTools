//
// Created by D. Rimron-Soutter on 17/06/2021.
//
#include <stdint.h>
#include <arch/zxn.h>
#include "uartSendChr.h"
#include "uartSendStr.h"

void nbnSendHeader(uint32_t filesize, uint16_t blocksize, const char *name) {
    uint32_t blockcount = filesize / blocksize;
    uint16_t finalblock = filesize % blocksize;

    // Send size
//    uartByte = ((filesize) >> 24) & 255;    uartSendChr_FASTMACRO(uartByte);
    uartSendChr_FASTMACRO((filesize >> 24) & 255);
//    uartByte = ((filesize) >> 16) & 255;    uartSendChr_FASTMACRO(uartByte);
    uartSendChr_FASTMACRO((filesize >> 16) & 255);
//    uartByte = ((filesize) >> 8 ) & 255;    uartSendChr_FASTMACRO(uartByte);
    uartSendChr_FASTMACRO((filesize >> 8 ) & 255);
//    uartByte = ((filesize)      ) & 255;    uartSendChr_FASTMACRO(uartByte);
    uartSendChr_FASTMACRO((filesize      ) & 255);

    // Send blocks
//    uartByte = (blockcount >> 24) & 255;    uartSendChr_FASTMACRO(uartByte);
    uartSendChr_FASTMACRO((blockcount >> 24) & 255);
//    uartByte = (blockcount >> 16) & 255;    uartSendChr_FASTMACRO(uartByte);
    uartSendChr_FASTMACRO((blockcount >> 16) & 255);
//    uartByte = (blockcount >> 8 ) & 255;    uartSendChr_FASTMACRO(uartByte);
    uartSendChr_FASTMACRO((blockcount >> 8 ) & 255);
//    uartByte =  blockcount        & 255;    uartSendChr_FASTMACRO(uartByte);
    uartSendChr_FASTMACRO((blockcount      ) & 255);

    // Send remainder
//    uartByte = (finalblock >> 8 ) & 255;    uartSendChr_FASTMACRO(uartByte);
    uartSendChr_FASTMACRO((finalblock >> 8 ) & 255);
//    uartByte =  finalblock        & 255;    uartSendChr_FASTMACRO(uartByte);
    uartSendChr_FASTMACRO((finalblock      ) & 255);

    // Filename
    uartSendStr(name);

    // Terminator byte
    uartSendChr_FASTMACRO(0);
}
