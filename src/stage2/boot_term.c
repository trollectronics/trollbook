#include <stdint.h>
#include "util.h"
#include "bios.h"
#include "boot_term.h"
#include "vgafont.h"

#define MEM_VGA_RAM ((volatile void *) 0x80000)

#define TERM_W		88
#define	TERM_H		30

#define W 800
#define H 480

static int pos_x;
static int pos_y;
static uint32_t vsync_count;


uint32_t boot_term_get_vsync() {
	return vsync_count;
}


void term_init() {
	int i;
	volatile uint32_t *vgabuff = MEM_VGA_RAM;

	pos_x = 0;
	pos_y = 0;
	
	for (i = 0; i < W*H/4; i++)
		vgabuff[i] = 0;
	
	
	//BOOT_TERM_VSYNC_ENABLE();
	
	return;
}


void term_putc(int c, int color) {
	int i, j;
	int row, col, data;
	volatile uint8_t *vgabuff = MEM_VGA_RAM;

	col = pos_x * 9;
	row = pos_y * 16;

	for (i = 0; i < 16; i++) {
		data = vgafont_data[c * 16 + i];
		for (j = 0; j < 8; j++) {
			vgabuff[(row + i) * W + col + j] = (data & 1) ? color : 0;
			data >>= 1;
		}
	}

	pos_x++;
	if (pos_x == TERM_W) {
		pos_x = 0, pos_y++;
		if (pos_y == TERM_H)
			/* TODO: Implement scrolling */
			pos_y = TERM_H - 1;
	}
	return;
}


void term_putc_term(unsigned char c, int color) {
	if (c == '\n') {
		pos_y++;
		pos_x = 0;
		if (pos_y == TERM_H)
			/* TODO: Implement scrolling */
			pos_y--;
	} else if (c == '\r')
		pos_x = 0;
	else if (c == '\t') {
		pos_x = (pos_x + 8) & ~7;
	} else
		term_putc(c, color);
}


void term_puts(const char *str, int color) {
	for (; *str; str++)
		term_putc_term((unsigned) *str, color);
	return;
}


void term_set_pos(int x, int y) {
	pos_x = x % (TERM_W + 1);
	pos_y = y % (TERM_H + 1);
}


void term_export() {
	volatile struct BiosInfo *bi = BIOS_INFO_ADDR;

	bi->font = vgafont_data;
	bi->term_x = pos_x;
	bi->term_y = pos_y;
	bi->def_fg = 15;
	bi->def_bg = 0;

	return;
}


void term_import() {
	volatile struct BiosInfo *bi = BIOS_INFO_ADDR;
	
	nop();
	pos_x = bi->term_x;
	pos_y = bi->term_y;
	
	return;
}
