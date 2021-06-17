//
// Created by D. Rimron-Soutter on 13/06/2021.
//

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <arch/zxn.h>

#include "uartWaitOK.h"
#include "uartGetChr.h"

uint8_t uartWaitOK(bool localecho) {
    unsigned char cbuff[4];
    repeat:
    cbuff[0] = cbuff[1];
    cbuff[1] = cbuff[2];
    cbuff[2] = cbuff[3];
    cbuff[3] = uartGetChr();

    if(localecho==true) {
        printf("%c%c%c%c(%d,%d,%d,%d)>", cbuff[0], cbuff[1], cbuff[2], cbuff[3], cbuff[0], cbuff[1], cbuff[2],
               cbuff[3]);
    }

    if ((cbuff[3] == 10 && cbuff[2] == 13 && cbuff[1] == 'K' && cbuff[0] == 'O') ||
        (cbuff[3] == 10 && cbuff[2] == 'K' && cbuff[1] == 'O')) {
        if(localecho==true) {
            printf(" OK! \n");
        }
        return 0;
    }
    else if (cbuff[3] == 10 && cbuff[2] == 13 && cbuff[1] == 'R' && cbuff[0] == 'O') {
        if(localecho==true) {
            printf(" ER? \n");
        }

        return 254;
    }
    goto repeat;
}