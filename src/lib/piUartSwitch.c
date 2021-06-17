//
// Created by D. Rimron-Soutter on 14/06/2021.
//

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <arch/zxn.h>

#include "uart.h"
#include "piUartSwitch.h"

void piUartSwitch() {
    // Select Pi UART
    ZXN_NEXTREG(0xA0, 0x30);  // Enable UART on Pi Bus
    IO_UART_CONTROL = 0x40;   // Direct the UART to the Pi
}
