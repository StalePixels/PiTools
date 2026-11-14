//
// Created by D. Rimron-Soutter on 13/11/2026.
//

#ifndef PITOOLS_LIB_PISUPRESETTIMEOUT_H
#define PITOOLS_LIB_PISUPRESETTIMEOUT_H

#include <stdint.h>
#include <stdbool.h>

// piSupReset with a time limit: drain the UART, send Control D and drain
// again while the Pi sends nothing, then send ENTER and wait for the SUP>
// prompt. All of it takes at most about frames video frames (50 or 60 a
// second); each uartDrain counts as the 100 frames it waits when the UART
// is quiet. Returns true at the SUP> prompt, false when the time ran out.
// It never calls exit().
bool piSupResetTimeout(uint16_t frames);

#endif //PITOOLS_LIB_PISUPRESETTIMEOUT_H
