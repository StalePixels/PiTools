//
// Created by D. Rimron-Soutter on 18/06/2021.
//
#include <arch/zxn.h>
#include <stdint.h>
#include "uartGetChr.h"

uint8_t nbnSendBlock(uint16_t size, const unsigned char *data) {
    uint8_t nbn_checksum = 0;
    for(uint16_t nbn_byte = 0; nbn_byte < size; nbn_byte++) {
        while (IO_UART_STATUS & IUS_TX_BUSY);  // Busy wait to send a single byte.
        IO_UART_TX = data[nbn_byte];

        nbn_checksum = nbn_checksum + data[nbn_byte];
        zx_border(data[nbn_byte]);
    }

    while (IO_UART_STATUS & IUS_TX_BUSY);  // Busy wait to send checksum
    IO_UART_TX = nbn_checksum;

    zx_border(0);

    return uartGetChr();
}