//
// Created by D. Rimron-Soutter on 13/06/2021.
//

#include <arch/zxn.h>
#include <arch/zxn/esxdos.h>

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <nbnSendBlock.h>

#include "ula.h"
#include "version.h"
#include "help.h"
#include "uart.h"
#include "piSupReset.h"
#include "piUartSwitch.h"
#include "uartSendCmd.h"
#include "uartWaitStr.h"
#include "uartWaitOK.h"
#include "uartSetBaud.h"
#include "nbnSendHeader.h"
#include "spuiDrawTriangle.h"

#define BUFFER_SIZE 4096

#define NBN_RETRIES 3
#define NBN_BLOCK_SUCCESS               '!'
#define NBN_BLOCK_FAIL                  '?'

uint8_t buffer[BUFFER_SIZE];

#define HELP_TEXT_LENGTH        6

const uint8_t help_text_length = HELP_TEXT_LENGTH;
const char *help_text[HELP_TEXT_LENGTH] = {
        " Usage examples",
        "\nUpload to pi & show progress bar",
        "\n\t.PIPUT /path/to/file.ext",
        "\nUpload to pi & hide progress bar",
        "\n\t.PIPUT -q /path/to/file.ext",
        "\nFull Docs at http://zxn.gg/piput",
};

const unsigned char    *block           = 0x4000;
bool                    verbose         = true;
uint8_t                 filearg         = 1;
struct esxdos_stat      finfo;  // = {0,0,0,0,0};

const char              rainbow[]       = {'2', '6', '4', '5', '0', '0'};
uint8_t                 rainbow_pointer = 0;

uint16_t                packet_size;
uint8_t                 nbn_retries;
uint8_t                 nbn_status;
uint8_t                 progress_style  = 0; // slow
unsigned char           progressChar    = ' ';
uint16_t                progress_part   = 0;
uint16_t                progress        = 0;
uint8_t                 file_in;
uint8_t                 top_page, btm_page;

const char              name[] = "PIPUT";

int main(int argc, char **argv)
{
    // One arg, minimum
    if((argc < 2) || (!strcmp(argv[1],"-h"))) {
        printf("%s version %s\n", name, VERSION);
//        logo(name);
        help(name);
        printf("%32s\n", "http://zxn.gg/pitools");
        exit(errno);
    }

    if(!strcmp(argv[1],"-q")) {
        verbose = false;
        filearg++;
    }

    top_page = esx_ide_bank_alloc(0);
    btm_page = esx_ide_bank_alloc(0);

    if(!top_page || ! btm_page) {
        exit(15);
    }

    file_in = esxdos_f_open(argv[filearg], ESXDOS_MODE_R | ESXDOS_MODE_OE);

    if(errno) {
        return errno;
    }

    esxdos_f_fstat(file_in, &finfo);

    piUartSwitch();
    uartSetBaud(115200);
    piSupReset(verbose);

    char *filename = strrchr(argv[filearg], '/');

    if (filename == NULL) {
        filename = argv[filearg];
    }
    else {
        filename++;
    }

    uint32_t nbn_blocks = (finfo.size / NBN_BLOCK_SIZE);
    uint16_t finalblock = finfo.size % NBN_BLOCK_SIZE;

    uint16_t progress_parts = nbn_blocks + 1;

    if(nbn_blocks<21) {
        progress_style = 1; //  fast
        progress_part = 20 / nbn_blocks;
    }
    else {
        progress_part = (nbn_blocks / 20) - 1;
    }

    if(verbose) {
        printf("\x16\x06\x0A\x10""7\x11""0\x13""1 Uploading...   \x10""2 ");

        for (uint8_t col = 22; col < 27; col++) {
            spuiDrawTriangle(col, 10);
            printPaper(rainbow[rainbow_pointer]);
            rainbow_pointer++;
            printf("\x10%c ", rainbow[rainbow_pointer]);
        }

        printf("\x10""0\x11""7\x16\x06\x0B                      "
               "\x16\x06\x0C                      "
               "\x16\x06\x0D______________________");

        for (uint8_t row = 80; row < 104; row++) {
            plot(40, row);
            plot(215, row);
        }
    }

    uartSendCmd("nextpi-file_receive -nbn 256\n");
    uartWaitOK(false);
    nbnSendHeader(finfo.size, filename);
    uartWaitOK(false);

    if(nbn_blocks<21) {
        progress_style = 1; //  fast
        progress_part = 20 / nbn_blocks;
    }
    else {
        progress_part = (nbn_blocks / 20) - 1;
    }

    if(verbose) {
        printf("\x16\x07\x0C\x13""0                    \x16\x07\x0C");
    }
    // FOR BLOCKS
    for(;nbn_blocks>0;nbn_blocks--) {
    load_block:
        if(verbose) {
            printf("\x12\x31\x10""6%c", progressChar);
        }
        ZXN_WRITE_MMU2(btm_page);
        ZXN_WRITE_MMU3(top_page);

        nbn_retries = NBN_RETRIES;
        esxdos_f_read(file_in, block, packet_size);

transmit_block:
        ZXN_WRITE_MMU2(btm_page);
        ZXN_WRITE_MMU3(top_page);

        nbn_status = nbnSendBlock(NBN_BLOCK_SIZE, block);

        ZXN_WRITE_MMU2(10);
        ZXN_WRITE_MMU3(11);

        if(nbn_status==NBN_BLOCK_FAIL) {
            if(--nbn_retries) {
                if(verbose) {
                    printf("\x1C\x10""6\x11""2");  // move cursor back one
                }
                progressChar = '0' + nbn_retries;
                goto transmit_block;
            }
            else {
                goto cleanup;
            }
        }
        else {
            if(verbose) {
                printf("\x1C\x11""4\x12\x30");  // move cursor back one

                if (progress_style) {
                    for (uint8_t iter8 = progress_part; iter8 > 0; --iter8) {
                        putchar(progressChar);
                    }
                } else {
                    ++progress;
                    if (progress >= progress_parts * (progress_part / 20.0f)) {
                        ++progress_part;
                        putchar(progressChar);
                    }
                }
                progressChar = ' ';
            }
        }
    }

    ZXN_WRITE_MMU2(btm_page);
    ZXN_WRITE_MMU3(top_page);

    nbn_retries = NBN_RETRIES;
    esxdos_f_read(file_in, block, finalblock);

transmit_last_block:
    ZXN_WRITE_MMU2(btm_page);
    ZXN_WRITE_MMU3(top_page);

    nbn_status = nbnSendBlock(finalblock, block);

    ZXN_WRITE_MMU2(10);
    ZXN_WRITE_MMU3(11);

    if(verbose) {
        putchar(progressChar);
    }

    if(nbn_status==NBN_BLOCK_FAIL) {
        if(--nbn_retries) {
            if(verbose) {
                printf("\x1C\x10""6\x11""2");  // move cursor back one
            }
            progressChar = '0' + nbn_retries;
            goto transmit_last_block;
        }
        else {
            goto cleanup;
        }
    }
    else {
        if(verbose) {
            printf("\x16\x07\x0C\x11""4\x12\x30                    \x13""0");
        }
    }
cleanup:

    esx_ide_bank_free(top_page, 0);
    esx_ide_bank_free(btm_page, 0);

    esxdos_f_close(file_in);

    return 0;
}