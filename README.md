Pi Tools for the Spectrum Next
==============================

crc32
-----
Compute the CRC32 checksum of a file.  CRC32 checksums are used as part of NextPi's Persistent Cache. 

This version uses Slicing-by-8 algorithm, by Intel - as found on https://sourceforge.net/projects/slicing-by-8/ and reasonable sized embedded lookup table to help the speed along.


piput
-----
Sent a file to the Raspberry Pi using the "NBN" 8bit, error correcting protocol while displaying optional progress bar.


piget
-----
Retrieve a file from the Raspberry Pi using the "NBN" 8bit, error correcting protocol while displaying optional progress bar.

piver
-----
Get the current version of NextPi that is running, write it to memory, print to screen, or both.