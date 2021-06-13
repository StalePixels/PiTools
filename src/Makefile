
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

Z88DK_CRT = 30

CC=zcc
CCFLAGS=+zxn -vn -O2 -startup=$(Z88DK_CRT) -clib=new
CZFLAGS=-Cz="--clean --fullsize --main-fence 0xC000"
LDFLAGS=-m -lm
INCFLAGS=-Iinc
BUILD_DIR = ../build

.PHONY: clean crc32

default: all

all: crc32

crc32:
	$(CC) $(CCFLAGS) $(LDFLAGS) $(INCFLAGS) @crc32.lst -ocrc32 -create-app \
		-pragma-include:crc32-pragma.inc -subtype=dotn $(CZFLAGS)
	$(MV) CRC32 $(BUILD_DIR)/

clean:
	rm -f *.o *~ core crc32_*bin crc32.map