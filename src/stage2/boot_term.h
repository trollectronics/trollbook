#ifndef _BOOT_TERM_H_
#define	_BOOT_TERM_H_

#include <stdint.h>

void term_init();
void term_putc(int c, int color);
void term_putc_term(unsigned char c, int color);
void term_puts(char *str, int color);

void term_set_pos(int x, int y);
void term_export();
void term_import();

#endif
