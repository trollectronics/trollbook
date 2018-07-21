#include <stdbool.h>
#include <stddef.h>
#include <rickmod.h>
#include <terminal.h>
#include <fat.h>
#include <printf.h>
#include <mem.h>
#include <sound.h>
#include <interrupt.h>
#include <peripheral.h>
#include "main.h"

#define ABS(a) ((a) > 0 ? (a) : -(a))
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

static void init() {
	sound_setup((void *) (LLRAM_BASE + 800*480 + 1024));
	
	rm = rm_init(48000, data.modfile, data.modsize);
	rm_repeat_set(rm, false);
	
	rm_mix_u8(rm, (uint8_t *) sound_buffer[0], 512);
	rm_mix_u8(rm, (uint8_t *) sound_buffer[1], 512);
	
	done = false;
	
	interrupt_register(5, audio_callback);
	interrupt_perihperal_enable(PERIPHERAL_ID_AUDIO, 5);
	interrupt_global_enable();
	
	sound_start();
}
static void finished() {
	sound_stop();
	interrupt_global_disable();
	rm_free(rm);
}

void demo() {
	init();
	
	for(;;) {
		gfx_blit_fast(data.tj0, 320, 200, 40, 20);
		gfx_buffer_flip();
		//printf("tj0\n");
		delay_timer(3, 1000);
		gfx_blit_fast(data.h3y, 320, 200, 40, 20);
		gfx_buffer_flip();
		//printf("h3y\n");
		delay_timer(3, 1000);
	}
	
	finished();
}
