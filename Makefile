
all:
	$(MAKE) -C src all

clean:
	$(MAKE) -C src clean

install:
	$(MAKE) -C src install_crc32
	$(MAKE) -C src install_piput