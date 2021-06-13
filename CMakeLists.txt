# Minimum CMake support, to help Code Completing IDEs
cmake_minimum_required(VERSION 3.10)
project(pitools C)

include_directories(
        /u/ZX/z88dk/include/_DEVELOPMENT/sccz80
        src/inc
)

add_executable(crc32
        src/crc32.c
        src/inc/crc32.h src/lib/crc32.c
        src/inc/help.h src/lib/help.c

)
