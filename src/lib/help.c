#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "help.h"
#include "logo.h"
#include "version.h"

void help(char *name) {
    for(uint8_t help=0;help<help_text_length;help++)
        printf("%s\n", help_text[help]);
}