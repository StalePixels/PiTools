

.PIVER
======
Interrogates NextPi for the current version details and then exists.

Prerequisites
-------------
You must either load the UART speed into NextReg 0x7F manually (2=115.2KBit, 8=2Mbit) or use ".pisend -q" to automatically detect the Pi UART speed before execution.

Cannot be run from banked code, and expects MMU slots 6 & 7 to be default bank 0/1 EXCEPT when using the -p option.

When not using -p it store the version in the first 3 bytes above RAMTOP (set with CLEAR before execution) in BANK 0/1.

Options
-------

       -b
(both)  Print the current version of NextPi as text on the current channel, and write it to BANK0/1 as indicated via the RAMTOP sysvar

        -d
(debug) Print debug data while communicating with pi

        -h
(help)  Print the helpfile and exit before doing any additional processing - this is the default option if no switch provided

        -p
(print) ONLY return the version of NextPi only as text on the current channel, and not write it to BANK0/1 as indicated via the RAMTOP sysvar

        -q
(quiet) ONLY return the version in Bank0/1 as indicated via the RAMTOP sysvar, do not print to current channel

        -v
(version) Print version of .PIVER and exit

Errors
------
      13      Bad HANDLE
No return method set, retry with -b, -p or -q 

      20      No SYS
Pi not found

      31      Device busy
RAMTOP not in acceptable range

Data Formats
------------

Memory patch version format is 3 bytes:
_RAMTOP: patchlevel
_RAMTOP+1: minor version
_RAMTOP+2: major version

Printed patch version format is M.mmP
M: single ASCII digit, 0 and up
mm: double ASCII digit, 0 to 99
P: single ASCII letter

Letters, generally, reflect the following:
* A - Internal Team (alpha) testing 
* B - Discord Team (beta) testing 
* C - Release Test (candidate) validation
* D - Stable Release (deliverable) package
* E - E and later are hotfixes, no specific meanings
