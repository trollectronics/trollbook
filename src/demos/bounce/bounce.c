#include <stdint.h>
#define MEM_VGA_RAM ((volatile uint8_t *) 0x80000)

#define DISPLAY_W 800
#define DISPLAY_H 480

void dumbdelay(volatile unsigned int count) {
	while(count-- != 0);
}

int main() {
	int i, j;
	int x = 10, y = 10;
	int xvel = 1, yvel = 1;
	int size = 20;
	unsigned char col = 0;
	
	for(i = 0; i < DISPLAY_W*DISPLAY_H; i++)
		MEM_VGA_RAM[i] = 0x0;
	
	for(;;) {
		for(i = y; i < y + size; i++)
			for(j = x; j < x + size; j++)
				MEM_VGA_RAM[i * DISPLAY_W  + j] = 0x0;
		
		x += xvel;
		if(x + size > (DISPLAY_W - 1) || x < 0) {
			xvel = -xvel;
			x += xvel;
		}
		
		y += yvel;
		if(y + size > (DISPLAY_H - 1) || y < 0) {
			yvel = -yvel;
			y += yvel;
		}
		
		for(i = y; i < y + size; i++)
			for(j = x; j < x + size; j++)
				MEM_VGA_RAM[i * DISPLAY_W  + j] = col;
		
		col++;
		dumbdelay(10000);
	}
	
	return 0;
}
