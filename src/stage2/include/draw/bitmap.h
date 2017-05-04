#ifndef DRAW_BITMAP_H_
#define DRAW_BITMAP_H_

#include <stdbool.h>
#include "color.h"

typedef struct DrawBitmap DrawBitmap;
struct DrawBitmap {
	DrawColor *data;
	bool free_data;
	int x;
	int y;
	int w;
	int h;
};

DrawBitmap *draw_bitmap_new(int w, int h, DrawColor *data);
DrawBitmap *draw_bitmap_new_raw(int w, int h);
void draw_bitmap_free(DrawBitmap *bitmap);
void draw_bitmap_move(DrawBitmap *bitmap, int x, int y);
void draw_bitmap_draw(DrawBitmap *bitmap);

#endif
