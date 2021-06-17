//
// Created by D. Rimron-Soutter on 13/06/2021.
//

#ifndef PITOOLS_LIB_UARTSENDCHR_H
#define PITOOLS_LIB_UARTSENDCHR_H

#include <arch/zxn.h>

#define uartSendChr_FASTMACRO(c)      while(IO_UART_STATUS & IUS_TX_BUSY); IO_UART_TX = c;

void uartSendChr(const char c);

#endif //PITOOLS_LIB_UARTSENDCHR_H
