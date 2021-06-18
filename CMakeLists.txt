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
        src/inc/piSupReset.h src/lib/piSupReset.c
        src/inc/piUartSwitch.h src/lib/piUartSwitch.c
        src/inc/uartDrain.h src/lib/uartDrain.c
        src/inc/uartGetChr.h src/lib/uartGetChr.c
        src/inc/uartSendChr.h src/lib/uartSendChr.c
        src/inc/uartSendCmd.h src/lib/uartSendCmd.c
        src/inc/uartSendStr.h src/lib/uartSendStr.c
        src/inc/uartSetBaud.h src/lib/uartSetBaud.c
        src/inc/uartWaitOK.h src/lib/uartWaitOK.c
        src/inc/uartWaitStr.h src/lib/uartWaitStr.c
        src/inc/nbnSendHeader.h src/lib/nbnSendHeader.c
        src/inc/nbnSendBlock.h src/lib/nbnSendBlock.c
        src/inc/spuiDrawTriangle.h src/lib/spuiDrawTriangle.c)

add_executable(crc32
        src/crc32.c
        src/lib/help.c src/inc/help.h
        src/lib/crc32.c src/inc/crc32.h)
