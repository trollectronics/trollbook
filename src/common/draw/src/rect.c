#include <stddef.h>
#include <mem.h>
#include <draw/rect.h>
#include <draw/pixel.h>

#define MIN(a, b) (((a) < (b) ? (a) : (b)))
#define MAX(a, b) (((a) > (b) ? (a) : (b)))

DrawRectSet *draw_rect_set_new(size_t rects) {
	DrawRectSet *set = NULL;
	DrawRect *rect = NULL;
	
	if(!(set = malloc(sizeof(DrawRectSet))))
		goto fail;
	
	if(!(rect = malloc(sizeof(DrawRect)*rects)))
		goto fail;
	
	memset(rect, 0, sizeof(DrawRect)*rects);
	set->rect = rect;
	set->rects = rects;
	
	return set;
	
	fail:
	free(set);
	free(rect);
	
	return NULL;
}

void draw_rect_set_move(DrawRectSet *set, size_t rect, int x1, int y1, int x2, int y2) {
	if(!set)
		return;
	
	if(rect >= set->rects)
		return;
	
	set->rect[rect].x1 = x1;
	set->rect[rect].y1 = y1;
	set->rect[rect].x2 = x2;
	set->rect[rect].y2 = y2;
}

void draw_rect_set_get(DrawRectSet *set, size_t rect, int *x1, int *y1, int *x2, int *y2) {
	if(!set)
		return;
	
	if(rect >= set->rects)
		return;
	
	*x1 = set->rect[rect].x1;
	*y1 = set->rect[rect].y1;
	*x2 = set->rect[rect].x2;
	*y2 = set->rect[rect].y2;
}

void draw_rect_set_draw(DrawRectSet *set, size_t rects) {
	size_t i;
	int x, y;
	
	if(!set)
		return;
	
	rects = MIN(rects, set->rects);
	
	for(i = 0; i < rects; i++) {
		int x1, x2, y1, y2;
		
		x1 = MIN(set->rect[i].x1, set->rect[i].x2);
		y1 = MIN(set->rect[i].y1, set->rect[i].y2);
		x2 = MAX(set->rect[i].x1, set->rect[i].x2);
		y2 = MAX(set->rect[i].y1, set->rect[i].y2);

		for(y = y1; y < y2; y++)
			for(x = x1; x < x2; x++)
				draw_pixel(x, y);
		
	}
}

void draw_rect_set_free(DrawRectSet *set) {
	if(!set)
		return;
	free(set->rect);
	free(set);
}
