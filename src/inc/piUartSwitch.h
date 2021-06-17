//
// Created by D. Rimron-Soutter on 14/06/2021.
//

#ifndef PITOOLS_LIB_PIUARTSWITCH_H
#define PITOOLS_LIB_PIUARTSWITCH_H

#include <arch/zxn.h>

__sfr __banked __at 0x153b IO_153B;
__sfr __banked __at 0x153b IO_UART_CONTROL;

void piUartSwitch();

#endif //PITOOLS_LIB_PIUARTSWITCH_H
