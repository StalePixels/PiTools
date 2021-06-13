CP := @cp -rv

INSTALL_DIR = /Volumes/devnext.xalior.com

all:
	$(MAKE) -C src all

clean:
	$(MAKE) -C src clean

install_crc32:
	$(CP) build/CRC32 $(INSTALL_DIR)/dot/

install: install_crc32