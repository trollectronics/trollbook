#ifndef DRAW_FONT_H_
#define DRAW_FONT_H_

#include "color.h"

typedef struct DrawFont DrawFont;
struct DrawFont {
	DrawColor *mem;
	unsigned int glyph_width;
	unsigned int glyph_height;
};

#endif
