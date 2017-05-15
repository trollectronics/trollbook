#include "terminal.h"
#include "fat.h"
#include "wav.h"
#include "printf.h"
#include "mem.h"
#include "main.h"

uint8_t wavbuf[17*1024*1024];

void demo_run() {
	int i, j, fd;
	uint32_t size;
	volatile uint32_t *src, *dst;
	volatile uint8_t *tmp;
	uint8_t *wav_buffer;
	
	//terminal_clear();
	
	
	fd = fat_open("/TROLOLO.WAV", O_RDONLY);
	printf("opening file trololo.wav %i\n", fd);
	size = fat_fsize(fd);
	
	printf("allocating %u bytes\n", size);
	//dst = malloc(size);
	dst = (uint32_t *) wavbuf;
	memset(wavbuf, 0, size);
	printf("successfully allocated %u bytes\n", size);
	wav_buffer = (uint8_t *) dst;
	for(j = 0; j < size; j += 512) {
		fat_read_sect(fd);
		src = (uint32_t *) fat_buf;
		for(i = 0; i < 512/4; i++) {
			*dst++ = *src++;
		}
		//printf("\r%u/%u kB", j >> 10, size >> 10);
	}
	fat_close(fd);
	
	fd = fat_open("/TROLOLO.BIN", O_RDONLY);
	size = fat_fsize(fd);
	dst = MEM_VGA_RAM;
	
	for(j = 512; j < size; j += 512) {
		fat_read_sect(fd);
		src = (uint32_t *) fat_buf;
		for(i = 0; i < 512/4; i++) {
			*dst++ = *src++;
		}
	}
	fat_read_sect(fd);
	tmp = (uint8_t *) dst;
	for(i = 0; i < j - size; i++) {
		*tmp++ = fat_buf[i];
	}
	
	fat_close(fd);
	
	wav_play(wav_buffer);
}
