
all:
	$(MAKE) -C src all

clean:
	$(MAKE) -C src clean

install:
	$(MAKE) -C src install_crc32
	$(MAKE) -C src install_piget
	$(MAKE) -C src install_piput
	$(MAKE) -C src install_piver

tbblue: clean all
	cp build/* /u/GIT/ZX/tbblue/dot
	cp docs/* /u/GIT/ZX/tbblue/docs/rpi/DotCommands
	
