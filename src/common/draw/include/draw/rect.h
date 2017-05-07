#ifndef DRAW_RECT_H_
#define DRAW_RECT_H_

#include <stddef.h>

typedef struct DrawRect DrawRect;
struct DrawRect {
	int x1;
	int y1;
	int x2;
	int y2;
};

typedef struct DrawRectSet DrawRectSet;
struct DrawRectSet {
	DrawRect *rect;
	size_t rects;
};

DrawRectSet *draw_rect_set_new(size_t rects);
void draw_rect_set_move(DrawRectSet *set, size_t rect, int x1, int y1, int x2, int y2);
void draw_rect_set_get(DrawRectSet *set, size_t rect, int *x1, int *y1, int *x2, int *y2);
void draw_rect_set_draw(DrawRectSet *set, size_t rects);
void draw_rect_set_free(DrawRectSet *set);

#endif
