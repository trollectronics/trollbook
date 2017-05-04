#ifndef _BOOT_TERM_H_
#define	_BOOT_TERM_H_

#include <stdint.h>
#include "peripheral.h"

#define MEM_VGA_RAM ((volatile void *) (LLRAM_BASE + 0x0))

typedef enum TerminalColor TerminalColor;
enum TerminalColor {
	TERMINAL_COLOR_BLACK,
	TERMINAL_COLOR_BLUE,
	TERMINAL_COLOR_GREEN,
	TERMINAL_COLOR_CYAN,
	TERMINAL_COLOR_RED,
	TERMINAL_COLOR_MAGENTA,
	TERMINAL_COLOR_BROWN,
	TERMINAL_COLOR_LIGHT_GRAY,
	TERMINAL_COLOR_GRAY,
	TERMINAL_COLOR_LIGHT_BLUE,
	TERMINAL_COLOR_LIGHT_GREEN,
	TERMINAL_COLOR_LIGHT_CYAN,
	TERMINAL_COLOR_LIGHT_RED,
	TERMINAL_COLOR_LIGHT_MAGENTA,
	TERMINAL_COLOR_YELLOW,
	TERMINAL_COLOR_WHITE,
};

void terminal_init();
void terminal_clear();
void terminal_putc(int c);
void terminal_putc_term(unsigned char c);
void terminal_puts(const char *str);

void terminal_set_pos(int x, int y);
void terminal_get_pos(int *x, int *y);
void terminal_set_fg(uint8_t color);
void terminal_set_bg(uint8_t color);

void terminal_export();
void terminal_import();

#endif
