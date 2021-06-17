//
// Created by D. Rimron-Soutter on 13/06/2021.
//

#ifndef PITOOLS_ZXN_H
#define PITOOLS_ZXN_H

#define WAIT_FOR_SCANLINE(line)         while (ZXN_READ_REG(REG_ACTIVE_VIDEO_LINE_L) == line); \
                                        while (ZXN_READ_REG(REG_ACTIVE_VIDEO_LINE_L) != line);

#endif //PITOOLS_ZXN_H
