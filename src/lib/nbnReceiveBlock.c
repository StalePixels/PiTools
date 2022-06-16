//
// Created by D. Rimron-Soutter on 24/06/2021.
//


#include <stdint.h>
#include <stdio.h>
#include <arch/zxn.h>
#include <uartGetChr.h>
#include "nbnReceiveBlock.h"
#include "nbn.h"

extern uint8_t btm_page,top_page;


uint8_t nbnReceiveBlock(uint16_t size, unsigned char *data)  {
    uint8_t nbn_status;
    uint8_t nbn_checksum = 0;
    uint8_t nbn_computed = 0;

    nbn_status = NBN_BLOCK_FAIL;

    for(uint16_t nbn_byte = 0; nbn_byte < size; nbn_byte++) {
        while (!(IO_UART_STATUS & IUS_RX_AVAIL));
        data[nbn_byte] = IO_UART_RX;

        nbn_computed = nbn_computed + data[nbn_byte];
        zx_border(data[nbn_byte]);
    }
    nbn_checksum =  uartGetChr();

    if(nbn_checksum == nbn_computed) {
        nbn_status = NBN_BLOCK_SUCCESS;
    }

    return nbn_status;
}
