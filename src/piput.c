//
// Created by D. Rimron-Soutter on 13/06/2021.
//

#include <arch/zxn.h>
#include <arch/zxn/esxdos.h>

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "help.h"
#include "uart.h"
#include "piSupReset.h"
#include "piUartSwitch.h"
#include "uartSendCmd.h"
#include "uartWaitStr.h"
#include "uartWaitOK.h"
#include "uartSetBaud.h"

#define BUFFER_SIZE 4096
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

bool   progress = true;

int main(int argc, char **argv)
{
    uint8_t filename = 1;

    // One arg, minimum
    if((argc < 2) || (!strcmp(argv[1],"-h"))) exit_with_help("PIPUT");

    if(!strcmp(argv[1],"-q")) {
        progress = false;
        filename++;
    }

    uint8_t data = esxdos_f_open(argv[filename], ESXDOS_MODE_R | ESXDOS_MODE_OE);

    if(errno) {
        return errno;
    }

    struct esxdos_stat finfo;  // = {0,0,0,0,0};
    esxdos_f_fstat(data, &finfo);

//    while(finfo.size > BUFFER_SIZE) {
//        esxdos_f_read(data, buffer, BUFFER_SIZE);
//        finfo.size = finfo.size - BUFFER_SIZE;
//        previousCrc32 = crc32_4x8bytes(buffer, BUFFER_SIZE, previousCrc32);
//    }
//    esxdos_f_read(data, buffer, finfo.size);
//    previousCrc32 = crc32_4x8bytes(buffer, finfo.size, previousCrc32);
//
//    printf("%lx\n", previousCrc32);
    esxdos_f_close(data);

    piUartSwitch();
    uartSetBaud(115200);
    piSupReset();
    uartSendCmd("ls\n");
    uartWaitStr("SUP>");
    uartSendCmd("echo \"OK\"\n");
    uartWaitOK(true);

    return 0;
}