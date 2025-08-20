//
// Created by D. Rimron-Soutter on 13/06/2021.
//

#include <arch/zxn.h>
#include <z80.h>
#include <arch/zxn/esxdos.h>

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <nbnReceiveBlock.h>
#include <nbn.h>
#include <intrinsic.h>

#include "ula.h"
#include "version.h"
#include "help.h"
#include "uart.h"
#include "zxn.h"
#include "piSupReset.h"
#include "piUartSwitch.h"
#include "uartSendChr.h"
#include "uartSendCmd.h"
#include "uartSendStr.h"
#include "uartWaitStr.h"
#include "uartWaitOK.h"
#include "uartSetBaud.h"
#include "nbnGetHeader.h"
#include "spuiDrawTriangle.h"

#define BUFFER_SIZE 4096

uint8_t buffer[BUFFER_SIZE];

#define HELP_TEXT_LENGTH        6

const uint8_t help_text_length = HELP_TEXT_LENGTH;
const char *help_text[HELP_TEXT_LENGTH] = {
        " Usage examples",
        "\nDownload with progress bar",
        "\n .PIGET /path/to/file.ext",
        "\nDownload without progress bar",
        "\n .PIGET -q /path/to/file.ext",
        "\nFull Docs at http://zxn.gg/piget",
};

unsigned char          *block           = 0x4000;
bool                    verbose         = true;
uint8_t                 filearg         = 1;

const char              rainbow[]       = {'2', '6', '4', '5', '0', '0'};
uint8_t                 rainbow_pointer = 0;

uint16_t                packet_size = NBN_MAX_BLOCKSIZE;
uint8_t                 nbn_retries;
uint8_t                 nbn_status;


uint32_t nbn_size = 0;
uint32_t nbn_blocks = 0;
uint16_t nbn_remainder = 0;

uint8_t                 progress_style  = 0; // slow
unsigned char           progressChar    = ' ';
uint16_t                progress_part   = 0;
uint16_t                progress        = 0;
uint8_t                 file_out;
uint8_t                 top_page, btm_page;

const char              name[] = "PIGET";

unsigned char orig_cpu_speed;
void at_exit() {
    ZXN_WRITE_MMU2(10);
    ZXN_WRITE_MMU3(11);
    intrinsic_ei();
    // Finally, restore the original CPU speed
    ZXN_NEXTREGA(REG_TURBO_MODE, orig_cpu_speed);
}

int main(int argc, char **argv)
{
    // Store CPU speed
    orig_cpu_speed = ZXN_READ_REG(REG_TURBO_MODE);
    // Set CPU speed to 28Mhz
    ZXN_NEXTREG(REG_TURBO_MODE, 3);
    intrinsic_di();
// Ensure we clean up as we shut down...
    atexit(at_exit);

    // One arg, minimum
    if((argc < 2) || (!strcmp(argv[1],"-h"))) {
        printf("%s version %s\n", name, VERSION);
        help(name);
        printf("%32s\n", "http://zxn.gg/pitools");
        exit(errno);
    }

    if(!strcmp(argv[1],"-q")) {
        verbose = false;
        filearg++;
    }

    piUartSwitch();
    uint8_t detected_speed = ZXN_READ_REG(REG_USER);
    if(detected_speed==2) {
        uartSetBaud(115200);
    }
    else if(detected_speed==8) {
        uartSetBaud(2000000);
    }
    else {
        if(verbose) printf("Use .pisend -q to set speed");
        exit(20);
    }
    piSupReset(verbose);

    char *filename = strrchr(argv[filearg], '/');

    if (filename == NULL) {
        filename = argv[filearg];
    }
    else {
        filename++;
    }

    top_page = esx_ide_bank_alloc(0);
    btm_page = esx_ide_bank_alloc(0);

    if(!top_page || ! btm_page) {
        exit(15);
    }

    if(verbose) {
        printf("\x16\x06\x0A\x10""7\x11""0\x13""1 Downloading... \x10""2 ");

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

    uartSendCmd("nextpi-file_transmit -nbn\n");
    uartWaitOK(false);
    uartSendChr(0);
    uartSendStr(argv[filearg]);
    uartSendChr(0);

    nbnGetHeader(&nbn_size, &nbn_blocks, &nbn_remainder);

    if(!nbn_size) {
        exit(5);
    }

    // truncate existing file
    file_out = esxdos_f_open(filename, ESXDOS_MODE_W | ESXDOS_MODE_CT);

    if(errno) {
        return errno;
    }

    uint16_t progress_parts = nbn_blocks + 1;

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
    nbn_status = NBN_BLOCK_SUCCESS;
    for (; nbn_blocks > 0; nbn_blocks--) {
        if(verbose) {
            printf("\x12\x31\x10""6%c", progressChar);
        }

        nbn_retries = NBN_RETRIES;

        receive_next_block:
        uartSendChr(nbn_status);
        ZXN_WRITE_MMU2(btm_page);
        ZXN_WRITE_MMU3(top_page);
        nbn_status = nbnReceiveBlock(packet_size, block);
        ZXN_WRITE_MMU2(10);
        ZXN_WRITE_MMU3(11);

        if (nbn_status == NBN_BLOCK_FAIL) {
            if (--nbn_retries) {
                if(verbose) {
                    printf("\x1C\x10""6\x11""2");  // move cursor back one
                }
                progressChar = '0' + nbn_retries;

                goto receive_next_block;
            } else {
                goto cleanup;
            }
        } else {
            ZXN_WRITE_MMU2(btm_page);
            ZXN_WRITE_MMU3(top_page);
            esxdos_f_write(file_out, block, packet_size);
            ZXN_WRITE_MMU2(10);
            ZXN_WRITE_MMU3(11);

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

    nbn_retries = NBN_RETRIES;

    packet_size = nbn_remainder;

    nbn_retries = NBN_RETRIES;
    uartSendChr(NBN_BLOCK_SUCCESS);

    receive_last_block:
    ZXN_WRITE_MMU2(btm_page);
    ZXN_WRITE_MMU3(top_page);

    nbn_status = nbnReceiveBlock(packet_size, block);

    ZXN_WRITE_MMU2(10);
    ZXN_WRITE_MMU3(11);
    if (nbn_status == NBN_BLOCK_FAIL) {
        if (--nbn_retries) {
            if(verbose) {
                printf("\x1C\x10""6\x11""2");  // move cursor back one
            }
            uartSendChr(nbn_status);
            goto receive_last_block;
        } else {
            goto cleanup;
        }
    } else {
        ZXN_WRITE_MMU2(btm_page);
        ZXN_WRITE_MMU3(top_page);
        esxdos_f_write(file_out, block, packet_size);
        ZXN_WRITE_MMU2(10);
        ZXN_WRITE_MMU3(11);

        if(verbose) {
            printf("\x16\x07\x0C\x11""4\x12\x30                    \x13""0");
        }
    }

cleanup:

    esx_ide_bank_free(top_page, 0);
    esx_ide_bank_free(btm_page, 0);

    esxdos_f_close(file_out);

    return 0;
}
