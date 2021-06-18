//
// Created by D Rimron-Soutter on 07/08/2020.
//

#include <arch/zxn.h>
#include <stdio.h>

#ifndef LIB_ULA_H
#define LIB_ULA_H

#define MACRO__WAITKEY                     while(in_inkey()==0){};while(in_inkey()!=0){};

#define printInk(k)                 printf("\x10%c", '0'+k)
#define printPaper(k)               printf("\x11%c", '0'+k)
#define printFlash(k)               printf("\x12%c",30+k)
#define printFlashOn()              printf("\x12\x31")
#define printFlashOff()             printf("\x12\x30")
#define printBright(k)              printf("\x13%d", k)
#define printBrightOn()             printf("\x13""1")
#define printBrightOff()            printf("\x13""0")
#define printAt(row, col)           printf("\x16%c%c", col, row)
#define printAtStr(row, col, ...)   printf("\x16%c%c%s", col, row, __VA_ARGS__)

#define plot(x, y)                  (*zx_pxy2saddr(x,y) |= zx_px2bitmask(x));

#endif //LIB_ULA_H
