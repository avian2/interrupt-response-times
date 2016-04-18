all: rpi_c_mmap

rpi_c_mmap: rpi_c_mmap.c c_gpio.c c_gpio.h
	gcc -Wall -o $@ rpi_c_mmap.c c_gpio.c

clean:
	rm -f rpi_c_mmap

.PHONY: all clean
