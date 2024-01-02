.PIPUT
======
Copies a file from your Next's SD card to NextPi.

Prerequisites
-------------
You must either load the UART speed into NextReg 0x7F manually (2=115.2KBit, 8=2Mbit) or use ".pisend -q" to automatically detect the Pi UART speed before execution.

Expects MMU slots 2 & 3 to be default bank/ULA and Sysvars paged in and active RST16 output to be pointing at Layer 0.

Options
-------

        -q
(quiet) Do not use integrated SPUI library to draw progress bar and track retries/fails/etc.

Errors
------
      5       No such FILE or DIR
File not found on NextPi

      15      File pointer OVERFLOW
Ran out of memory to cache incoming file during transfer

      20      No SYS
Pi not found

All other errors reported relate to ESXDOS error numbers.

Wire Formats
------------

PIPUT uses a customised version of the NextBestNetwork binary transfer protocol (same as in NBNSHELL and NBNGET) this breaks files into 16KB blocks and transmits them one at a time, joining them back up as they're written to the SD card.  This means that if a file is failing to transfer correctly only 16KB at most is required to be retransmitted to recover from the single error.

The sourcecode to the NBN protocol can be found in `/src/lib/nbn*` so you can use these in your own application (including permission to modify, them to make them integrate better, if required) when writing your own software that needs to communicate with NextPi on the condition that you comply with the software license as included in this repository.