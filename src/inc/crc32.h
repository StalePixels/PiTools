//
// Created by D. Rimron-Soutter on 13/06/2021
//

#ifndef PITOOLS_CRC32_H
#define PITOOLS_CRC32_H
// //////////////////////////////////////////////////////////
// Crc32.h
// Copyright (c) 2011-2019 Stephan Brumme. All rights reserved.
// Slicing-by-16 contributed by Bulat Ziganshin
// Tableless bytewise CRC contributed by Hagai Gold
// see http://create.stephan-brumme.com/disclaimer.html
//

// uint8_t, uint32_t, int32_t
#include <stdint.h>
// size_t
#include <stdlib.h>

extern const uint32_t Crc32Lookup[8][256]; // extern is needed to keep compiler happy

/// compute CRC32 (Slicing-by-8 algorithm), unroll inner loop 4 times
uint32_t crc32_4x8bytes(const void* data, size_t length, uint32_t previousCrc32);

#endif //PITOOLS_CRC32_H
