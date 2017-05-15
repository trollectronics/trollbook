#include "modplay.h"
#include "terminal.h"
#include "fat.h"
#include "printf.h"
#include "mem.h"
#include "main.h"

void play(const char *filename) {
	int i, j, fd;
	uint32_t size;
	uint32_t *src, *dst;
	uint8_t *tmp;
	struct RickmodState *rm;
	void *mod_buffer;
	int buffer;
	
	static volatile uint16_t *sound_buffer[] = {
		(volatile uint16_t *) (LLRAM_BASE /*+ 800*480/2*/),
		((volatile uint16_t *) (LLRAM_BASE) /*+ 800*480/2*/ + 512),
	};
	
	printf("open file %s\n", filename);
	fd = fat_open(filename, O_RDONLY);
	size = fat_fsize(fd);
	
	printf("allocating %u bytes\n", size);
	mod_buffer = malloc(size);
	printf("successfully allocated %u bytes\n", size);
	dst = mod_buffer;
	for(j = 0; j < size; j += 512) {
		fat_read_sect(fd);
		src = (uint32_t *) fat_buf;
		for(i = 0; i < 512/4; i++) {
			*dst++ = *src++;
		}
		printf("\r%u/%u kB", j >> 10, size >> 10);
	}
	fat_close(fd);
	printf("\n");
	
	sound_setup();
	
	rm = rm_init(48000, mod_buffer, size);
	printf("MOD: %s\n", rm->name);
	for(i = 0; i < 31; i++) {
		printf("%s\n", rm->sample[i].name);
	}
	
	volatile uint32_t *sound_hw = (volatile uint32_t *) PERIPHERAL_SOUND_BASE;
	//sound_hw[2] = (uint32_t) LLRAM_BASE + 800*480/2;
	
	rm_mix_u8(rm, (uint8_t *) sound_buffer[0], 512);
	rm_mix_u8(rm, (uint8_t *) sound_buffer[1], 512);
	sound_start();
	for(;;) {
		buffer = sound_wait();
		rm_mix_u8(rm, (uint8_t *) sound_buffer[buffer], 512);
	}
}
