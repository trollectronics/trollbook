#include <stddef.h>
#include <mem.h>
#include <draw/text.h>
#include <draw/screen.h>
#include <peripheral.h>

DrawTextSurface *draw_text_surface_new(DrawFont *font, size_t bufsiz, unsigned int linelen, int x, int y) {
	DrawTextSurface *surface = NULL;
	char *buf = NULL;
	
	if(!(surface = malloc(sizeof(DrawTextSurface))))
		goto fail;
	
	if(!(buf = malloc(bufsiz + 1)))
		goto fail;
	
	*buf = 0;
	
	surface->font = font;
	surface->buf = buf;
	surface->bufsiz = bufsiz;
	surface->linelen = linelen;
	surface->x = x;
	surface->y = y;
	
	return surface;
	
	fail:
	free(buf);
	free(surface);
	return NULL;
}

void draw_text_surface_free(DrawTextSurface *surface) {
	if(surface)
		free(surface->buf);
	free(surface);
}

void draw_text_surface_draw(DrawTextSurface *surface) {
	int i, j;
	int row, col, data;
	int line_x;
	int c;
	char *buf;
	int glyph_w = surface->font->glyph_width;
	int glyph_h = surface->font->glyph_height;
	
	DrawColor fg = draw_color;
	
	//col = pos_x * surface->font->glyph_w;
	//row = pos_y * surface->font->glyph_h;
	
	col = surface->x;
	row = surface->y;
	line_x = 0;
	for(buf = surface->buf; (c = *buf); line_x += glyph_w, buf++) {
		if(line_x + glyph_w > surface->linelen) {
			col = surface->x;
			row += glyph_h;
		}
		
		for (i = 0; i < glyph_h; i++) {
			data = surface->font->mem[c * glyph_h + i];
			for (j = 0; j < glyph_w; j++) {
				if(data & 1)
					draw_framebuffer[(row + i) * DRAW_SCREEN_W + col + j] = fg;
				data >>= 1;
			}
		}
		col += glyph_w;
	}
}

void draw_text_surface_string_append(DrawTextSurface *surface, char *s) {
	if(!surface)
		return;
	if(!surface->buf)
		return;
	
	strcat(surface->buf, s);
}

void draw_text_surface_reset(DrawTextSurface *surface) {
	if(!surface)
		return;
	if(!surface->buf)
		return;
	
	*surface->buf = 0;
}
