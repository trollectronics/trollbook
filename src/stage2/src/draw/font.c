#include <stddef.h>
#include <mem.h>
#include <draw/font.h>

DrawFont *draw_font_new(void *mem, unsigned int glyph_width, unsigned int glyph_height) {
	DrawFont *font = NULL;
	
	if(!(font = malloc(sizeof(DrawFont))))
		goto fail;
	
	font->mem = mem;
	font->glyph_width = glyph_width;
	font->glyph_height = glyph_height;
	
	return font;
	
	fail:
	return NULL;
}

void draw_font_free(DrawFont *font) {
	free(font);
}

int draw_font_string_geometrics(DrawFont *font, char *s, int max_line_w, int *text_w, int *text_h) {
	int lines;
	
	if(!font || !s)
		return -1;
	
	lines = (strlen(s) + (max_line_w - 1))/max_line_w;
	if(lines == 0)
		lines = 1;
	
	if(text_w) {
		if(lines == 1)
			*text_w = font->glyph_width * strlen(s);
		else
			*text_w = font->glyph_width * max_line_w;
	}
	
	if(text_h) {
		*text_h = font->glyph_height * lines;
	}
	
	return 0;
}

int draw_font_glyph_h(DrawFont *font) {
	if(!font)
		return -1;
	
	return font->glyph_height;
}

int draw_font_string_w(DrawFont *font, char *s) {
	if(!font)
		return -1;
	
	return font->glyph_width * strlen(s);
}
