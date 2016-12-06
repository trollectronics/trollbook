/* bios-fupl.c - Steven Arnow <s@rdw.se>,  2016 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdint.h>
#include <libgen.h>
#include <inttypes.h>
#include <stdbool.h>
#include <termios.h>
#include <sys/types.h>
#include <math.h>
#include <limits.h>

static int _get_serial_port(const char *path) {
	int fd, flags;
	struct termios tios;

	if ((fd = open(path, O_RDWR | O_NOCTTY)) < 0) {
		fprintf(stderr, "Unable to open %s\n", path);
		return -1;
	}
	tcgetattr(fd, &tios);

	tios.c_cflag &= ~(CSIZE);
	tios.c_cflag |= CS8;
	tios.c_cc[VMIN] = 1;
	tios.c_cc[VTIME] = 2;
	tios.c_iflag = 0;
	tios.c_oflag = 0;
	tios.c_lflag = 0;
	//cfmakeraw(&tios);
	cfsetospeed(&tios, B115200);
	cfsetispeed(&tios, B115200);
	if (tcsetattr(fd, TCSAFLUSH, &tios) < 0) {
		fprintf(stderr, "Unable to set UART attributes\n");
		close(fd);
		return -1;
	}

	if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
		flags = 0;
//	fcntl(fd, F_SETFL, flags | O_NONBLOCK);

	return fd;

}


FILE *send_header(int fd, const char *filename) {
	FILE *fp;
	uint32_t fsz;
	char dosname[11];
	char *newname, *file;
	uint8_t b;
	int i, j;

	newname = strdup(filename);
	file = basename(newname);

	for (i = j = 0; i < 8; i++) {
		dosname[i] = ' ';
		
		if (file[j] == '.' || !file[j])
			continue;
		dosname[i] = file[j++];
		if (dosname[i] >= 'a' && dosname[i] <= 'z')
			dosname[i] ^= 0x20;
	}

	while (file[j] && file[j] != '.')
		j++;
	while (file[j] == '.')
		j++;
	
	for (; i < 11; i++) {
		dosname[i] = ' ';
		if (!file[j])
			continue;
		if (file[j] == '.' || file[j] == ':');
		else
			dosname[i] = file[j++];
		if (dosname[i] >= 'a' && dosname[i] <= 'z')
			dosname[i] ^= 0x20;
	}

	if (!(fp = fopen(filename, "rb")))
		return NULL;
	write(fd, "                    FISK", 24);
	fseek(fp, 0, SEEK_END);
	fsz = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	b = fsz >> 24;
	write(fd, &b, 1);
	b = fsz >> 16;
	write(fd, &b, 1);
	b = fsz >> 8;
	write(fd, &b, 1);
	b = fsz;
	write(fd, &b, 1);

	write(fd, dosname, 11);
	free(newname);
	return fp;
}


int main(int argc, char **argv) {
	FILE *fp;
	int fd;
	uint8_t buff[256];
	
	
	if (argc <3)
		return fprintf(stderr, "usage: %s <file to send> <serial port>\n", argc?argv[0]:"./bios-fupl.elf"), 1;
	if ((fd = _get_serial_port(argv[2])) < 0)
		return 1;
	
	if (!(fp = send_header(fd, argv[1])))
		return fprintf(stderr, "Unable to open file %s\n", argv[1]), 1;
	while (!feof(fp))
		write(fd, buff, fread(buff, 1, 256, fp));
	return 0;
}
