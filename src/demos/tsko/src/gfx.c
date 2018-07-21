#include <stdint.h>
#include <stdbool.h>
#include <peripheral.h>

static unsigned screen_w;

void gfx_set_lowres() {
	volatile uint32_t *vga_hw = (volatile uint32_t *) PERIPHERAL_VGA_BASE;
	vga_hw[0] = 0x3;
	screen_w = 400;
	//input_poll();
	//vga_hw[0] = 0x7; //flip buffer
	//input_poll();
}

void gfx_buffer_flip() {
	
}

void gfx_blit(void *image, int width, int x, int y) {
	
}

void gfx_blit_fast(void *image, unsigned width, unsigned height, unsigned x, unsigned y) {
	uint32_t *from;
	uint32_t *to;
	uint32_t data;
	unsigned initial_x;
	unsigned i, j;
	
	from = image;
	to = (void *) (0x00080000UL + y*screen_w);
	
	width = width >> 2;
	height = height;
	x = x >> 2;
	
	i = 0;
	j = 0;
	
	for(;;) {
		data = *from++;
		to[x + i] = data;
		
		if(++i == width) {
			i = 0;
			if(++j == height)
				return;
			
			to += (screen_w >> 2);
		}
		
	}
	
}
