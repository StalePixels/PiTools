//
// Created by D. Rimron-Soutter on 24/06/2021.
//

#include <stdint.h>
#include <stdio.h>
#include "nbnGetHeader.h"

#include "uartGetChr.h"

void nbnGetHeader(uint32_t *nbn_size, uint32_t *nbn_blocks, uint16_t *nbn_remainder) {
    *nbn_size = 0;
    *nbn_blocks = 0;
    *nbn_remainder = 0;

    // get Size
    *nbn_size = uartGetChr();
    *nbn_size = (*nbn_size << 8) + uartGetChr();
    *nbn_size = (*nbn_size << 8) + uartGetChr();
    *nbn_size = (*nbn_size << 8) + uartGetChr();

    if(*nbn_size == 0) return;

    // Send blocks
    *nbn_blocks = uartGetChr();
    *nbn_blocks = (*nbn_blocks << 8) + uartGetChr();
    *nbn_blocks = (*nbn_blocks << 8) + uartGetChr();
    *nbn_blocks = (*nbn_blocks << 8) + uartGetChr();

    // Send remainder
    *nbn_remainder = uartGetChr();
    *nbn_remainder = (*nbn_remainder << 8) + uartGetChr();
}
