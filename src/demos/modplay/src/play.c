#include <rickmod.h>
#include <terminal.h>
#include <fat.h>
#include <printf.h>
#include <mem.h>
#include <sound.h>
#include <interrupt.h>
#include <peripheral.h>
#include "main.h"

static volatile uint16_t *sound_buffer[] = {
	(volatile uint16_t *) (LLRAM_BASE + 800*480 + 1024),
	(volatile uint16_t *) (LLRAM_BASE + 800*480 + 2048),
};

static int buffer;
static bool done;
static struct RickmodState *rm;

static void audio_callback(int number) {
	volatile uint32_t *sound_hw = (volatile uint32_t *) PERIPHERAL_SOUND_BASE;
	volatile uint32_t *interrupt_hw = (volatile uint32_t *) PERIPHERAL_INTERRUPT_BASE;
	
	rm_mix_u8(rm, (uint8_t *) sound_buffer[buffer], 512);
	
	if(rm_end_reached(rm))
		done = true;
	
	interrupt_hw[32 + PERIPHERAL_ID_AUDIO] = 0x0;
	buffer = (sound_hw[1] & 0x1);
}

static void player_ui() {
	uint16_t vol;
	int vol_l, vol_r;
	int i;
	
	vol = sound_buffer[buffer][0];
	vol_l = (vol >> 8) & 0xFF;
	vol_r = vol & 0xFF;
	
	printf("left: ");
	
	for(i = 0; i < 16; i++) {
		if(vol_l > 0)
			printf("|");
		else
			printf(" ");
		
		vol_l -= 16;
	}
	
	printf(" right: ");
	
	for(i = 0; i < 16; i++) {
		if(vol_r > 0)
			printf("|");
		else
			printf(" ");
		
		vol_r -= 16;
	}
	
}

void play(const char *filename) {
	int i, j, fd;
	uint32_t size;
	uint32_t *src, *dst;
	void *mod_buffer;
	
	terminal_clear();
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
	
	sound_setup((void *) (LLRAM_BASE + 800*480 + 1024));
	
	rm = rm_init(48000, mod_buffer, size);
	rm_repeat_set(rm, false);
	printf("MOD: %s\n", rm->name);
	for(i = 0; i < 31; i++) {
		printf("%s\n", rm->sample[i].name);
	}
	
	//volatile uint32_t *sound_hw = (volatile uint32_t *) PERIPHERAL_SOUND_BASE;
	//sound_hw[2] = (uint32_t) LLRAM_BASE + 800*480;
	
	rm_mix_u8(rm, (uint8_t *) sound_buffer[0], 512);
	rm_mix_u8(rm, (uint8_t *) sound_buffer[1], 512);
	
	done = false;
	terminal_clear();
	
	interrupt_register(1, audio_callback);
	interrupt_perihperal_enable(PERIPHERAL_ID_AUDIO, 1);
	interrupt_global_enable();
	
	sound_start();
	
	volatile uint32_t *sound_hw = (volatile uint32_t *) PERIPHERAL_SOUND_BASE;
	while(!done) {
		//printf("\r%i  ", (sound_hw[1] & 0x1));
		printf("\r");
		player_ui();
	}
	
	sound_stop();
	interrupt_global_disable();
	rm_free(rm);
	free(mod_buffer);
}
