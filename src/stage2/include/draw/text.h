#ifndef DRAW_TEXT_H_
#define DRAW_TEXT_H_

#include <stddef.h>
#include "font.h"

typedef struct DrawTextSurface DrawTextSurface;
struct DrawTextSurface {
	DrawFont *font;
	int x;
	int y;
	size_t bufsiz;
	char *buf;
	unsigned int linelen;
};

DrawTextSurface *draw_text_surface_new(DrawFont *font, size_t bufsiz, unsigned int linelen, int x, int y);
void draw_text_surface_free(DrawTextSurface *surface);
void draw_text_surface_draw(DrawTextSurface *surface);
void draw_text_surface_string_append(DrawTextSurface *surface, char *s);
void draw_text_surface_reset(DrawTextSurface *surface);

#endif
