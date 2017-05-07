#ifndef DRAW_FONT_H_
#define DRAW_FONT_H_

#include "color.h"

typedef struct DrawFont DrawFont;
struct DrawFont {
	uint8_t *mem;
	unsigned int glyph_width;
	unsigned int glyph_height;
};

DrawFont *draw_font_new(void *mem, unsigned int glyph_width, unsigned int glyph_height);
void draw_font_free(DrawFont *font);
int draw_font_string_geometrics(DrawFont *font, char *s, int max_line_w, int *text_w, int *text_h);
int draw_font_glyph_h(DrawFont *font);
int draw_font_string_w(DrawFont *font, char *s);

#endif
