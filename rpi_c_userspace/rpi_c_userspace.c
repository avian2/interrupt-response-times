/* In part adapted from:
 *
 * Raspberry Pi GPIO example using sysfs interface.
 * Guillermo A. Amaral B. <g@maral.me>
 *
 * http://elinux.org/RPi_GPIO_Code_Samples
 */ 
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_MAX 40

#define RISING	1
#define FALLING 2

#define IN	0
#define OUT	1

#define PIN	17
#define POUT	18

static int gpio_export(int pin)
{
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;
 
	fd = open("/sys/class/gpio/export", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open export for writing!\n");
		return -1;
	}
 
	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return 0;
}
 
static int gpio_unexport(int pin)
{
	char buffer[BUFFER_MAX];
	ssize_t bytes_written;
	int fd;
 
	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open unexport for writing!\n");
		return(-1);
	}
 
	bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
	write(fd, buffer, bytes_written);
	close(fd);
	return(0);
}

static int gpio_write(int pin, int value)
{
	static const char s_values_str[] = "01";
 
	char path[BUFFER_MAX];
	int fd;
 
	snprintf(path, BUFFER_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio value for writing!\n");
		return -1;
	}
 
	if (1 != write(fd, &s_values_str[value ? 1 : 0], 1)) {
		fprintf(stderr, "Failed to write value!\n");
		return -1;
	}
 
	close(fd);
	return 0;
}

static int gpio_direction(int pin, int dir)
{
	static const char s_directions_str[]  = "in\0out";
 
	char path[BUFFER_MAX];
	int fd;
 
	snprintf(path, BUFFER_MAX, "/sys/class/gpio/gpio%d/direction", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		return -1;
	}
 
	if (-1 == write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3)) {
		fprintf(stderr, "Failed to set direction!\n");
		return -1;
	}
 
	close(fd);
	return 0;
}

static int gpio_edge(int pin, int edge)
{
	static const char s_rising_str[]  = "rising";
	static const char s_falling_str[] = "falling";
 
	char path[BUFFER_MAX];
	int fd;
 
	snprintf(path, BUFFER_MAX, "/sys/class/gpio/gpio%d/edge", pin);
	fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio edge for writing!\n");
		return -1;
	}

	int r = -1;
	if(edge == RISING) {
		r = write(fd, s_rising_str, strlen(s_rising_str)+1);
	} else if(edge == FALLING) {
		r = write(fd, s_falling_str, strlen(s_falling_str)+1);
	}
 
	if (r == -1) {
		fprintf(stderr, "Failed to set edge!\n");
		return -1;
	}
 
	close(fd);
	return 0;
}

static int gpio_wait(int pin)
{
	char path[BUFFER_MAX];
	int fd;
 
	snprintf(path, BUFFER_MAX, "/sys/class/gpio/gpio%d/value", pin);
	fd = open(path, O_RDONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio for reading!\n");
		return -1;
	}

	read(fd, path, 3);

	struct pollfd p = {
		.fd = fd,
		.events = POLLPRI|POLLERR,
	};

	int r = poll(&p, 1, -1);
	if(r != 1) {
		fprintf(stderr, "poll() failed!\n");
		close(fd);
		return -1;
	}

	close(fd);
	return 0;
}
 
 
int main(int argv, char** argc)
{
	if(gpio_export(PIN) || gpio_export(POUT)) {
		return 1;
	}

	gpio_direction(PIN, IN);
	gpio_direction(POUT, OUT);

	while(1) {
		gpio_edge(PIN, RISING);
		gpio_wait(PIN);
		gpio_write(POUT, 1);
		gpio_edge(PIN, FALLING);
		gpio_wait(PIN);
		gpio_write(POUT, 0);
	}

	if(gpio_unexport(PIN) || gpio_unexport(POUT)) {
		return 1;
	}

	return 0;
}
