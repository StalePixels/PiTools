
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
CZFLAGS=-Cz="--clean --fullsize --main-fence 0xC000"
LDFLAGS=-m -lm
INCFLAGS=-Iinc
BUILD_DIR = ../build

INSTALL_DIR = /Volumes/devnext.xalior.com

.PHONY: clean crc32dot piputdot

default: all

all: crc32 piput

lib/%.o: lib/%.c inc/%.h
	$(CC) $(CCFLAGS) $(LDFLAGS) $(INCFLAGS) $< -c -o$@

###
# CRC32
###
CRC32_OBJS := lib/logo.o lib/help.o lib/crc32.o

crc32: $(CRC32_OBJS)
	$(CC) $(CCFLAGS) -startup=30 $(LDFLAGS) $(INCFLAGS) crc32.c $(CRC32_OBJS) -ocrc32 -create-app \
		-pragma-include:crc32-pragma.inc -subtype=dotn $(CZFLAGS)
	$(MV) CRC32 $(BUILD_DIR)/

install_crc32:
	$(CP) $(BUILD_DIR)/CRC32 $(INSTALL_DIR)/dot/

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
	$(CC) $(CCFLAGS) -startup=1 $(LDFLAGS) $(INCFLAGS) piput.c $(PIPUT_OBJS) -opiput -create-app \
		-pragma-include:piput-pragma.inc -subtype=dotn $(CZFLAGS)
	$(MV) PIPUT $(BUILD_DIR)/

install_piput:
	$(CP) $(BUILD_DIR)/PIPUT $(INSTALL_DIR)/dot/

###
# META
###
dist_clean: clean
	rm -f $(BUILD_DIR)/*
clean:
	rm -f lib/*.o *.o *~ *_CODE.bin *_MAIN.bin *_UNASSIGNED.bin *.map+
