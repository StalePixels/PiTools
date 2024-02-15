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
#include "uartDrain.h"
#include "uartDump.h"
#include "spuiDrawTriangle.h"

//#define BUFFER_SIZE 4096
//
//uint8_t buffer[BUFFER_SIZE];

#define HELP_TEXT_LENGTH        10


const uint8_t help_text_length = HELP_TEXT_LENGTH;
const char *help_text[HELP_TEXT_LENGTH] = {
        " Usage examples",
        "\nPrint NextPi version",
        "\n .PIVER -p",
        "\nSave NextPi version above RAMTOP",
        "\n .PIVER -q",
        "\nBoth options above at same time",
        "\n .PIVER -b",
        "\nDo both, & print debug output",
        "\n .PIVER -b -d",
        "\nFull Docs at http://zxn.gg/piver"
};

bool                    memdump         = false;
bool                    verbose         = false;
bool                    debug           = false;

const char              name[] = "PIVER";

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
    intrinsic_di();
// Ensure we clean up as we shut down...
    atexit(at_exit);

    // One arg, or else
    if(argc < 2) exitWithHelp();

    for(uint8_t param=1; param<argc; param++) {
        if (strcmp(argv[param], "-d")==0) {
            debug = true;
        } else
        if (strcmp(argv[param], "-b")==0) {
            verbose = true;
            memdump = true;
        } else
        if (strcmp(argv[param], "-p")==0) {
            verbose = true;
        } else
        if (strcmp(argv[param], "-q")==0) {
            memdump = true;
        } else
        if (strcmp(argv[param], "-v")==0) {
            version();
            exit(errno);
        } else {
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
    uint8_t detected_speed = ZXN_READ_REG(REG_USER);
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
    piSupReset(debug);

    uartSendCmd("nextpi-admin_version\n");

#define VER_MAXLEN      16
    uint8_t ver_len = 0;
    char ver[VER_MAXLEN+1];
    memset(ver, 0, VER_MAXLEN+1);


    for(uint8_t checking=255;checking;--checking) {
        MACRO__WAIT_FOR_SCANLINE(1); //waste some time, in case there's LOTS of output, let it flood the buffer if required!
        get_more:
        if (IO_UART_STATUS & IUS_RX_AVAIL) {  // Busy wait && get a single byte.
            zx_border((uint8_t )checking%8);
            ver[ver_len] = IO_UART_RX;
            if(ver[ver_len]==13) goto ver_got;
            ver_len++;
            if(ver_len>VER_MAXLEN) goto ver_got;
            goto get_more;
        }
    }
    goto fail;
    ver_got:
    if(strstr(ver, "bash") == NULL) {
        if(verbose) printf(ver);

        if(memdump) {

            uint8_t ver_major;
            uint8_t ver_minor;
            uint8_t ver_patch;

            ver_major = ver[0] - '0';
            ver_minor = ((ver[2] - '0') * 10) + ver[3] - '0';
            ver_patch = ver[4];

            uint8_t mmu6 = ZXN_READ_REG(REG_MMU6);
            uint8_t mmu7 = ZXN_READ_REG(REG_MMU7);

            ZXN_WRITE_MMU6(0);
            ZXN_WRITE_MMU7(1);

            uint8_t * verram = (uint8_t *) ram;
            verram[0] = ver_patch;
            verram[1] = ver_minor;
            verram[2] = ver_major;

            ZXN_WRITE_MMU6(mmu6);
            ZXN_WRITE_MMU7(mmu7);
        }
        exit(0);
    }

    fail:
    printf("ERROR retrieving version");
    exit(13);
}