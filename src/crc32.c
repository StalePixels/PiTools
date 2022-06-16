//
// Created by D. Rimron-Soutter on 13/06/2021.
//

#include <arch/zxn.h>
#include <arch/zxn/esxdos.h>

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "crc32.h"
#include "help.h"

#define BUFFER_SIZE 4096
uint8_t buffer[BUFFER_SIZE];

#define HELP_TEXT_LENGTH        4
const uint8_t help_text_length = HELP_TEXT_LENGTH;
const char *help_text[HELP_TEXT_LENGTH] = {
        " Usage examples",
        "\nCompute CRC32 checksum of a file",
        "\n\t.CRC32 /path/to/file.ext",
        "\nFull Docs at http://zxn.gg/crc32",
};

static uint32_t previousCrc32 = 0;

const char              name[] = "CRC32";

int main(int argc, char **argv)
{
    // One arg, minimum
    if((argc < 2) || (!strcmp(argv[1],"-h"))) {
        help(name);
        exit(errno);
    }

    uint8_t data = esxdos_f_open(argv[1], ESXDOS_MODE_R | ESXDOS_MODE_OE);

    if(errno) {
        return errno;
    }

    struct esxdos_stat finfo;  // = {0,0,0,0,0};
    esxdos_f_fstat(data, &finfo);

    while(finfo.size > BUFFER_SIZE) {
        esxdos_f_read(data, buffer, BUFFER_SIZE);
        finfo.size = finfo.size - BUFFER_SIZE;
        previousCrc32 = crc32_4x8bytes(buffer, BUFFER_SIZE, previousCrc32);
    }
    esxdos_f_read(data, buffer, finfo.size);
    previousCrc32 = crc32_4x8bytes(buffer, finfo.size, previousCrc32);

    printf("%lx\n", previousCrc32);
    esxdos_f_close(data);

    return 0;
}