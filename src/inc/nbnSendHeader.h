//
// Created by D. Rimron-Soutter on 17/06/2021.
//

#ifndef PITOOLS_LIB_NBNSENDHEADER_H
#define PITOOLS_LIB_NBNSENDHEADER_H

#define NBN_BLOCK_SIZE          16384

void nbnSendHeader(uint32_t filesize, const char *name);

#endif //PITOOLS_LIB_NBNSENDHEADER_H
