#include <stdint.h>
#include "uart.h"
#include "fat.h"
#include "peripheral.h"
#include "terminal.h"
#include "printf.h"
#include "input.h"
#include "main.h"

typedef struct Header Header;
struct Header {
	uint32_t magic;
	uint32_t size;
	char fname[12];
};

void serial_transfer_recv() {
	Header header = {};
	uint8_t *buf = (void *) SDRAM_BASE, *writebuf;
	char pathbuf[32] = "/0", *tmp;
	int fd, i, j;
	uint8_t c;
	
	uart_flush();
	terminal_clear();
	printf("Serial file transfer\n");
	
	writebuf = buf;
	
	do {
		header.magic <<= 8;
		header.magic |= uart_recv();
	} while(header.magic != 0x4649534BUL);
	
	for(i = 0; i < 4; i++) {
		header.size <<= 8;
		header.size |= uart_recv();
	}
	
	for(i = 0; i < 11; i++) {
		header.fname[i] = uart_recv();
	}
	
	for(i = 0; i < header.size; i++) {
		*writebuf++ = uart_recv();
	}
	
	tmp = pathbuf + 1;
	for(i = 0; i < 8; i++) {
		if(header.fname[i] == ' ')
			break;
		*tmp++ = header.fname[i];
	}
	*tmp++ = '.';
	
	for(i = 8; i < 11; i++) {
		if(header.fname[i] == ' ')
			break;
		*tmp++ = header.fname[i];
	}
	*tmp = 0;
	
	fat_delete_file(pathbuf);
	fat_create_file("/", pathbuf + 1, 0);
	fd = fat_open(pathbuf, O_RDWR);
	fat_set_fsize(pathbuf, header.size);

	for(j = 0; j < header.size; j += 512) {
		for(i = 0; i < 512; i++) {
			fat_buf[i] = *buf++;
		}
		fat_write_sect(fd);
	}
	fat_close(fd);
	printf("Done writing to file %s\n", pathbuf);
	input_poll();
}
