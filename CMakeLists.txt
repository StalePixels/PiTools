# Minimum CMake support, to help Code Completing IDEs
cmake_minimum_required(VERSION 3.10)
project(pitools C)

include_directories(
        /u/ZX/z88dk/include/_DEVELOPMENT/sccz80
        src/inc
)

add_executable(piput
        src/piput.c
        src/inc/zxn.h
        src/inc/help.h src/lib/help.c
        src/inc/uart.h src/lib/uart.c
        src/inc/piSupReset.h src/lib/piSupReset.c
        src/inc/nbn.h src/lib/nbn.c
        src/inc/nbnSendHeader.h src/lib/nbnSendHeader.c)

add_executable(crc32
        src/crc32.c
        src/lib/help.c src/inc/help.h
        src/lib/crc32.c src/inc/crc32.h)
