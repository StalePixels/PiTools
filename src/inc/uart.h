//
// Created by D. Rimron-Soutter on 13/06/2021.
//

#ifndef PITOOLS_UART_H
#define PITOOLS_UART_H

#include <arch/zxn.h>
#include <stdbool.h>

extern uint16_t                  uart_idx;

void uartInit();

void uartGetStr(char* responseBuffer);

#endif //PITOOLS_UART_H
