all: rpi_c_userspace

rpi_c_userspace: rpi_c_userspace.c
	gcc -Wall -o $@ $<

clean:
	rm -f rpi_c_userspace

.PHONY: all clean
