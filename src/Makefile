
SH := /bin/sh
ECHO := @/bin/echo -n
CD := cd
RSYNC := rsync -parv
MKDIR := mkdir -p
RM := rm -rfv
LSH := ls -larth
CP := @cp -rv
MV := @mv -v
CAT := `which cat`
SED := `which sed`
LS := ls -l@k

#Z88DK_CRT = 30

CC=zcc
CCFLAGS=+zxn -vn -O2 -clib=new
CZFLAGS=-Cz="--main-fence --clean 0xC000"
LDFLAGS=-m -lm
INCFLAGS=-Iinc
BUILD_DIR = ../build

INSTALL_DIR = /Volumes/devnext.xalior.com
RELEASE_DIR = /u/GIT/ZX/tbblue

.PHONY: clean crc32dot piputdot

default: all

all: crc32 piput piget piver

lib/%.o: lib/%.c inc/%.h
	$(CC) $(CCFLAGS) $(LDFLAGS) $(INCFLAGS) $< -c -o$@

###
# CRC32
###
CRC32_OBJS := lib/logo.o lib/help.o lib/crc32.o

crc32: $(CRC32_OBJS)
	$(CC) $(CCFLAGS) -startup=30 $(LDFLAGS) $(INCFLAGS) crc32.c $(CRC32_OBJS) -oCRC32 -create-app \
		-pragma-include:crc32-pragma.inc -subtype=dotn $(CZFLAGS)
	$(MV) CRC32 $(BUILD_DIR)/

install_crc32:
	$(CP) $(BUILD_DIR)/CRC32 $(INSTALL_DIR)/dot/

test_crc32: crc32 install_crc32


###
# PIPUT
###
PIPUT_OBJS := lib/help.o \
	lib/piUartSwitch.o lib/piSupReset.o \
	lib/uartSetBaud.o lib/uartDrain.o \
	lib/uartSendCmd.o lib/uartWaitOK.o \
	lib/uartWaitStr.o lib/uartSendStr.o \
	lib/uartGetChr.o lib/uartSendChr.o \
	lib/nbnSendHeader.o lib/nbnSendBlock.o \
	lib/spuiDrawTriangle.o

piput: $(PIPUT_OBJS)
	$(CC) $(CCFLAGS) -startup=1 $(LDFLAGS) $(INCFLAGS) piput.c $(PIPUT_OBJS) -oPIPUT -create-app \
		-pragma-include:piput-pragma.inc -subtype=dotn $(CZFLAGS)
	$(MV) PIPUT $(BUILD_DIR)/

install_piput:
	$(CP) $(BUILD_DIR)/PIPUT $(INSTALL_DIR)/dot/

test_piput: piput install_piput


###
# PIGET
###
PIGET_OBJS := lib/help.o \
	lib/piUartSwitch.o lib/piSupReset.o \
	lib/uartSetBaud.o lib/uartDrain.o \
	lib/uartSendCmd.o lib/uartWaitOK.o \
	lib/uartWaitStr.o lib/uartSendStr.o \
	lib/uartGetChr.o lib/uartSendChr.o \
	lib/nbnGetHeader.o lib/nbnReceiveBlock.o \
	lib/spuiDrawTriangle.o

piget: $(PIGET_OBJS)
	$(CC) $(CCFLAGS) -startup=1 $(LDFLAGS) $(INCFLAGS) piget.c $(PIGET_OBJS) -oPIGET -create-app \
		-pragma-include:piget-pragma.inc -subtype=dotn $(CZFLAGS)
	$(MV) PIGET $(BUILD_DIR)/

install_piget:
	$(CP) $(BUILD_DIR)/PIGET $(INSTALL_DIR)/dot/

test_piget: piget install_piget


###
# PIVER
###
PIVER_OBJS := lib/help.o \
	lib/piUartSwitch.o lib/piSupReset.o \
	lib/uartSetBaud.o lib/uartDrain.o lib/uartDump.o \
	lib/uartSendCmd.o lib/uartWaitOK.o \
	lib/uartWaitStr.o lib/uartSendStr.o \
	lib/uartGetChr.o lib/uartSendChr.o

piver: $(PIVER_OBJS)
	$(CC) $(CCFLAGS) -startup=1 $(LDFLAGS) $(INCFLAGS) piver.c $(PIVER_OBJS) -oPIVER -create-app \
		-pragma-include:piver-pragma.inc -subtype=dotn $(CZFLAGS)
	$(MV) PIVER $(BUILD_DIR)/

install_piver:
	$(CP) $(BUILD_DIR)/PIVER $(INSTALL_DIR)/dot/

test_piver: piver install_piver


###
# META
###
install: install_piput install_piget install_piver install_crc32

test: test_piput test_piget test_piver test_crc32

dist_clean: clean
	rm -f $(BUILD_DIR)/*

clean:
	rm -f lib/*.o *.o *~ *_CODE.bin *_MAIN.bin *_UNASSIGNED.bin *.map+

release:
	$(CP) $(BUILD_DIR)/* $(RELEASE_DIR)/dot

CMakeList:
	echo "cmake_minimum_required(VERSION 3.10)" > CMakeLists.txt
	echo "	project (CLionDummy C)" >> CMakeLists.txt
	echo "	include_directories(" >> CMakeLists.txt
	echo "		/u/ZX/z88dk/include/_DEVELOPMENT/sccz80" >> CMakeLists.txt
	echo "		." >> CMakeLists.txt
	echo "	)" >> CMakeLists.txt
	echo "" >> CMakeLists.txt
	echo "add_executable(CLionDummy" >> CMakeLists.txt
	find `pwd` -name \*.c >> CMakeLists.txt
	find `pwd` -name \*.h >> CMakeLists.txt
	find `pwd` -name \*.asm >> CMakeLists.txt
	echo " )" >> CMakeLists.txt
