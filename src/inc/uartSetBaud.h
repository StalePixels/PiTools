//
// Created by D. Rimron-Soutter on 13/06/2021.
//

#ifndef PITOOLS_LIB_UARTSETBAUD_H
#define PITOOLS_LIB_UARTSETBAUD_H

#include <arch/zxn.h>
#include <stdbool.h>

extern const unsigned long uart_clock[];

void uartSetBaud(unsigned long speed);

#endif //PITOOLS_LIB_UARTSETBAUD_H
