//
// Created by D. Rimron-Soutter on 13/11/2026.
//

#ifndef PITOOLS_LIB_UARTWAITSTRTIMEOUT_H
#define PITOOLS_LIB_UARTWAITSTRTIMEOUT_H

#include <stdint.h>
#include <stdbool.h>

// Waits until str comes in on the UART, as uartWaitStr does (CR is skipped),
// for at most frames video frames (50 or 60 a second, whatever the CPU speed).
// Returns true when str came, false when the time ran out.
bool uartWaitStrTimeout(const char* str, uint16_t frames);

#endif //PITOOLS_LIB_UARTWAITSTRTIMEOUT_H
