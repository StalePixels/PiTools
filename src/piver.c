//
// Created by D. Rimron-Soutter on 13/06/2021.
//

#include <arch/zxn.h>
#include <z80.h>
#include <arch/zxn/esxdos.h>

#include <stdio.h>
#include <stdbool.h>
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
#include "uartDrain.h"
#include "uartDump.h"
#include "spuiDrawTriangle.h"

//#define BUFFER_SIZE 4096
//
//uint8_t buffer[BUFFER_SIZE];

#define HELP_TEXT_LENGTH        7


const uint8_t help_text_length = HELP_TEXT_LENGTH;
const char *help_text[HELP_TEXT_LENGTH] = {
        " Usage examples",
        "\n .PIVER -p        print version",
        "\n .PIVER -q       save at RAMTOP",
        "\n .PIVER -b        do both above",
        "\n .PIVER -p -d     print & debug",
        "\n .PIVER -c   force update cache",
        "\nFull Docs at http://zxn.gg/piver"
};

bool                    memdump         = false;
bool                    verbose         = false;
bool                    debug           = false;
bool                    useCache        = true;
bool                    cacheUsed       = false;

uint8_t                 scratch8_1;
uint8_t                 scratch8_2;
uint8_t                 detected_speed;

uint8_t                 ver_major;
uint8_t                 ver_minor;
uint8_t                 ver_patch;

const char              name[] = "PIVER";
const char              cachepath[] = "/sys/piver.cache";
#define VER_MAXLEN      16
char                    ver[VER_MAXLEN+1];

unsigned char orig_cpu_speed;
void at_exit() {
    intrinsic_ei();
    // Finally, restore the original CPU speed
    ZXN_NEXTREGA(REG_TURBO_MODE, orig_cpu_speed);
}

void version() {
    printf("%s\n", VERSION);
}

void exitWithHelp() {
    version();
    help(name);
    printf("%32s\n", "http://zxn.gg/pitools");
    exit(errno);
}

int main(int argc, char **argv)
{
    // Store CPU speed
    orig_cpu_speed = ZXN_READ_REG(REG_TURBO_MODE);
    // Set CPU speed to 28Mhz
    ZXN_NEXTREG(REG_TURBO_MODE, 3);
    // intrinsic_di();
// Ensure we clean up as we shut down...
    atexit(at_exit);

    // One arg, or else
    if(argc < 2) exitWithHelp();

    for(scratch8_1=1; scratch8_1<argc; scratch8_1++) {
        if (*argv[scratch8_1] != '-') {
            exitWithHelp();
        }
        switch (argv[scratch8_1][1]) {
            case 'd':
                debug = true;
                break;
            case 'b':
                verbose = true;
                memdump = true;
                break;
            case 'p':
                verbose = true;
                break;
            case 'q':
                memdump = true;
                break;
            case 'c':
                useCache = false;
                break;
            case 'v':
                version();
                exit(errno);
            default:
                exitWithHelp();
        }
    }

    if(memdump == false && verbose == false) {
        version();
        printf("\nNo return method set (-b/-p/-q)\n");
        exit(13);
    }

    if(memdump == true && (_SYSVAR_RAMTOP > 65365 || _SYSVAR_RAMTOP < 49152)) {
        printAtStr(0,0, "RAMTOP not between 49152 & 65365");
        exit(31);
    }

    uint16_t ram = _SYSVAR_RAMTOP;
    if(memdump && debug) {
        printf("_SYSVAR_RAMTOP: %u\n", ram);

    }

    piUartSwitch();
    detected_speed = ZXN_READ_REG(REG_USER);
    if(detected_speed==2) {
        uartSetBaud(115200);
    }
    else if(detected_speed==8) {
        uartSetBaud(2000000);
    }
    else {
        if(debug) printf("Use .pisend -q to set speed");
        exit(20);
    }

    //If the cache is enabled, let's use it
    if (useCache) {
        scratch8_2 = esxdos_f_open(cachepath, ESXDOS_MODE_R);
        if (errno) {
            goto no_cache;
        }
        scratch8_1 = esxdos_f_read(scratch8_1, ver, 16);
        esxdos_f_close(scratch8_1);

        if (scratch8_1 == 0) goto no_cache;

        if(debug) printf("cache used: %s, %d bytes\n", ver, scratch8_1);

        cacheUsed = true;

        goto ver_got;
    }

    no_cache:
    errno = 0;
    piSupReset(debug);

    uartSendCmd("nextpi-admin_version\n");

    scratch8_1 = 0;
    for(scratch8_2=255;scratch8_2;--scratch8_2) {
        MACRO__WAIT_FOR_SCANLINE(1); //waste some time, in case there's LOTS of output, let it flood the buffer if required!
        get_more:
        if (IO_UART_STATUS & IUS_RX_AVAIL) {  // Busy wait && get a single byte.
            zx_border((uint8_t )scratch8_2%8);
            ver[scratch8_1] = IO_UART_RX;
            if(ver[scratch8_1]==13) goto ver_got;
            scratch8_1++;
            if(scratch8_1>VER_MAXLEN) goto ver_got;
            goto get_more;
        }
    }
    goto fail;

    ver_got:
    // ver[++scratch8_1] = 0;
    if(ver[0] != 'b') {
        if(verbose) printf(ver);

        if(memdump) {
            ver_major = ver[0] - '0';
            ver_minor = ((ver[2] - '0') * 10) + ver[3] - '0';
            ver_patch = ver[4];

            uint8_t * verram = (uint8_t *) ram;
            verram[0] = ver_patch;
            verram[1] = ver_minor;
            verram[2] = ver_major;
        }

        if (!cacheUsed) {
            scratch8_1 = esxdos_f_open(cachepath, ESXDOS_MODE_W | ESXDOS_MODE_CT);
            if (errno) {
                printf("ERROR creating cache file");
                exit(errno);
            }
            scratch8_2 = esxdos_f_write(scratch8_1, ver, 16);
            esxdos_f_close(scratch8_1);
        }
        exit(0);
    }

    fail:
    printf("ERROR retrieving version\n");
    exit(13);
}