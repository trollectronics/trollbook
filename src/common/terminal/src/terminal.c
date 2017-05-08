#include <stdint.h>
#include "util.h"
#include "bios.h"
#include "terminal.h"
#include "font.h"

#define TERM_W		100
#define	TERM_H		30

#define W 800
#define H 480

static int pos_x;
static int pos_y;
static uint32_t vsync_count;


uint32_t terminal_get_vsync() {
	return vsync_count;
}


void terminal_init() {
	volatile struct BiosInfo *bi = BIOS_INFO_ADDR;
	int i;
	volatile uint32_t *vgabuff = MEM_VGA_RAM;

	pos_x = 0;
	pos_y = 0;
	
	bi->font = vgafont_data;
	bi->term_x = pos_x;
	bi->term_y = pos_y;
	bi->def_fg = TERMINAL_COLOR_WHITE;
	bi->def_bg = TERMINAL_COLOR_BLACK;
	
	for (i = 0; i < W*H/4; i++)
		vgabuff[i] = 0;
	
	
	//BOOT_TERM_VSYNC_ENABLE();
	
	return;
}

void terminal_clear() {
	int i;
	volatile uint32_t *vgabuff = MEM_VGA_RAM;
	
	for (i = 0; i < W*H/4; i++)
		vgabuff[i] = 0;
	
	pos_x = pos_y = 0;
}

void terminal_putc(int c) {
	volatile struct BiosInfo *bi = BIOS_INFO_ADDR;
	int i, j;
	int row, col, data;
	volatile uint8_t *vgabuff = MEM_VGA_RAM;
	
	uint8_t fg = bi->def_fg;
	uint8_t bg = bi->def_bg;
	
	col = pos_x * 8;
	row = pos_y * 16;

	for (i = 0; i < 16; i++) {
		data = vgafont_data[c * 16 + i];
		for (j = 0; j < 8; j++) {
			vgabuff[(row + i) * W + col + j] = (data & 1) ? fg : bg;
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


void terminal_putc_term(unsigned char c) {
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
		terminal_putc(c);
}


void terminal_puts(const char *str) {
	for (; *str; str++)
		terminal_putc_term((unsigned) *str);
	return;
}


void terminal_set_pos(int x, int y) {
	pos_x = x % (TERM_W + 1);
	pos_y = y % (TERM_H + 1);
}

void terminal_get_pos(int *x, int *y) {
	*x = pos_x;
	*y = pos_y;
}


void terminal_set_fg(uint8_t color) {
	volatile struct BiosInfo *bi = BIOS_INFO_ADDR;
	
	bi->def_fg = color;
	return;
}


void terminal_set_bg(uint8_t color) {
	volatile struct BiosInfo *bi = BIOS_INFO_ADDR;
	
	bi->def_bg = color;
	return;
}


void terminal_export() {
	volatile struct BiosInfo *bi = BIOS_INFO_ADDR;

	bi->font = vgafont_data;
	bi->term_x = pos_x;
	bi->term_y = pos_y;
	bi->def_fg = 15;
	bi->def_bg = 0;

	return;
}


void terminal_import() {
	volatile struct BiosInfo *bi = BIOS_INFO_ADDR;
	
	nop();
	pos_x = bi->term_x;
	pos_y = bi->term_y;
	
	return;
}
